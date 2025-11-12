//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[32];
    int line;
    int column;
} Token;

typedef struct {
    const char *input;
    int position;
    int line;
    int column;
    Token current;
} Lexer;

typedef void (*TokenHandler)(const Token*);

void handle_number(const Token *token) {
    if (token == NULL) return;
    printf("Number: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void handle_identifier(const Token *token) {
    if (token == NULL) return;
    printf("Identifier: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void handle_operator(const Token *token) {
    if (token == NULL) return;
    printf("Operator: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void handle_string(const Token *token) {
    if (token == NULL) return;
    printf("String: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void handle_unknown(const Token *token) {
    if (token == NULL) return;
    printf("Unknown: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void skip_whitespace(Lexer *lexer) {
    if (lexer == NULL || lexer->input == NULL) return;
    
    while (lexer->input[lexer->position] != '\0' && isspace((unsigned char)lexer->input[lexer->position])) {
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

Token get_next_token(Lexer *lexer) {
    Token token = {TOKEN_UNKNOWN, "", lexer->line, lexer->column};
    
    if (lexer == NULL || lexer->input == NULL) {
        return token;
    }
    
    skip_whitespace(lexer);
    
    if (lexer->input[lexer->position] == '\0') {
        token.type = TOKEN_EOF;
        strncpy(token.value, "EOF", sizeof(token.value) - 1);
        token.value[sizeof(token.value) - 1] = '\0';
        return token;
    }
    
    char current = lexer->input[lexer->position];
    
    if (isdigit((unsigned char)current)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (lexer->input[lexer->position] != '\0' && isdigit((unsigned char)lexer->input[lexer->position]) && i < (int)sizeof(token.value) - 1) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
    } else if (isalpha((unsigned char)current)) {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (lexer->input[lexer->position] != '\0' && (isalnum((unsigned char)lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') && i < (int)sizeof(token.value) - 1) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
    } else if (is_operator(current)) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (lexer->input[lexer->position] != '\0' && is_operator(lexer->input[lexer->position]) && i < (int)sizeof(token.value) - 1) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        int i = 0;
        lexer->position++;
        lexer->column++;
        while (lexer->input[lexer->position] != '\0' && lexer->input[lexer->position] != '"' && i < (int)sizeof(token.value) - 1) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
        if (lexer->input[lexer->position] == '"') {
            lexer->position++;
            lexer->column++;
        }
    } else {
        token.type =