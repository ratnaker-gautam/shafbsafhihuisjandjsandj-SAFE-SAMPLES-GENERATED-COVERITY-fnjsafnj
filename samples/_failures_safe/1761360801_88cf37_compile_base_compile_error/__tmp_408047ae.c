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
    char *input;
    int position;
    int line;
    int column;
    struct Token tokens[MAX_TOKENS];
    int token_count;
};

void tokenizer_init(struct Tokenizer *t, char *input) {
    if (t == NULL || input == NULL) return;
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
    if (t == NULL) return;
    while (t->input[t->position] != '\0' && isspace(t->input[t->position])) {
        if (t->input[t->position] == '\n') {
            t->line++;
            t->column = 1;
        } else {
            t->column++;
        }
        t->position++;
    }
}

int read_number(struct Tokenizer *t, struct Token *token) {
    if (t == NULL || token == NULL) return 0;
    int len = 0;
    
    while (t->input[t->position] != '\0' && isdigit(t->input[t->position])) {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        token->value[len++] = t->input[t->position];
        t->position++;
        t->column++;
    }
    
    if (len == 0) return 0;
    
    token->value[len] = '\0';
    token->type = TOKEN_NUMBER;
    token->line = t->line;
    token->column = t->column - len;
    return 1;
}

int read_identifier(struct Tokenizer *t, struct Token *token) {
    if (t == NULL || token == NULL) return 0;
    int len = 0;
    
    if (t->input[t->position] != '\0' && isalpha(t->input[t->position])) {
        token->value[len++] = t->input[t->position];
        t->position++;
        t->column++;
        
        while (t->input[t->position] != '\0' && (isalnum(t->input[t->position]) || t->input[t->position] == '_')) {
            if (len >= MAX_TOKEN_LEN - 1) return 0;
            token->value[len++] = t->input[t->position];
            t->position++;
            t->column++;
        }
    }
    
    if (len == 0) return 0;
    
    token->value[len] = '\0';
    token->type = TOKEN_IDENTIFIER;
    token->line = t->line;
    token->column = t->column - len;
    return 1;
}

int read_operator(struct Tokenizer *t, struct Token *token) {
    if (t == NULL || token == NULL) return 0;
    int len = 0;
    
    if (t->input[t->position] != '\0' && is_operator_char(t->input[t->position])) {
        token->value[len++] = t->input[t->position];
        t->position++;
        t->column++;
        
        if (t->input[t->position] != '\0' && is_operator_char(t->input[t->position])) {
            if (len >= MAX_TOKEN_LEN - 1) return 0;
            token->value[len++] = t->input[t->position];
            t->position++;
            t->column++;
        }
    }
    
    if (len == 0) return 0;
    
    token->value[len] = '\0';
    token->type = TOKEN_OPERATOR;
    token->line = t->line;
    token->column = t->column - len;
    return 1;
}

int read_paren(struct Tokenizer *t, struct Token *token) {
    if (t == NULL || token == NULL) return 0;
    
    if (t->input[t->position] == '(' || t->input[t->position] == ')') {
        token->value[0] = t->input[t->position];
        token->value[1] = '\0';
        token->type = TOKEN_PAREN;
        token->line = t->line;
        token->column = t->column;
        t->position++;
        t->column++;
        return 1;
    }
    
    return 0;
}

int tokenize(struct Tokenizer *t) {
    if (t == NULL) return 0;
    
    while (t->input[t->position] != '\0' && t->token_count < MAX_TOKENS) {
        skip_whitespace(t);
        
        if (t->input[t->position] == '\0') break;
        
        struct Token token;