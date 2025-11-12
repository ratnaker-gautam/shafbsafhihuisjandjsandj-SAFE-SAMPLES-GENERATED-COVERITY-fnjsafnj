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
    int len = 0;
    while (isdigit(t->input[t->position]) && len < MAX_TOKEN_LEN - 1) {
        token->value[len++] = t->input[t->position++];
        t->column++;
    }
    token->value[len] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int read_identifier(struct Tokenizer *t, struct Token *token) {
    int len = 0;
    while (isalnum(t->input[t->position]) && len < MAX_TOKEN_LEN - 1) {
        token->value[len++] = t->input[t->position++];
        t->column++;
    }
    token->value[len] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int read_operator(struct Tokenizer *t, struct Token *token) {
    int len = 0;
    while (is_operator_char(t->input[t->position]) && len < MAX_TOKEN_LEN - 1) {
        token->value[len++] = t->input[t->position++];
        t->column++;
    }
    token->value[len] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

int read_string(struct Tokenizer *t, struct Token *token) {
    if (t->input[t->position] != '"') return 0;
    
    t->position++;
    t->column++;
    int len = 0;
    
    while (t->input[t->position] != '"' && t->input[t->position] != '\0' && len < MAX_TOKEN_LEN - 1) {
        token->value[len++] = t->input[t->position++];
        t->column++;
    }
    
    if (t->input[t->position] == '"') {
        t->position++;
        t->column++;
        token->value[len] = '\0';
        token->type = TOKEN_STRING;
        return 1;
    }
    
    return 0;
}

int tokenize_next(struct Tokenizer *t) {
    if (t->token_count >= MAX_TOKENS) return 0;
    
    skip_whitespace(t);
    
    if (t->input[t->position] == '\0') {
        struct Token *token = &t->tokens[t->token_count++];
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        token->line = t->line;
        token->column = t->column;
        return 1;
    }
    
    struct Token *token = &t->tokens[t->token_count];
    token->line = t->line;
    token->column = t->column;
    
    char c = t->input[t->position];
    
    if (isdigit(c)) {
        if (read_number(t, token)) {
            t->token_count++;
            return 1;
        }
    } else if (isalpha(c)) {
        if (read_identifier(t, token)) {
            t->token_count++;
            return 1;
        }
    } else if (is_operator_char(c)) {
        if (read_operator(t, token)) {
            t->token_count++;
            return 1;
        }
    } else if (c == '"') {
        if (read_string(t, token)) {
            t->token_count++;
            return 1;
        }
    } else {
        token->value[0] = c;
        token->value[1] = '\0';
        token->type = TOKEN_UNKNOWN;
        t->position++;
        t->column++;
        t->token_count++;
        return 1;
    }
    
    return 0;
}

void tokenize_all(struct Tokenizer *t) {
    while (tokenize_next(t) && t->tokens[t->token_count - 1].