#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <sstream>


using namespace std;

void readDataSets(string nameFile) {
	ifstream inFile(nameFile);
	stringstream sstream;
	string line;

	unsigned videosNumber,
		endpointsNumber,
		requestNumber,
		cacheNumber,
		cacheSize;

	Video newVideo;
	vector<Video> videos;

	EndPoint newEndPoint;
	vector<EndPoint> endPoints;

	unsigned dataCenterLantency,
		connectCachesNumber;

	vector <Cache> caches;


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
				for (unsigned i = 0; i < videosNumber; i++)
				{
					newVideo.ID = i;
					sstream >> newVideo.size;
					videos.push_back(newVideo);
				}
				sstream.clear();
				line.clear();
			}
			if (endpointsNumber != 0)
			{
				for (int i = 0; i < endpointsNumber; i++)
				{
					getline(inFile, line);
					sstream << line;
					sstream >> dataCenterLantency;
					sstream >> connectCachesNumber;
					newEndPoint.setLatencyToDataCenter(dataCenterLantency);
					sstream.clear();
					line.clear();

				}

			}

		}
	}

}

struct Video
{
	unsigned ID;
	unsigned size;
};

struct Request
{
	Video videoRequest;
	unsigned numberOfRequests;
};

unsigned Cache::nextID = 0;

class Cache
{
private:
	static unsigned nextID;
	static unsigned maxSize;
	unsigned ID;
	unsigned actualSize;
	vector<Video> videos;

public:
	Cache(unsigned maxSize) {
		this->ID = nextID;
		nextID++;
	}
	void addVideo(Video video) {
		videos.push_back(video);
		actualSize -= video.size;
	}
	void removeVideo(Video video) {
		find(videos.begin(), videos.end(), video);
	}
	unsigned int getActualSize() {
		return actualSize;
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
	return ((double)lhs.numberOfRequests / lhs.videoRequest.size < (double)rhs.numberOfRequests / rhs.videoRequest.size);
}


class EndPoint {
private:

	vector<CacheAndLatency> caches;
	int dataCenterLatency;
	vector<Request> videoRequests;
public:
	EndPoint();
	void setCaches(vector<Cache> caches, vector<int> endPointToCachesLatency) {
		CacheAndLatency a;
		for (int i = 0; i < caches.size(); i++) {
			a.cache = &caches.at(i);
			a.latencyToEndp = endPointToCachesLatency.at(i);
			this->caches.push_back(a);
		}
		sort(caches.begin(), caches.end(), compareCaches);
	}
	void setLatencyToDataCenter(int latency) {
		dataCenterLatency = latency;
	}
	void setVideoRequests(vector<Request> requests) {
		this->videoRequests = requests;
		sort(videoRequests.begin(), videoRequests.end(), compareRequest);
	}


	EndPoint(vector<Cache> caches, int dataCenterLatency, vector<int> endPointToCachesLatency, vector<Request> requests) {
		CacheAndLatency a;
		for (int i = 0; i < caches.size(); i++) {
			a.cache = &caches.at(i);
			a.latencyToEndp = endPointToCachesLatency.at(i);
			this->caches.push_back(a);
		}
		sort(caches.begin(), caches.end(), compareCaches);
		this->dataCenterLatency = dataCenterLatency;
		this->videoRequests = requests;
		sort(videoRequests.begin(), videoRequests.end(), compareRequest);
	}
	void addVideosToCaches() {
		for (int k = 0; k < caches.size(); k++) {
			while (videoRequests.size() >= 0 && caches.at(k).cache->getActualSize() > videoRequests.at(0).videoRequest.size) {
				caches.at(k).cache->addVideo(videoRequests.at(0).videoRequest);
				videoRequests.erase(videoRequests.begin());
			}
			
		}
		
	}

};

int main() {
	string filename = "test.ini";
	vector<EndPoint> endPoints = readDataSets(filename);
	for (int i = 0; i < endPoints.size(); i++) {
		endPoints.at(i).addVideosToCaches();
	}
	

	return 0;
}


