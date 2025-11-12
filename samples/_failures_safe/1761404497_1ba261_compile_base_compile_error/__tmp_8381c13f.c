//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 1000
#define MAX_TOKEN_LEN 256

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
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

void init_lexer(Lexer *lexer, const char *input) {
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->token_count = 0;
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int add_token(Lexer *lexer, TokenType type, const char *value) {
    if (lexer->token_count >= MAX_TOKENS) {
        return 0;
    }
    Token *token = &lexer->tokens[lexer->token_count];
    token->type = type;
    token->line = lexer->line;
    token->column = lexer->column;
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
    lexer->token_count++;
    return 1;
}

void skip_whitespace(Lexer *lexer) {
    while (lexer->input[lexer->position] != '\0') {
        char c = lexer->input[lexer->position];
        if (c == ' ' || c == '\t') {
            lexer->position++;
            lexer->column++;
        } else if (c == '\n') {
            lexer->position++;
            lexer->line++;
            lexer->column = 1;
        } else {
            break;
        }
    }
}

int read_identifier(Lexer *lexer) {
    int start = lexer->position;
    while (isalnum((unsigned char)lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') {
        lexer->position++;
        lexer->column++;
    }
    int len = lexer->position - start;
    if (len >= MAX_TOKEN_LEN) {
        return 0;
    }
    char value[MAX_TOKEN_LEN];
    strncpy(value, &lexer->input[start], len);
    value[len] = '\0';
    return add_token(lexer, TOKEN_IDENTIFIER, value);
}

int read_number(Lexer *lexer) {
    int start = lexer->position;
    while (isdigit((unsigned char)lexer->input[lexer->position])) {
        lexer->position++;
        lexer->column++;
    }
    int len = lexer->position - start;
    if (len >= MAX_TOKEN_LEN) {
        return 0;
    }
    char value[MAX_TOKEN_LEN];
    strncpy(value, &lexer->input[start], len);
    value[len] = '\0';
    return add_token(lexer, TOKEN_NUMBER, value);
}

int read_operator(Lexer *lexer) {
    int start = lexer->position;
    while (is_operator_char(lexer->input[lexer->position])) {
        lexer->position++;
        lexer->column++;
    }
    int len = lexer->position - start;
    if (len >= MAX_TOKEN_LEN) {
        return 0;
    }
    char value[MAX_TOKEN_LEN];
    strncpy(value, &lexer->input[start], len);
    value[len] = '\0';
    return add_token(lexer, TOKEN_OPERATOR, value);
}

int read_string(Lexer *lexer) {
    if (lexer->input[lexer->position] != '"') {
        return 0;
    }
    lexer->position++;
    lexer->column++;
    int start = lexer->position;
    while (lexer->input[lexer->position] != '\0' && lexer->input[lexer->position] != '"') {
        lexer->position++;
        lexer->column++;
    }
    if (lexer->input[lexer->position] != '"') {
        return 0;
    }
    int len = lexer->position - start;
    if (len >= MAX_TOKEN_LEN) {
        return 0;
    }
    char value[MAX_TOKEN_LEN];
    strncpy(value, &lexer->input[start], len);
    value[len] = '\0';
    lexer->position++;
    lexer->column++;
    return add_token(lexer, TOKEN_STRING, value);
}

int tokenize(Lexer *lexer) {
    while (lexer->input[lexer->position] != '\0') {
        skip_whitespace(lexer);
        char c = lexer->input[lexer->position];
        if (c == '\0') {
            break;
        }
        if (isalpha((unsigned char)c) || c == '_') {
            if (!read_identifier(lexer))