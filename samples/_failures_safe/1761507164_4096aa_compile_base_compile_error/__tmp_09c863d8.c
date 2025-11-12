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
    TOKEN_UNKNOWN
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

int read_number(struct Tokenizer *t, struct Token *token) {
    int start = t->position;
    while (isdigit(t->input[t->position])) {
        t->position++;
        t->column++;
    }
    int length = t->position - start;
    if (length >= MAX_TOKEN_LEN) {
        return 0;
    }
    strncpy(token->value, &t->input[start], length);
    token->value[length] = '\0';
    token->type = TOKEN_NUMBER;
    token->line = t->line;
    token->column = t->column - length;
    return 1;
}

int read_identifier(struct Tokenizer *t, struct Token *token) {
    int start = t->position;
    while (isalnum(t->input[t->position]) || t->input[t->position] == '_') {
        t->position++;
        t->column++;
    }
    int length = t->position - start;
    if (length >= MAX_TOKEN_LEN) {
        return 0;
    }
    strncpy(token->value, &t->input[start], length);
    token->value[length] = '\0';
    token->type = TOKEN_IDENTIFIER;
    token->line = t->line;
    token->column = t->column - length;
    return 1;
}

int read_operator(struct Tokenizer *t, struct Token *token) {
    int start = t->position;
    while (is_operator_char(t->input[t->position])) {
        t->position++;
        t->column++;
    }
    int length = t->position - start;
    if (length >= MAX_TOKEN_LEN) {
        return 0;
    }
    strncpy(token->value, &t->input[start], length);
    token->value[length] = '\0';
    token->type = TOKEN_OPERATOR;
    token->line = t->line;
    token->column = t->column - length;
    return 1;
}

int read_string(struct Tokenizer *t, struct Token *token) {
    if (t->input[t->position] != '"') {
        return 0;
    }
    t->position++;
    t->column++;
    int start = t->position;
    while (t->input[t->position] != '\0' && t->input[t->position] != '"') {
        if (t->input[t->position] == '\n') {
            return 0;
        }
        t->position++;
        t->column++;
    }
    if (t->input[t->position] != '"') {
        return 0;
    }
    int length = t->position - start;
    if (length >= MAX_TOKEN_LEN) {
        return 0;
    }
    strncpy(token->value, &t->input[start], length);
    token->value[length] = '\0';
    t->position++;
    t->column++;
    token->type = TOKEN_STRING;
    token->line = t->line;
    token->column = t->column - length - 2;
    return 1;
}

int tokenize_next(struct Tokenizer *t) {
    skip_whitespace(t);
    if (t->input[t->position] == '\0') {
        if (t->token_count >= MAX_TOKENS) {
            return 0;
        }
        struct Token *token = &t->tokens[t->token_count++];
        strcpy(token->value, "");
        token->type = TOKEN_EOF;
        token->line = t->line;
        token->column = t->column;
        return 1;
    }
    if (t->token_count >= MAX_TOKENS) {
        return 0;
    }
    struct Token *token = &t->tokens[t->token_count];
    char c = t->input[t->position];
    if (isdigit(c)) {
        if (!read_number(t, token