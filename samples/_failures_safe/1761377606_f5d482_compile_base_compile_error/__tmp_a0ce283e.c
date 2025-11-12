//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_LEN 256
#define MAX_TOKENS 1000

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_PAREN,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

typedef struct {
    Token tokens[MAX_TOKENS];
    int count;
    int pos;
} Tokenizer;

void init_tokenizer(Tokenizer *t) {
    t->count = 0;
    t->pos = 0;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

int tokenize(Tokenizer *t, const char *input) {
    int i = 0;
    int token_count = 0;
    
    while (input[i] != '\0' && token_count < MAX_TOKENS) {
        while (isspace(input[i])) i++;
        
        if (input[i] == '\0') break;
        
        if (isdigit(input[i])) {
            int j = 0;
            while (isdigit(input[i]) && j < MAX_TOKEN_LEN - 1) {
                t->tokens[token_count].value[j++] = input[i++];
            }
            t->tokens[token_count].value[j] = '\0';
            t->tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
        }
        else if (isalpha(input[i])) {
            int j = 0;
            while (isalnum(input[i]) && j < MAX_TOKEN_LEN - 1) {
                t->tokens[token_count].value[j++] = input[i++];
            }
            t->tokens[token_count].value[j] = '\0';
            t->tokens[token_count].type = TOKEN_IDENTIFIER;
            token_count++;
        }
        else if (is_operator_char(input[i])) {
            t->tokens[token_count].value[0] = input[i++];
            t->tokens[token_count].value[1] = '\0';
            t->tokens[token_count].type = TOKEN_OPERATOR;
            token_count++;
        }
        else if (input[i] == '(' || input[i] == ')') {
            t->tokens[token_count].value[0] = input[i++];
            t->tokens[token_count].value[1] = '\0';
            t->tokens[token_count].type = TOKEN_PAREN;
            token_count++;
        }
        else {
            return -1;
        }
    }
    
    t->tokens[token_count].type = TOKEN_EOF;
    t->tokens[token_count].value[0] = '\0';
    t->count = token_count + 1;
    return 0;
}

Token *peek_token(Tokenizer *t) {
    if (t->pos >= t->count) return NULL;
    return &t->tokens[t->pos];
}

Token *next_token(Tokenizer *t) {
    if (t->pos >= t->count) return NULL;
    return &t->tokens[t->pos++];
}

int parse_expression(Tokenizer *t);

int parse_factor(Tokenizer *t) {
    Token *tok = peek_token(t);
    if (!tok) return 0;
    
    if (tok->type == TOKEN_NUMBER) {
        next_token(t);
        return 1;
    }
    else if (tok->type == TOKEN_IDENTIFIER) {
        next_token(t);
        return 1;
    }
    else if (tok->type == TOKEN_PAREN && tok->value[0] == '(') {
        next_token(t);
        if (!parse_expression(t)) return 0;
        tok = next_token(t);
        if (!tok || tok->type != TOKEN_PAREN || tok->value[0] != ')') return 0;
        return 1;
    }
    return 0;
}

int parse_term(Tokenizer *t) {
    if (!parse_factor(t)) return 0;
    
    Token *tok = peek_token(t);
    while (tok && tok->type == TOKEN_OPERATOR && 
           (tok->value[0] == '*' || tok->value[0] == '/')) {
        next_token(t);
        if (!parse_factor(t)) return 0;
        tok = peek_token(t);
    }
    return 1;
}

int parse_expression(Tokenizer *t) {
    if (!parse_term(t)) return 0;
    
    Token *tok = peek_token(t);
    while (tok && tok->type == TOKEN_OPERATOR && 
           (tok->value[0] == '+' || tok->value[0] == '-')) {
        next_token(t);
        if (!parse_term(t)) return 0;
        tok = peek_token(t);
    }
    return 1;
}

int parse_assignment(Tokenizer *t) {
    Token *tok = next_token(t);
    if (!tok || tok->type != TOKEN_IDENTIFIER) return 0;
    
    tok = next_token(t);
    if (!tok || tok->type != TOKEN_OPERATOR || tok->value[0] != '=') return 0;
    
    if (!parse_expression(t)) return 0;
    
    tok =