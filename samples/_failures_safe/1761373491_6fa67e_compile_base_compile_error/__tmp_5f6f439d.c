//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_ERROR
};

struct Token {
    enum TokenType type;
    char value[32];
    int line;
    int column;
};

struct Lexer {
    const char *input;
    int position;
    int line;
    int column;
    int length;
};

void lexer_init(struct Lexer *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->length = strlen(input);
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || 
           c == '=' || c == '<' || c == '>' || c == '!';
}

struct Token lexer_next(struct Lexer *lexer) {
    struct Token token = {TOKEN_ERROR, "", lexer->line, lexer->column};
    
    if (lexer == NULL || lexer->input == NULL) {
        return token;
    }
    
    while (lexer->position < lexer->length) {
        char current = lexer->input[lexer->position];
        
        if (isspace(current)) {
            if (current == '\n') {
                lexer->line++;
                lexer->column = 1;
            } else {
                lexer->column++;
            }
            lexer->position++;
            continue;
        }
        
        token.line = lexer->line;
        token.column = lexer->column;
        
        if (isdigit(current)) {
            token.type = TOKEN_NUMBER;
            int i = 0;
            while (lexer->position < lexer->length && 
                   i < 31 && 
                   isdigit(lexer->input[lexer->position])) {
                token.value[i++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            token.value[i] = '\0';
            return token;
        }
        
        if (isalpha(current) || current == '_') {
            token.type = TOKEN_IDENTIFIER;
            int i = 0;
            while (lexer->position < lexer->length && 
                   i < 31 && 
                   (isalnum(lexer->input[lexer->position]) || 
                    lexer->input[lexer->position] == '_')) {
                token.value[i++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            token.value[i] = '\0';
            return token;
        }
        
        if (is_operator_char(current)) {
            token.type = TOKEN_OPERATOR;
            int i = 0;
            while (lexer->position < lexer->length && 
                   i < 31 && 
                   is_operator_char(lexer->input[lexer->position])) {
                token.value[i++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            token.value[i] = '\0';
            return token;
        }
        
        if (current == '"') {
            token.type = TOKEN_STRING;
            int i = 0;
            lexer->position++;
            lexer->column++;
            
            while (lexer->position < lexer->length && 
                   i < 31 && 
                   lexer->input[lexer->position] != '"') {
                token.value[i++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            
            if (lexer->position < lexer->length && 
                lexer->input[lexer->position] == '"') {
                lexer->position++;
                lexer->column++;
            }
            token.value[i] = '\0';
            return token;
        }
        
        lexer->position++;
        lexer->column++;
    }
    
    token.type = TOKEN_EOF;
    strcpy(token.value, "");
    return token;
}

void print_token(struct Token token) {
    switch (token.type) {
        case TOKEN_NUMBER:
            printf("NUMBER: %s (line %d, column %d)\n", 
                   token.value, token.line, token.column);
            break;
        case TOKEN_IDENTIFIER:
            printf("IDENTIFIER: %s (line %d, column %d)\n", 
                   token.value, token.line, token.column);
            break;
        case TOKEN_OPERATOR:
            printf("OPERATOR: %s (line %d, column %d)\n", 
                   token.value, token.line, token.column);
            break;
        case TOKEN_STRING:
            printf("STRING: \"%s\" (line %d, column %d)\n", 
                   token.value, token.line, token.column);
            break;
        case TOKEN_EOF:
            printf("EOF (line %d, column %d)\n", token.line, token.column);
            break;
        case TOKEN_ERROR:
            printf("ERROR (line %d, column %d)\n", token.line, token.column);
            break;
    }
}

int main() {
    char input[1024];
    
    printf("Enter text to tokenize