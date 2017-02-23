#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <queue>


using namespace std;

void readDataSets(string nameFile){
	ifstream inFile(nameFile);
	string line;

	for (int i = 0; i < 3; i++)
	{
		if (inFile.is_open())
		{
			while (getline(inFile, line))
			{				
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

class Cache
{
private:
	unsigned ID;
	unsigned maxSize;
	vector<Video> videos;

public:
	Cache(unsigned ID, unsigned maxSize) {
		this->ID = ID;
		this->maxSize = maxSize;
	}
};


struct CacheAndLatency {
	Cache *cache;
	int latencyToEndp;
};

class compareCaches
{
public:
	bool operator() (const CacheAndLatency& lhs, const CacheAndLatency&rhs) const
	{
		if (lhs.latencyToEndp < rhs.latencyToEndp) {
			return true;
		}
		return false;
	}
};

class EndPoint {
private:

	std::priority_queue<CacheAndLatency,vector<CacheAndLatency>,compareCaches> caches;
	int dataCenterLatency;
	vector<Request> videoRequests;
public:
	EndPoint(vector<Cache> caches, int dataCenterLatency, vector<int> endPointToCachesLatency, vector<Request> requests) {
		CacheAndLatency a;
		for (int i = 0; i < caches.size(); i++) {
			a.cache = &caches.at(i);
			a.latencyToEndp = endPointToCachesLatency.at(i);
		}
		this->dataCenterLatency = dataCenterLatency;
		this->videoRequests = requests;
	}

};

int main() {

	return 0;
}