#include <iostream>
#include "../../src/backend_cpp/graph.h"

using namespace std;

int main() {
    Graph g;

    g.addEdge("Laptop", "Charger");
    g.addEdge("Laptop", "Mouse");

    vector<string> neighbors = g.getNeighbors("Laptop");

    if (neighbors.size() == 2) {
        cout << "[PASS] Graph adjacency list works correctly.\n";
    } else {
        cout << "[FAIL] Graph neighbors count incorrect.\n";
    }

    return 0;
}
