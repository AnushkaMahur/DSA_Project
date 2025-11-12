#include "trie.h"
#include <algorithm>

Trie::Trie() {
    root = new TrieNode();
}

Trie::~Trie() {
    deleteNode(root);
}

void Trie::deleteNode(TrieNode* node) {
    if (!node) return;
    for (int i = 0; i < 26; i++) {
        if (node->children[i]) {
            deleteNode(node->children[i]);
        }
    }
    delete node;
}

void Trie::insert(const string& word) {
    TrieNode* current = root;
    string wordLower = word;
    transform(wordLower.begin(), wordLower.end(), wordLower.begin(), ::tolower);
    
    for (char c : wordLower) {
        if (c < 'a' || c > 'z') continue; // Skip non-alphabetic
        int index = c - 'a';
        if (!current->children[index]) {
            current->children[index] = new TrieNode();
        }
        current = current->children[index];
    }
    
    current->isEndOfWord = true;
    current->fullWord = word;
}

bool Trie::search(const string& word) {
    TrieNode* current = root;
    string wordLower = word;
    transform(wordLower.begin(), wordLower.end(), wordLower.begin(), ::tolower);
    
    for (char c : wordLower) {
        if (c < 'a' || c > 'z') continue;
        int index = c - 'a';
        if (!current->children[index]) {
            return false;
        }
        current = current->children[index];
    }
    
    return current != nullptr && current->isEndOfWord;
}

vector<string> Trie::autocomplete(const string& prefix) {
    vector<string> results;
    TrieNode* current = root;
    string prefixLower = prefix;
    transform(prefixLower.begin(), prefixLower.end(), prefixLower.begin(), ::tolower);
    
    // Navigate to prefix node
    for (char c : prefixLower) {
        if (c < 'a' || c > 'z') continue;
        int index = c - 'a';
        if (!current->children[index]) {
            return results; // No matches
        }
        current = current->children[index];
    }
    
    // Collect all words from this node
    collectWords(current, results);
    return results;
}

void Trie::collectWords(TrieNode* node, vector<string>& results) {
    if (!node) return;
    
    if (node->isEndOfWord) {
        results.push_back(node->fullWord);
    }
    
    for (int i = 0; i < 26; i++) {
        if (node->children[i]) {
            collectWords(node->children[i], results);
        }
    }
}
