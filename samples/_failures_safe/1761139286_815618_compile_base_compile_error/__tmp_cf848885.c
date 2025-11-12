//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_ERROR
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
    int line;
    int column;
} Token;

typedef struct {
    const char* input;
    int position;
    int line;
    int column;
    Token tokens[MAX_TOKENS];
    int token_count;
} Lexer;

typedef void (*TokenHandler)(Token*);

void handle_number(Token* token) {
    printf("Number: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void handle_identifier(Token* token) {
    printf("Identifier: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void handle_operator(Token* token) {
    printf("Operator: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void handle_string(Token* token) {
    printf("String: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void handle_error(Token* token) {
    printf("Error: Invalid token '%s' at line %d, column %d\n", token->value, token->line, token->column);
}

TokenHandler get_handler(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return handle_number;
        case TOKEN_IDENTIFIER: return handle_identifier;
        case TOKEN_OPERATOR: return handle_operator;
        case TOKEN_STRING: return handle_string;
        case TOKEN_ERROR: return handle_error;
        default: return NULL;
    }
}

void skip_whitespace(Lexer* lexer) {
    while (lexer->input[lexer->position] != '\0' && isspace(lexer->input[lexer->position])) {
        if (lexer->input[lexer->position] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
    }
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

Token lex_number(Lexer* lexer) {
    Token token = {TOKEN_NUMBER, "", lexer->line, lexer->column};
    int i = 0;
    
    while (lexer->input[lexer->position] != '\0' && isdigit(lexer->input[lexer->position])) {
        if (i < MAX_TOKEN_LEN - 1) {
            token.value[i++] = lexer->input[lexer->position];
        }
        lexer->position++;
        lexer->column++;
    }
    token.value[i] = '\0';
    return token;
}

Token lex_identifier(Lexer* lexer) {
    Token token = {TOKEN_IDENTIFIER, "", lexer->line, lexer->column};
    int i = 0;
    
    while (lexer->input[lexer->position] != '\0' && 
           (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_')) {
        if (i < MAX_TOKEN_LEN - 1) {
            token.value[i++] = lexer->input[lexer->position];
        }
        lexer->position++;
        lexer->column++;
    }
    token.value[i] = '\0';
    return token;
}

Token lex_operator(Lexer* lexer) {
    Token token = {TOKEN_OPERATOR, "", lexer->line, lexer->column};
    int i = 0;
    
    while (lexer->input[lexer->position] != '\0' && is_operator(lexer->input[lexer->position])) {
        if (i < MAX_TOKEN_LEN - 1) {
            token.value[i++] = lexer->input[lexer->position];
        }
        lexer->position++;
        lexer->column++;
    }
    token.value[i] = '\0';
    return token;
}

Token lex_string(Lexer* lexer) {
    Token token = {TOKEN_STRING, "", lexer->line, lexer->column};
    int i = 0;
    
    if (lexer->input[lexer->position] == '"') {
        lexer->position++;
        lexer->column++;
        
        while (lexer->input[lexer->position] != '\0' && lexer->input[lexer->position] != '"') {
            if (i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = lexer->input[lexer->position];
            }
            lexer->position++;
            lexer->column++;
        }
        
        if (lexer->input[lexer->position] == '"') {
            lexer->position++;
            lexer->column++;
        }
    }
    token.value[i] = '\0';
    return token