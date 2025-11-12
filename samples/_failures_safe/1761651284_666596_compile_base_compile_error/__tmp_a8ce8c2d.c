//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64
#define MAX_INPUT_LEN 1024

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_PAREN,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_UNKNOWN
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
    int line;
    int column;
};

struct Lexer {
    char *input;
    int position;
    int line;
    int column;
    struct Token tokens[MAX_TOKENS];
    int token_count;
};

void init_lexer(struct Lexer *lexer, char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->token_count = 0;
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void add_token(struct Lexer *lexer, enum TokenType type, char *value) {
    if (lexer == NULL || value == NULL || lexer->token_count >= MAX_TOKENS) return;
    struct Token *token = &lexer->tokens[lexer->token_count];
    token->type = type;
    token->line = lexer->line;
    token->column = lexer->column;
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
    lexer->token_count++;
}

void skip_whitespace(struct Lexer *lexer) {
    if (lexer == NULL) return;
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

int scan_number(struct Lexer *lexer) {
    if (lexer == NULL) return 0;
    int start = lexer->position;
    while (isdigit(lexer->input[lexer->position])) {
        lexer->position++;
        lexer->column++;
    }
    if (lexer->position > start) {
        char num[MAX_TOKEN_LEN];
        int len = lexer->position - start;
        if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
        strncpy(num, &lexer->input[start], len);
        num[len] = '\0';
        add_token(lexer, TOKEN_NUMBER, num);
        return 1;
    }
    return 0;
}

int scan_identifier(struct Lexer *lexer) {
    if (lexer == NULL) return 0;
    int start = lexer->position;
    while (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') {
        lexer->position++;
        lexer->column++;
    }
    if (lexer->position > start) {
        char ident[MAX_TOKEN_LEN];
        int len = lexer->position - start;
        if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
        strncpy(ident, &lexer->input[start], len);
        ident[len] = '\0';
        add_token(lexer, TOKEN_IDENTIFIER, ident);
        return 1;
    }
    return 0;
}

int scan_operator(struct Lexer *lexer) {
    if (lexer == NULL) return 0;
    if (is_operator(lexer->input[lexer->position])) {
        char op[2] = {lexer->input[lexer->position], '\0'};
        add_token(lexer, TOKEN_OPERATOR, op);
        lexer->position++;
        lexer->column++;
        return 1;
    }
    return 0;
}

int scan_paren(struct Lexer *lexer) {
    if (lexer == NULL) return 0;
    if (lexer->input[lexer->position] == '(' || lexer->input[lexer->position] == ')') {
        char paren[2] = {lexer->input[lexer->position], '\0'};
        add_token(lexer, TOKEN_PAREN, paren);
        lexer->position++;
        lexer->column++;
        return 1;
    }
    return 0;
}

int scan_string(struct Lexer *lexer) {
    if (lexer == NULL) return 0;
    if (lexer->input[lexer->position] == '"') {
        int start = lexer->position;
        lexer->position++;
        lexer->column++;
        while (lexer->input[lexer->position] != '"' && lexer->input[lexer->position] != '\0') {
            if (lexer->input[lexer->position] == '\n') {