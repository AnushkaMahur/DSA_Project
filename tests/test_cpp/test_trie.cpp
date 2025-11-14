#include <iostream>
#include <vector>
#include "../../src/backend_cpp/trie.h"

using namespace std;

int main() {
    Trie trie;
    trie.insert("laptop");
    trie.insert("lamp");
    trie.insert("charger");

    vector<string> results = trie.searchPrefix("la");

    if (results.size() == 2) {
        cout << "[PASS] Trie prefix search works correctly.\n";
    } else {
        cout << "[FAIL] Trie prefix search returned wrong output.\n";
    }

    return 0;
}
