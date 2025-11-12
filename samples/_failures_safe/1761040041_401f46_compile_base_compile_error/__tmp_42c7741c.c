//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: parser_tokenizer
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

int parse_number(struct Tokenizer *t) {
    int start = t->position;
    while (isdigit(t->input[t->position])) {
        t->position++;
        t->column++;
    }
    int len = t->position - start;
    if (len > 0 && len < MAX_TOKEN_LEN && t->token_count < MAX_TOKENS) {
        struct Token *token = &t->tokens[t->token_count];
        token->type = TOKEN_NUMBER;
        strncpy(token->value, t->input + start, len);
        token->value[len] = '\0';
        token->line = t->line;
        token->column = t->column - len;
        t->token_count++;
        return 1;
    }
    return 0;
}

int parse_identifier(struct Tokenizer *t) {
    int start = t->position;
    if (isalpha(t->input[t->position]) || t->input[t->position] == '_') {
        t->position++;
        t->column++;
        while (isalnum(t->input[t->position]) || t->input[t->position] == '_') {
            t->position++;
            t->column++;
        }
        int len = t->position - start;
        if (len > 0 && len < MAX_TOKEN_LEN && t->token_count < MAX_TOKENS) {
            struct Token *token = &t->tokens[t->token_count];
            token->type = TOKEN_IDENTIFIER;
            strncpy(token->value, t->input + start, len);
            token->value[len] = '\0';
            token->line = t->line;
            token->column = t->column - len;
            t->token_count++;
            return 1;
        }
    }
    return 0;
}

int parse_operator(struct Tokenizer *t) {
    if (is_operator_char(t->input[t->position])) {
        if (t->token_count < MAX_TOKENS) {
            struct Token *token = &t->tokens[t->token_count];
            token->type = TOKEN_OPERATOR;
            token->value[0] = t->input[t->position];
            token->value[1] = '\0';
            token->line = t->line;
            token->column = t->column;
            t->position++;
            t->column++;
            t->token_count++;
            return 1;
        }
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
            int len = t->position - start - 1;
            if (len >= 0 && len < MAX_TOKEN_LEN && t->token_count < MAX_TOKENS) {
                struct Token *token = &t->tokens[t->token_count];
                token->type = TOKEN_STRING;
                strncpy(token->value, t->input + start + 1, len);
                token->value[len] = '\0';
                token->line = t->line;
                token->column = t->column - len - 1;
                t->position++;
                t->column++;
                t->token_count++;
                return 1;
            }
        }
    }
    return 0;
}

void parse_unknown(struct Tokenizer *t) {
    if (t->token_count < MAX_TOKENS) {
        struct Token *token = &t->tokens[t->token_count];
        token