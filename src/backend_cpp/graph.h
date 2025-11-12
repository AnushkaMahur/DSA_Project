#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <map>
using namespace std;

class RecommendationGraph {
private:
    map<string, vector<string>> adjacencyList;
    
public:
    void addEdge(const string& product1, const string& product2);
    vector<string> getRecommendations(const string& productName, int maxResults = 5);
    void loadRecommendations(const string& filename);
};

#endif
