#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <sstream>


using namespace std;


struct Video
{
	int ID;
	int size;
};

struct Request
{
	Video videoRequest;
	int numberOfRequests;
};



class Cache
{
private:
	static int nextID;
	static int maxSize;
	int ID;
	int actualSize;
	vector<Video> videos;

public:
	Cache(int maxSize) {
		this->maxSize = maxSize;
		this->ID = nextID;
		nextID++;
		actualSize = maxSize;
	}
	int getID() {
		return ID;
	}
	vector<Video> getVideos() {
		return videos;
	}
	void addVideo(Video &video) {
		videos.push_back(video);
		actualSize -= video.size;
	}
	int getActualSize() {
		return actualSize;
	}
	int getMaxSize() {
		return maxSize;
	}
};


struct CacheAndLatency {
	Cache *cache;
	int latencyToEndp;
};

bool compareCaches(const CacheAndLatency& lhs, const CacheAndLatency&rhs)
{
	if (lhs.latencyToEndp < rhs.latencyToEndp) {
		return true;
	}
	return false;
}

bool compareRequest(const Request& lhs, const Request&rhs)
{
	return ((double)lhs.numberOfRequests / lhs.videoRequest.size > (double)rhs.numberOfRequests / rhs.videoRequest.size);
}

int Cache::nextID = 0;
int Cache::maxSize = 0;



class EndPoint {
private:
	static int nextID;
	int ID;
	vector<CacheAndLatency> caches;
	int dataCenterLatency;
	vector<Request> videoRequests;
public:
	EndPoint() {
		this->ID = nextID;
		nextID++;
	}
	vector<CacheAndLatency> getCaches() {
		return caches;
	}

	void setCaches(vector<Cache> caches, vector<int> endPointToCachesLatency) {
		CacheAndLatency a;
		for (int i = 0; i < caches.size(); i++) {
			a.cache = &caches.at(i);
			a.latencyToEndp = endPointToCachesLatency.at(i);
			this->caches.push_back(a);
		}
		sort(this->caches.begin(), this->caches.end(), compareCaches);
	}

	void addCaches(CacheAndLatency a) {
		caches.push_back(a);
		sort(caches.begin(), caches.end(), compareCaches);
	}

	void setLatencyToDataCenter(int latency) {
		dataCenterLatency = latency;
	}

	void setVideoRequests(vector<Request> requests) {
		this->videoRequests = requests;
		sort(videoRequests.begin(), videoRequests.end(), compareRequest);
	}

	void addVideoRequest(Request r) {
		videoRequests.push_back(r);
		sort(videoRequests.begin(), videoRequests.end(), compareRequest);
	}

	int getID() {
		return this->ID;
	}

	EndPoint(vector<Cache> caches, int dataCenterLatency, vector<int> endPointToCachesLatency, vector<Request> requests) {
		CacheAndLatency a;
		for (int i = 0; i < caches.size(); i++) {
			a.cache = &caches.at(i);
			a.latencyToEndp = endPointToCachesLatency.at(i);
			this->caches.push_back(a);
		}
		sort(this->caches.begin(), this->caches.end(), compareCaches);
		this->dataCenterLatency = dataCenterLatency;
		this->videoRequests = requests;
		sort(videoRequests.begin(), videoRequests.end(), compareRequest);
	}
	void addVideosToCaches() {
		for (int k = 0; k < caches.size(); k++) {
			while (videoRequests.size() > 0 && caches.at(k).cache->getActualSize() >= videoRequests.at(0).videoRequest.size) {
				caches.at(k).cache->addVideo(videoRequests.at(0).videoRequest);
				videoRequests.erase(videoRequests.begin());
			}
			
		}
		
	}

};
int EndPoint::nextID = 0;

