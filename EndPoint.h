#include <queue>
#include <vector>

using namespace std;
class EndPoint {
private:

	std::priority_queue<Cache> caches;
	int dataCenterLatency;
public: 
	EndPoint(vector<Cache> caches, int dataCenterLatency, vector<int> endPointToCachesLatency, vector<Request> requests);

};