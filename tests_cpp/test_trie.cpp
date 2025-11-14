#include <iostream>
#include "../../src/backend_cpp/trie.h"

int main() {
    Trie trie;
    trie.insert("laptop");
    trie.insert("lamp");
    trie.insert("charger");

    // Test prefix search
    auto results = trie.searchPrefix("la");

    if (results.size() == 2) {
        std::cout << "[PASS] Trie prefix search works correctly.\n";
    } else {
        std::cout << "[FAIL] Trie prefix search returned wrong output.\n";
    }

    return 0;
}
