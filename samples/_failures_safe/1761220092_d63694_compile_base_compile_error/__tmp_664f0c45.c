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
    const char *input;
    int position;
    int line;
    int column;
    Token tokens[MAX_TOKENS];
    int token_count;
} Lexer;

typedef void (*TokenHandler)(const Token*);

void handle_number(const Token *token) {
    printf("Number: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void handle_identifier(const Token *token) {
    printf("Identifier: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void handle_operator(const Token *token) {
    printf("Operator: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void handle_string(const Token *token) {
    printf("String: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void handle_error(const Token *token) {
    printf("Error: Invalid token '%s' at line %d, column %d\n", token->value, token->line, token->column);
}

void skip_whitespace(Lexer *lexer) {
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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

Token get_next_token(Lexer *lexer) {
    Token token = {TOKEN_ERROR, "", lexer->line, lexer->column};
    
    skip_whitespace(lexer);
    
    if (lexer->input[lexer->position] == '\0') {
        token.type = TOKEN_EOF;
        strcpy(token.value, "EOF");
        return token;
    }
    
    char current = lexer->input[lexer->position];
    
    if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (lexer->input[lexer->position] != '\0' && isdigit(lexer->input[lexer->position]) && i < MAX_TOKEN_LEN - 1) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
    } else if (isalpha(current)) {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (lexer->input[lexer->position] != '\0' && (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') && i < MAX_TOKEN_LEN - 1) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
    } else if (is_operator(current)) {
        token.type = TOKEN_OPERATOR;
        token.value[0] = lexer->input[lexer->position++];
        token.value[1] = '\0';
        lexer->column++;
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        int i = 0;
        lexer->position++;
        lexer->column++;
        while (lexer->input[lexer->position] != '\0' && lexer->input[lexer->position] != '"' && i < MAX_TOKEN_LEN - 1) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
        if (lexer->input[lexer->position] == '"') {
            lexer->position++;
            lexer->column++;
        }
    } else {
        token.type = TOKEN_ERROR;
        token.value[0] = lexer->input[lexer->position++];
        token.value[1] = '\0';
        lexer->column++;
    }
    
    return token;
}

int tokenize(Lexer *lexer, const char *input) {
    if (input == NULL) {
        return 0;
    }
    
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->token_count = 0;
    
    while (lexer->token_count < MAX_TOKENS) {
        Token token = get_next_token(lexer);
        lexer->tokens[lexer->token_count++] = token;
        
        if (