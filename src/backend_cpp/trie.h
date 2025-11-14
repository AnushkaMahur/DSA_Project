#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <vector>
using namespace std;

class TrieNode {
public:
    TrieNode* children[26];    //pointers for each alphabet
    bool isEndOfWord;
    string fullWord;    //actual word
    
    TrieNode() {
        for (int i = 0; i < 26; i++) {
            children[i] = nullptr;    // initialize all children as empty
        }
        isEndOfWord = false;
        fullWord = "";
    }
};

//trie for auto complete and fast prefix searches
class Trie {
private:
    TrieNode* root;    //root node of the trie
    void collectWords(TrieNode* node, vector<string>& results);
    void deleteNode(TrieNode* node);    //free nodes recursively
    
public:
    Trie();    //create empty trie
    ~Trie();    //free memory
    void insert(const string& word);    //insert a word into trie
    bool search(const string& word);    //check if a word exists
    vector<string> autocomplete(const string& prefix);    //all words starting with prefix
};

#endif
