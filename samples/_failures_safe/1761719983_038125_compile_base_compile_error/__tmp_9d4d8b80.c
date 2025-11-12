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

void add_token(struct Tokenizer *t, enum TokenType type, const char *value) {
    if (t->token_count >= MAX_TOKENS) {
        return;
    }
    struct Token *token = &t->tokens[t->token_count];
    token->type = type;
    token->line = t->line;
    token->column = t->column;
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
    t->token_count++;
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

void read_number(struct Tokenizer *t) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    while (t->input[t->position] != '\0' && i < MAX_TOKEN_LEN - 1) {
        char c = t->input[t->position];
        if (isdigit(c) || c == '.') {
            buffer[i++] = c;
            t->position++;
            t->column++;
        } else {
            break;
        }
    }
    buffer[i] = '\0';
    add_token(t, TOKEN_NUMBER, buffer);
}

void read_identifier(struct Tokenizer *t) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    while (t->input[t->position] != '\0' && i < MAX_TOKEN_LEN - 1) {
        char c = t->input[t->position];
        if (isalnum(c) || c == '_') {
            buffer[i++] = c;
            t->position++;
            t->column++;
        } else {
            break;
        }
    }
    buffer[i] = '\0';
    add_token(t, TOKEN_IDENTIFIER, buffer);
}

void read_operator(struct Tokenizer *t) {
    char buffer[3] = {0};
    buffer[0] = t->input[t->position];
    t->position++;
    t->column++;
    if (t->input[t->position] != '\0' && is_operator_char(t->input[t->position])) {
        buffer[1] = t->input[t->position];
        t->position++;
        t->column++;
    }
    add_token(t, TOKEN_OPERATOR, buffer);
}

void read_string(struct Tokenizer *t) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    t->position++;
    t->column++;
    while (t->input[t->position] != '\0' && i < MAX_TOKEN_LEN - 1) {
        char c = t->input[t->position];
        if (c == '"') {
            t->position++;
            t->column++;
            break;
        }
        buffer[i++] = c;
        t->position++;
        t->column++;
    }
    buffer[i] = '\0';
    add_token(t, TOKEN_STRING, buffer);
}

int tokenize(struct Tokenizer *t) {
    while (t->input[t->position] != '\0' && t->token_count < MAX_TOKENS) {
        skip_whitespace(t);
        char c = t->input[t->position];
        if (c == '\0') break;
        
        if (isdigit(c)) {
            read_number(t);
        } else if (isalpha(c) || c == '_') {
            read_identifier(t);
        } else if (is_operator_char(c)) {
            read_operator(t);
        } else if (c == '"') {
            read_string(t);
        } else {
            char unknown[2] = {c, '\0'};
            add_token(t, TOKEN_UNKNOWN, unknown);
            t->position++;
            t->column++;
        }
    }
    add_token(t, TOKEN_EOF, "");
    return t->token_count;
}

void print_tokens(struct Tokenizer *t) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF",