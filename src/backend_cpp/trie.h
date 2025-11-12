#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <vector>
using namespace std;

class TrieNode {
public:
    TrieNode* children[26];
    bool isEndOfWord;
    string fullWord;
    
    TrieNode() {
        for (int i = 0; i < 26; i++) {
            children[i] = nullptr;
        }
        isEndOfWord = false;
        fullWord = "";
    }
};

class Trie {
private:
    TrieNode* root;
    void collectWords(TrieNode* node, vector<string>& results);
    void deleteNode(TrieNode* node);
    
public:
    Trie();
    ~Trie();
    void insert(const string& word);
    bool search(const string& word);
    vector<string> autocomplete(const string& prefix);
};

#endif
