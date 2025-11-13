#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <map>
using namespace std;

// Graph that stores product recommendations
class RecommendationGraph {
private:
    map<string, vector<string>> adjacencyList;
    
public:
    void addEdge(const string& product1, const string& product2);    // Connect two products (means they are related)
    vector<string> getRecommendations(const string& productName, int maxResults = 5); // Get recommended products for a given product
    void loadRecommendations(const string& filename);
};

#endif
