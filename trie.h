#ifndef clox_trie_h
#define clox_trie_h

#include <string.h>

#include "common.h"
#include "scanner.h"

typedef struct{
    char character;
    bool word_end;
    TokenType token;
    TrieNode* children[26];
} TrieNode;

typedef struct{
    TrieNode* root;
} Trie;

TrieNode* createTrieNode(char character);
Trie* createTrie();
Trie* createKeywordTrie(const char* words[], TokenType tokens[], int num_words);
void insert(Trie* trie, const char* word, TokenType token);
TokenType test(Trie* trie, const char* word);

#endif