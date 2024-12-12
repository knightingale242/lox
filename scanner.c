#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "common.h"
#include "scanner.h"

typedef struct{
    const char* start;
    const char* current;
    int line;
    struct TrieNode* keyword_trie;
} Scanner;

Scanner scanner;

struct TrieNode* createTrieNode(char character){
    struct TrieNode* node = (struct TrieNode*)malloc(sizeof(struct TrieNode));
    node->character = character;
    node->word_end = false;
    for (int i = 0; i < 26; i++){
        node->children[i] = NULL;
    }

    return node;
}

struct TrieNode* createTrie(){
    struct TrieNode* trie = (struct TrieNode*)malloc(sizeof(struct TrieNode));
    trie = createTrieNode('\0');
    return trie;
}

/*
only takes words that are all lowercase :o
*/
void insert(struct TrieNode* trie, const char* word, TokenType token){
    struct TrieNode* temp = trie;
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

struct TrieNode* createKeywordTrie(const char* words[], TokenType tokens[], int num_words){
    struct TrieNode* trie = createTrie();

    for (int i = 0; i < num_words; i++){
        insert(trie, words[i], tokens[i]);
    }

    return trie;
}

TokenType test(struct TrieNode* trie, const char* word){
    struct TrieNode* current = trie;
    int length = strlen(word);

    for (int i = 0; i < length; i++){
        int index = word[i] - 'a';
        if (current->children[index] == NULL){
            return TOKEN_ERROR;
        }
        current = current->children[index];
    }

    if (current->word_end == true){
        if (current->token == TOKEN_PRINT){
        }
        return current->token;
    }
    return TOKEN_ERROR;
}

static bool isAtEnd(){
    return *scanner.current == '\0';
}

static bool isDigit(char c) {
  return c >= '0' && c <= '9';
}

static bool isAlpha(char c) {
  return (c >= 'a' && c <= 'z') ||
         (c >= 'A' && c <= 'Z') ||
          c == '_';
}

static char advance(){
    scanner.current++;
    return scanner.current[-1];
}

static char peek(){
    return *scanner.current;
}

static char peekNext(){
    if (isAtEnd()){
        return '\0';
    }
    return scanner.current[1];
}

static bool match(char expected){
    if (isAtEnd()){
        return false;
    }
    if (*scanner.current != expected){
        return false;
    }
    scanner.current++;
    return true;
}

static Token makeToken(TokenType type){
    Token token;
    token.type = type;
    token.start = scanner.start;
    token.length = (int)(scanner.current - scanner.start);
    token.line = scanner.line;
    return token;
}

static Token errorToken(const char* message){
    Token token;
    token.type = TOKEN_ERROR;
    token.start = message;
    token.length = (int)strlen(message);
    token.line = scanner.line;
    return token;
}

static void skipWhitespace(){
    for(;;){
        char c = peek();
        switch (c){
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                scanner.line++;
                advance();
                break;
            case '/':
                if (peekNext() == '/'){
                    while (peek() != '\n' && !isAtEnd()){
                        advance();
                    }
                } else{
                    return;
                }
                break;
            default:
                return;
        }
    }
}

char* getSubstring(const char* start, const char* current){
    size_t length = current - start;
    char* result = (char*)malloc(length + 1);
    memcpy(result, start, length);
    result[length] = '\0';
    return result;
}

static TokenType identifierType(){
    const char* word = getSubstring(scanner.start, scanner.current);
    TokenType token = test(scanner.keyword_trie, word);

    if (token != TOKEN_ERROR){
        return token;
    }
    return TOKEN_IDENTIFIER;
}

static Token string(){
    while (peek() != '"' && !isAtEnd()){
        if (peek() == '\n'){
            scanner.line++;
        }
        advance();
    }

    if (isAtEnd()){
        return errorToken("Unterminated String");
    }

    advance();
    return makeToken(TOKEN_STRING);
}

static Token number(){
    while (isDigit(peek())){
        advance();
    }

    if(peek() == '.' && isDigit(peekNext())){
        advance();

        while(isDigit(peek())){
            advance();
        }
    }
    return makeToken(TOKEN_NUMBER);
}

static Token identifier(){
    while(isAlpha(peek()) || isDigit(peek())){
        advance();
    }
    return makeToken(identifierType());
}

void initScanner(const char* source){
    const char* keywords[] = {
        "and", "class", "else", "false", "for", "fun", "if", "nil",
        "or", "print", "return", "super", "this", "true", "var", "while"
    };

    TokenType tokens[] = {
        TOKEN_AND, TOKEN_CLASS, TOKEN_ELSE, TOKEN_FALSE, TOKEN_FOR, TOKEN_FUN,
        TOKEN_IF, TOKEN_NIL, TOKEN_OR, TOKEN_PRINT, TOKEN_RETURN, TOKEN_SUPER,
        TOKEN_THIS, TOKEN_TRUE, TOKEN_VAR, TOKEN_WHILE
    };
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
    scanner.keyword_trie = createKeywordTrie(keywords, tokens, 16);
}

Token scanToken(){
    skipWhitespace();
    scanner.start = scanner.current;
    if(isAtEnd()){
        return makeToken(TOKEN_EOF);
    }
    char c = advance();
    if (isAlpha(c)){
        return identifier();
    }
    if (isDigit(c)){
        return number();
    }
    switch(c){
        case '(':
            return makeToken(TOKEN_LEFT_PAREN);
        case ')':
            return makeToken(TOKEN_RIGHT_PAREN);
        case '{':
            return makeToken(TOKEN_LEFT_BRACE);
        case '}':
            return makeToken(TOKEN_RIGHT_BRACE);
        case ';':
            return makeToken(TOKEN_SEMICOLON);
        case ',':
            return makeToken(TOKEN_COMMA);
        case '.':
            return makeToken(TOKEN_DOT);
        case '-':
            return makeToken(TOKEN_MINUS);
        case '+':
            return makeToken(TOKEN_PLUS);
        case '/':
            return makeToken(TOKEN_SLASH);
        case '*':
            return makeToken(TOKEN_STAR);
        case '!':
            return makeToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
        case '=':
            return makeToken(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '<':
            return makeToken(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '>':
            return makeToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
        case '"':
            return string();
    }
    return errorToken("Unexpected Character.");
}