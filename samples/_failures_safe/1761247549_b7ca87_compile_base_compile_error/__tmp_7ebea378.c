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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
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

int add_token(struct Tokenizer *t, enum TokenType type, const char *value) {
    if (t->token_count >= MAX_TOKENS) {
        return 0;
    }
    struct Token *token = &t->tokens[t->token_count];
    token->type = type;
    token->line = t->line;
    token->column = t->column;
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
    t->token_count++;
    return 1;
}

int parse_number(struct Tokenizer *t) {
    char buffer[MAX_TOKEN_LEN] = {0};
    int i = 0;
    
    while (isdigit(t->input[t->position]) && i < MAX_TOKEN_LEN - 1) {
        buffer[i++] = t->input[t->position++];
        t->column++;
    }
    
    if (i > 0) {
        buffer[i] = '\0';
        return add_token(t, TOKEN_NUMBER, buffer);
    }
    return 0;
}

int parse_identifier(struct Tokenizer *t) {
    char buffer[MAX_TOKEN_LEN] = {0};
    int i = 0;
    
    if (isalpha(t->input[t->position]) || t->input[t->position] == '_') {
        buffer[i++] = t->input[t->position++];
        t->column++;
        
        while ((isalnum(t->input[t->position]) || t->input[t->position] == '_') && i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = t->input[t->position++];
            t->column++;
        }
        
        buffer[i] = '\0';
        return add_token(t, TOKEN_IDENTIFIER, buffer);
    }
    return 0;
}

int parse_operator(struct Tokenizer *t) {
    char buffer[3] = {0};
    int i = 0;
    
    if (is_operator_char(t->input[t->position])) {
        buffer[i++] = t->input[t->position++];
        t->column++;
        
        if (is_operator_char(t->input[t->position]) && i < 2) {
            buffer[i++] = t->input[t->position++];
            t->column++;
        }
        
        buffer[i] = '\0';
        return add_token(t, TOKEN_OPERATOR, buffer);
    }
    return 0;
}

int parse_string(struct Tokenizer *t) {
    char buffer[MAX_TOKEN_LEN] = {0};
    int i = 0;
    
    if (t->input[t->position] == '"') {
        t->position++;
        t->column++;
        
        while (t->input[t->position] != '"' && t->input[t->position] != '\0' && i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = t->input[t->position++];
            t->column++;
        }
        
        if (t->input[t->position] == '"') {
            t->position++;
            t->column++;
            buffer[i] = '\0';
            return add_token(t, TOKEN_STRING, buffer);
        }
    }
    return 0;
}

int tokenize(struct Tokenizer *t) {
    while (t->input[t->position] != '\0' && t->token_count < MAX_TOKENS) {
        skip_whitespace(t);
        
        if (t->input[t->position] == '\0') {
            break;
        }
        
        if (parse_number(t)) continue;
        if (parse_identifier(t)) continue;
        if (parse_operator(t)) continue;
        if (parse_string(t)) continue;
        
        char invalid[2] = {t->input[t->position], '\0'};
        if (!add_token(t, TOKEN_INVALID, invalid)) {
            return