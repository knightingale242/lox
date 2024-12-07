#include <stdio.h>

#include "trie.h"
#include "scanner.h"

TrieNode* createTrieNode(char character){
    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
    node->character = character;
    node->word_end = false;
    for (int i = 0; i < 26; i++){
        node->children[i] = NULL;
    }

    return node;
}

Trie* createTrie(){
    Trie* trie = (Trie*)malloc(sizeof(Trie));
    trie->root = createTrieNode('\0');
    return trie;
}

/*
only takes words that are all lowercase :o
*/
void insert(Trie* trie, const char* word, TokenType token){
    TrieNode* temp = trie->root;
    int length = strlen(word);

    for(int i = 0; i < length; i++){
        int index = word[i] - 'a';
        if (temp->children[index] == NULL){
            temp->children[index] = createTrieNode(word[i]);   
        }
        temp = temp->children[index];
    }
    temp->word_end = true;
    temp->token = token;
}

Trie* createKeywordTrie(const char* words[], TokenType tokens[], int num_words){
    Trie* trie = createTrie();

    for (int i = 0; i < num_words; i++){
        insert(trie, words[i], tokens[i]);
    }

    return trie;
}

TokenType test(Trie* trie, const char* word){
    TrieNode* current = trie->root;
    int length = strlen(word);

    for (int i = 0; i < length; i++){
        int index = word[i] - 'a';
        if (current->children[index] == NULL){
            return TOKEN_ERROR;
        }
        current = current->children[index];
    }

    if (current->word_end == true){
        return current->token;
    }
    return TOKEN_ERROR;
}