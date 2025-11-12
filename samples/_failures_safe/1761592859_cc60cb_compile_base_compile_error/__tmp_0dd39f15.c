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
    
    while (t->input[t->position] != '\0' && isdigit(t->input[t->position])) {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        token->value[len++] = t->input[t->position++];
        t->column++;
    }
    
    token->value[len] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int read_identifier(struct Tokenizer *t, struct Token *token) {
    int len = 0;
    
    if (t->input[t->position] != '\0' && isalpha(t->input[t->position])) {
        token->value[len++] = t->input[t->position++];
        t->column++;
    } else {
        return 0;
    }
    
    while (t->input[t->position] != '\0' && (isalnum(t->input[t->position]) || t->input[t->position] == '_')) {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        token->value[len++] = t->input[t->position++];
        t->column++;
    }
    
    token->value[len] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int read_operator(struct Tokenizer *t, struct Token *token) {
    int len = 0;
    
    if (t->input[t->position] != '\0' && is_operator_char(t->input[t->position])) {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        token->value[len++] = t->input[t->position++];
        t->column++;
    } else {
        return 0;
    }
    
    token->value[len] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

int read_string(struct Tokenizer *t, struct Token *token) {
    int len = 0;
    
    if (t->input[t->position] != '\"') return 0;
    
    token->value[len++] = t->input[t->position++];
    t->column++;
    
    while (t->input[t->position] != '\0' && t->input[t->position] != '\"') {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        token->value[len++] = t->input[t->position++];
        t->column++;
    }
    
    if (t->input[t->position] == '\"') {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        token->value[len++] = t->input[t->position++];
        t->column++;
    } else {
        return 0;
    }
    
    token->value[len] = '\0';
    token->type = TOKEN_STRING;
    return 1;
}

int tokenize(struct Tokenizer *t) {
    while (t->input[t->position] != '\0' && t->token_count < MAX_TOKENS) {
        skip_whitespace(t);
        
        if (t->input[t->position] == '\0') break;
        
        struct Token token;
        token.line = t->line;
        token.column = t->column;
        
        int success = 0;
        
        if (isdigit(t->input[t->position])) {
            success = read_number(t, &token);
        } else if (isalpha(t->input[t->position])) {
            success = read_identifier(t, &token);
        } else if (t->input[t->position] == '\"') {
            success = read_string(t, &token);
        } else if (is_operator_char(t->input[t->position])) {
            success = read_operator(t, &token);
        }
        
        if (success) {