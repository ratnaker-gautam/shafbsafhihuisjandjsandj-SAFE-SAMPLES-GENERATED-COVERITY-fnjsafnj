//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

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
    return c == '+' || c == '-' || c == '*' || c == '/' || 
           c == '=' || c == '<' || c == '>' || c == '!';
}

int is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

void skip_whitespace(struct Tokenizer *t) {
    while (t->input[t->position] != '\0' && is_whitespace(t->input[t->position])) {
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
    int len = 0;
    
    while (t->input[t->position] != '\0' && isdigit(t->input[t->position])) {
        if (len < MAX_TOKEN_LEN - 1) {
            token->value[len++] = t->input[t->position];
        }
        t->position++;
        t->column++;
    }
    
    if (len > 0) {
        token->value[len] = '\0';
        token->type = TOKEN_NUMBER;
        return 1;
    }
    return 0;
}

int read_identifier(struct Tokenizer *t, struct Token *token) {
    int len = 0;
    
    if (t->input[t->position] != '\0' && isalpha(t->input[t->position])) {
        while (t->input[t->position] != '\0' && 
               (isalnum(t->input[t->position]) || t->input[t->position] == '_')) {
            if (len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = t->input[t->position];
            }
            t->position++;
            t->column++;
        }
        
        if (len > 0) {
            token->value[len] = '\0';
            token->type = TOKEN_IDENTIFIER;
            return 1;
        }
    }
    return 0;
}

int read_operator(struct Tokenizer *t, struct Token *token) {
    int len = 0;
    
    if (t->input[t->position] != '\0' && is_operator_char(t->input[t->position])) {
        while (t->input[t->position] != '\0' && is_operator_char(t->input[t->position])) {
            if (len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = t->input[t->position];
            }
            t->position++;
            t->column++;
        }
        
        if (len > 0) {
            token->value[len] = '\0';
            token->type = TOKEN_OPERATOR;
            return 1;
        }
    }
    return 0;
}

int read_string(struct Tokenizer *t, struct Token *token) {
    int len = 0;
    
    if (t->input[t->position] == '"') {
        t->position++;
        t->column++;
        
        while (t->input[t->position] != '\0' && t->input[t->position] != '"') {
            if (len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = t->input[t->position];
            }
            t->position++;
            t->column++;
        }
        
        if (t->input[t->position] == '"') {
            t->position++;
            t->column++;
            token->value[len] = '\0';
            token->type = TOKEN_STRING;
            return 1;
        }
    }
    return 0;
}

int tokenize_next(struct Tokenizer *t) {
    if (t->token_count >= MAX_TOKENS) {
        return 0;
    }
    
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
    
    if (read_number(t, token) || read_identifier(t, token) ||