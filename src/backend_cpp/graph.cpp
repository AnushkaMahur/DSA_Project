#include "graph.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <set>

void RecommendationGraph::addEdge(const string& product1, const string& product2) {
    string p1Lower = product1;
    string p2Lower = product2;
    transform(p1Lower.begin(), p1Lower.end(), p1Lower.begin(), ::tolower);
    transform(p2Lower.begin(), p2Lower.end(), p2Lower.begin(), ::tolower);
    
    adjacencyList[p1Lower].push_back(p2Lower);
    adjacencyList[p2Lower].push_back(p1Lower);
}

vector<string> RecommendationGraph::getRecommendations(const string& productName, int maxResults) {
    string nameLower = productName;
    transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);
    
    vector<string> recommendations;
    set<string> visited;
    
    if (adjacencyList.find(nameLower) != adjacencyList.end()) {
        for (const string& neighbor : adjacencyList[nameLower]) {
            if (visited.find(neighbor) == visited.end() && neighbor != nameLower) {
                recommendations.push_back(neighbor);
                visited.insert(neighbor);
                
                if (recommendations.size() >= maxResults) break;
            }
        }
    }
    
    return recommendations;
}

void RecommendationGraph::loadRecommendations(const string& filename) {
    // Build simple co-purchase graph
    // Format: product1|product2
    ifstream file(filename);
    if (!file.is_open()) {
        return; 
    }
    
    string line;
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        stringstream ss(line);
        string prod1, prod2;
        getline(ss, prod1, '|');
        getline(ss, prod2);
        
        addEdge(prod1, prod2);
    }
    
    file.close();
}
