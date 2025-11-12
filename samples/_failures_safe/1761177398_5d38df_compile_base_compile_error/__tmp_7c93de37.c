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
    TOKEN_PUNCTUATION,
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

struct Parser {
    struct Lexer *lexer;
    int current_token;
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

int is_punctuation(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == ';' || c == ',' || c == '.';
}

void add_token(struct Lexer *lexer, enum TokenType type, char *value) {
    if (lexer == NULL || value == NULL || lexer->token_count >= MAX_TOKENS) return;
    struct Token *token = &lexer->tokens[lexer->token_count];
    token->type = type;
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
    token->line = lexer->line;
    token->column = lexer->column;
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

int lex_number(struct Lexer *lexer) {
    if (lexer == NULL) return 0;
    int start = lexer->position;
    while (lexer->input[lexer->position] != '\0' && isdigit(lexer->input[lexer->position])) {
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

int lex_identifier(struct Lexer *lexer) {
    if (lexer == NULL) return 0;
    int start = lexer->position;
    while (lexer->input[lexer->position] != '\0' && (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_')) {
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

int lex_string(struct Lexer *lexer) {
    if (lexer == NULL || lexer->input[lexer->position] != '"') return 0;
    int start = lexer->position;
    lexer->position++;
    lexer->column++;
    while (lexer->input[lexer->position] != '\0' && lexer->input[lexer->position] != '"') {
        if (lexer->input[lexer->position] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
    }
    if (lexer->input[lexer->position] == '"') {
        lexer->position++;
        lexer->column++;
        char str[MAX_TOKEN_LEN];
        int len = lexer->position - start - 2;
        if (len < 0) len = 0;
        if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
        strncpy(str, &