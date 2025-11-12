//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50
#define DELIMITERS " \t\n\r"

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

Token tokens[MAX_TOKENS];
size_t token_count = 0;
size_t current_token = 0;

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '(' || c == ')';
}

void tokenize(const char* input) {
    const char* p = input;
    token_count = 0;
    
    while (*p && token_count < MAX_TOKENS) {
        while (*p && strchr(DELIMITERS, *p)) p++;
        if (!*p) break;
        
        if (isdigit(*p)) {
            size_t len = 0;
            while (*p && isdigit(*p) && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *p++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
        }
        else if (isalpha(*p) || *p == '_') {
            size_t len = 0;
            while (*p && (isalnum(*p) || *p == '_') && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *p++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            token_count++;
        }
        else if (*p == '"') {
            size_t len = 0;
            p++;
            while (*p && *p != '"' && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *p++;
            }
            if (*p == '"') p++;
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_STRING;
            token_count++;
        }
        else if (is_operator_char(*p)) {
            tokens[token_count].value[0] = *p++;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
            token_count++;
        }
        else {
            p++;
        }
    }
    
    tokens[token_count].type = TOKEN_EOF;
    tokens[token_count].value[0] = '\0';
}

const char* token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

void print_tokens() {
    for (size_t i = 0; i < token_count; i++) {
        printf("Token %zu: %s '%s'\n", i, token_type_name(tokens[i].type), tokens[i].value);
    }
}

int parse_expression();

int parse_factor() {
    if (current_token >= token_count) return 0;
    
    if (tokens[current_token].type == TOKEN_NUMBER) {
        current_token++;
        return 1;
    }
    else if (tokens[current_token].type == TOKEN_IDENTIFIER) {
        current_token++;
        return 1;
    }
    else if (tokens[current_token].type == TOKEN_OPERATOR && tokens[current_token].value[0] == '(') {
        current_token++;
        if (!parse_expression()) return 0;
        if (current_token >= token_count || tokens[current_token].type != TOKEN_OPERATOR || tokens[current_token].value[0] != ')') return 0;
        current_token++;
        return 1;
    }
    
    return 0;
}

int parse_term() {
    if (!parse_factor()) return 0;
    
    while (current_token < token_count && tokens[current_token].type == TOKEN_OPERATOR && 
           (tokens[current_token].value[0] == '*' || tokens[current_token].value[0] == '/')) {
        current_token++;
        if (!parse_factor()) return 0;
    }
    
    return 1;
}

int parse_expression() {
    if (!parse_term()) return 0;
    
    while (current_token < token_count && tokens[current_token].type == TOKEN_OPERATOR && 
           (tokens[current_token].value[0] == '+' || tokens[current_token].value[0] == '-')) {
        current_token++;
        if (!parse_term()) return 0;
    }
    
    return 1;
}

int parse_statement() {
    if (current_token >= token_count) return 0;
    
    if (tokens[current_token].type == TOKEN_IDENTIFIER) {
        current_token++;
        if (current_token >= token_count || tokens[current_token].type != TOKEN_OPERATOR || tokens[current_token