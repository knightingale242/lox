#include <stdio.h>
#include <string.h>

#include "common.h"
#include "scanner.h"
#include "trie.h"

typedef struct{
    const char* start;
    const char* current;
    int line;
    Trie* keyword_trie;
} Scanner;

Scanner scanner;

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

static TokenType identifierType(){
    TokenType token = test(scanner.keyword_trie, scanner.current);

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
    makeToken(TOKEN_STRING);
}

static Token number(){
    while (isDigit(peek())){
        advance();
    }

    if(peek == '.' && isDigit(peekNext())){
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
    scanner.keyword_trie = createKeywordTrie(&keywords, &tokens, 16);
}

Token scanToken(){
    skipWhitespace();
    scanner.start = scanner.current;

    if(isAtEnd()){
        return makeToken(TOKEN_EOF);

    char c = advance();
    if (isAlpha(c)){
        return identifier();
    }
    if (isDigit(peek())){
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