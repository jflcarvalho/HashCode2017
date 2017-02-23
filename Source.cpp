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

class EndPoint {
private:

	std::priority_queue<Cache> caches;
	int dataCenterLatency;
public:
	EndPoint(vector<Cache> caches, int dataCenterLatency, vector<int> endPointToCachesLatency, vector<Request> requests);

};

int main() {

	return 0;
}