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
    struct Token tokens[MAX_TOKENS];
    size_t token_count;
    char input[MAX_INPUT_LEN];
    size_t input_len;
    size_t position;
    int line;
    int column;
};

void tokenizer_init(struct Tokenizer *t, const char *input) {
    if (t == NULL || input == NULL) return;
    
    size_t len = strlen(input);
    if (len >= MAX_INPUT_LEN) len = MAX_INPUT_LEN - 1;
    
    strncpy(t->input, input, len);
    t->input[len] = '\0';
    t->input_len = len;
    t->position = 0;
    t->token_count = 0;
    t->line = 1;
    t->column = 1;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || 
           c == '=' || c == '<' || c == '>' || c == '!';
}

void skip_whitespace(struct Tokenizer *t) {
    if (t == NULL) return;
    
    while (t->position < t->input_len && isspace(t->input[t->position])) {
        if (t->input[t->position] == '\n') {
            t->line++;
            t->column = 1;
        } else {
            t->column++;
        }
        t->position++;
    }
}

int parse_number(struct Tokenizer *t, struct Token *token) {
    if (t == NULL || token == NULL) return 0;
    
    size_t start = t->position;
    int has_dot = 0;
    
    while (t->position < t->input_len && 
           (isdigit(t->input[t->position]) || t->input[t->position] == '.')) {
        if (t->input[t->position] == '.') {
            if (has_dot) break;
            has_dot = 1;
        }
        t->position++;
    }
    
    if (t->position > start) {
        size_t len = t->position - start;
        if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
        
        strncpy(token->value, &t->input[start], len);
        token->value[len] = '\0';
        token->type = TOKEN_NUMBER;
        token->line = t->line;
        token->column = t->column;
        
        t->column += (int)len;
        return 1;
    }
    
    return 0;
}

int parse_identifier(struct Tokenizer *t, struct Token *token) {
    if (t == NULL || token == NULL) return 0;
    
    size_t start = t->position;
    
    if (t->position < t->input_len && 
        (isalpha(t->input[t->position]) || t->input[t->position] == '_')) {
        t->position++;
        
        while (t->position < t->input_len && 
               (isalnum(t->input[t->position]) || t->input[t->position] == '_')) {
            t->position++;
        }
        
        size_t len = t->position - start;
        if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
        
        strncpy(token->value, &t->input[start], len);
        token->value[len] = '\0';
        token->type = TOKEN_IDENTIFIER;
        token->line = t->line;
        token->column = t->column;
        
        t->column += (int)len;
        return 1;
    }
    
    return 0;
}

int parse_string(struct Tokenizer *t, struct Token *token) {
    if (t == NULL || token == NULL) return 0;
    
    if (t->position < t->input_len && t->input[t->position] == '"') {
        size_t start = t->position;
        t->position++;
        t->column++;
        
        while (t->position < t->input_len && t->input[t->position] != '"') {
            if (t->input[t->position] == '\n') {
                t->line++;
                t->column = 1;
            } else {
                t->column++;
            }
            t->position++;
        }
        
        if (t->position < t->input_len && t->input[t->position] == '"') {
            size_t len = t->position - start - 1;
            if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
            
            strncpy(token->value, &t->input[start + 1], len);
            token->value[len] = '\0';
            token->type = TOKEN_STRING;
            token->line = t->line;
            token->column = t->column;
            
            t->position++;
            t