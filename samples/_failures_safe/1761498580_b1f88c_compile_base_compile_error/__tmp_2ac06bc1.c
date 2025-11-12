//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
    int line;
    int column;
};

struct Tokenizer {
    const char *input;
    int position;
    int line;
    int column;
    struct Token tokens[MAX_TOKENS];
    int token_count;
};

void tokenizer_init(struct Tokenizer *t, const char *input) {
    t->input = input;
    t->position = 0;
    t->line = 1;
    t->column = 1;
    t->token_count = 0;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(struct Tokenizer *t) {
    while (t->input[t->position] != '\0') {
        char c = t->input[t->position];
        if (c == ' ' || c == '\t') {
            t->position++;
            t->column++;
        } else if (c == '\n') {
            t->position++;
            t->line++;
            t->column = 1;
        } else {
            break;
        }
    }
}

struct Token make_token(enum TokenType type, const char *value, int line, int column) {
    struct Token token;
    token.type = type;
    token.line = line;
    token.column = column;
    strncpy(token.value, value, MAX_TOKEN_LEN - 1);
    token.value[MAX_TOKEN_LEN - 1] = '\0';
    return token;
}

int add_token(struct Tokenizer *t, struct Token token) {
    if (t->token_count >= MAX_TOKENS) {
        return 0;
    }
    t->tokens[t->token_count] = token;
    t->token_count++;
    return 1;
}

int parse_number(struct Tokenizer *t) {
    int start = t->position;
    int has_dot = 0;
    
    while (t->input[t->position] != '\0') {
        char c = t->input[t->position];
        if (isdigit(c)) {
            t->position++;
            t->column++;
        } else if (c == '.' && !has_dot) {
            has_dot = 1;
            t->position++;
            t->column++;
        } else {
            break;
        }
    }
    
    if (t->position - start > 0) {
        char num[MAX_TOKEN_LEN];
        int len = t->position - start;
        if (len >= MAX_TOKEN_LEN) {
            len = MAX_TOKEN_LEN - 1;
        }
        strncpy(num, &t->input[start], len);
        num[len] = '\0';
        struct Token token = make_token(TOKEN_NUMBER, num, t->line, t->column - len);
        return add_token(t, token);
    }
    return 0;
}

int parse_identifier(struct Tokenizer *t) {
    int start = t->position;
    
    if (t->input[t->position] != '\0' && (isalpha(t->input[t->position]) || t->input[t->position] == '_')) {
        t->position++;
        t->column++;
        
        while (t->input[t->position] != '\0' && (isalnum(t->input[t->position]) || t->input[t->position] == '_')) {
            t->position++;
            t->column++;
        }
        
        int len = t->position - start;
        if (len >= MAX_TOKEN_LEN) {
            len = MAX_TOKEN_LEN - 1;
        }
        char ident[MAX_TOKEN_LEN];
        strncpy(ident, &t->input[start], len);
        ident[len] = '\0';
        struct Token token = make_token(TOKEN_IDENTIFIER, ident, t->line, t->column - len);
        return add_token(t, token);
    }
    return 0;
}

int parse_operator(struct Tokenizer *t) {
    if (t->input[t->position] != '\0' && is_operator_char(t->input[t->position])) {
        char op[2] = {t->input[t->position], '\0'};
        struct Token token = make_token(TOKEN_OPERATOR, op, t->line, t->column);
        t->position++;
        t->column++;
        return add_token(t, token);
    }
    return 0;
}

int parse_string(struct Tokenizer *t) {
    if (t->input[t->position] == '"') {
        int start = t->position;
        t->position++;
        t->column++;
        
        while (t->input[t->position] != '\0' && t->input[t->position] != '"') {
            if (t->input[t->position] == '\n') {
                t->line++;
                t->column = 1;
            } else {
                t->column++;
            }
            t->position++;
        }
        
        if (t->input[t->position] == '"') {
            int len = t->position - start - 1