void printResult(vector<EndPoint> &endpoints) {
	ofstream out("a.out");
	int cachesUsed = 0;
	for (int i = 0; i < endpoints.size(); i++) {
		for (int c = 0; c < endpoints.at(i).getCaches().size(); c++) {
			if (endpoints.at(i).getCaches().at(c).cache->getActualSize() != endpoints.at(i).getCaches().at(c).cache->getMaxSize()) {
				cachesUsed++;
			}
		}
	}
	out << cachesUsed << "\n";
	for (int i = 0; i < endpoints.size(); i++) {
		for (int c = 0; c < endpoints.at(i).getCaches().size(); c++) {
			out << endpoints.at(i).getCaches().at(c).cache->getID();
			for (int v = 0; v < endpoints.at(i).getCaches().at(c).cache->getVideos().size(); v++) {
				out << " " << endpoints.at(i).getCaches().at(c).cache->getVideos().at(v).ID;
			}
			out << "\n";
 		}
	}
}

void readDataSets(string nameFile, vector<EndPoint> &endPoints, vector<Cache> &caches) {
	ifstream inFile(nameFile);
	stringstream sstream;
	string line;

	int videosNumber,
		endpointsNumber,
		requestNumber,
		cacheNumber,
		cacheSize;

	Video newVideo;
	vector<Video> videos;


	int dataCenterLantency,
		connectCachesNumber;

	int cacheID, lantencyToEP;

	int videoID,
		endPointID,
		videoRequestNumbers;



	for (int i = 0; i < 3; i++)
	{
		if (inFile.is_open())
		{
			getline(inFile, line);
			sstream << line;
			sstream >> videosNumber;
			sstream >> endpointsNumber;
			sstream >> requestNumber;
			sstream >> cacheNumber;
			sstream >> cacheSize;
			for (int i = 0; i < cacheNumber; i++)
			{
				Cache newCache(cacheSize);
				caches.push_back(newCache);
			}
			sstream.clear();
			line.clear();
			if (videosNumber != 0)
			{
				getline(inFile, line);
				sstream << line;
				for (int i = 0; i < videosNumber; i++)
				{
					newVideo.ID = i;
					sstream >> newVideo.size;
					videos.push_back(newVideo);
				}
				sstream.clear();
				line.clear();
			}
			for (int i = 0; i < endpointsNumber; i++)
			{
				EndPoint newEndPoint;
				getline(inFile, line);
				sstream << line;
				sstream >> dataCenterLantency;
				sstream >> connectCachesNumber;
				newEndPoint.setLatencyToDataCenter(dataCenterLantency);
				sstream.clear();
				line.clear();
				for (int j = 0; j < connectCachesNumber; j++)
				{
					getline(inFile, line);
					sstream << line;
					sstream >> cacheID;
					sstream >> lantencyToEP;
					for (int k = 0; k < caches.size(); k++)
					{
						if (caches.at(k).getID() == cacheID)
						{
							CacheAndLatency newCacheAndLantecy;
							newCacheAndLantecy.cache = &caches.at(k);
							newCacheAndLantecy.latencyToEndp = lantencyToEP;
							newEndPoint.addCaches(newCacheAndLantecy);
							break;
						}
					}
					sstream.clear();
					line.clear();
				}
				endPoints.push_back(newEndPoint);
			}
			for (int i = 0; i < requestNumber; i++)
			{
				getline(inFile, line);
				sstream << line;
				sstream >> videoID;
				sstream >> endPointID;
				sstream >> videoRequestNumbers;
				Request newRequest;
				newRequest.numberOfRequests = videoRequestNumbers;
				for (int j = 0; j < videos.size(); j++)
				{
					if (videos.at(j).ID == videoID)
					{
						newRequest.videoRequest = videos.at(j);
						break;
					}
				}
				for (int j = 0; j < endPoints.size(); j++)
				{
					if (endPoints.at(j).getID() == endPointID)
					{
						endPoints.at(j).addVideoRequest(newRequest);
					}
				}
				sstream.clear();
				line.clear();
			}
			inFile.close();
		}
	}
}
int main() {
	string filename = "me_at_the_zoo.in";
	vector<EndPoint> endPoints;
	vector<Cache> caches;
	readDataSets(filename, endPoints,caches);
	for (int i = 0; i < endPoints.size(); i++) {
		endPoints.at(i).addVideosToCaches();
	}
	printResult(endPoints);
	

	return 0;
}


