//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64
#define DELIMITERS " \t\n\r"

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

Token tokens[MAX_TOKENS];
size_t token_count = 0;
size_t current_token = 0;

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

void tokenize(const char* input) {
    const char* p = input;
    token_count = 0;
    
    while (*p && token_count < MAX_TOKENS) {
        while (*p && strchr(DELIMITERS, *p)) p++;
        if (!*p) break;
        
        Token* token = &tokens[token_count];
        memset(token->value, 0, MAX_TOKEN_LEN);
        
        if (isalpha(*p) || *p == '_') {
            token->type = TOKEN_IDENTIFIER;
            size_t i = 0;
            while ((isalnum(*p) || *p == '_') && i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = *p++;
            }
        } else if (isdigit(*p)) {
            token->type = TOKEN_NUMBER;
            size_t i = 0;
            while (isdigit(*p) && i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = *p++;
            }
        } else if (*p == '"') {
            token->type = TOKEN_STRING;
            size_t i = 0;
            p++;
            while (*p && *p != '"' && i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = *p++;
            }
            if (*p == '"') p++;
        } else if (is_operator(*p)) {
            token->type = TOKEN_OPERATOR;
            size_t i = 0;
            while (is_operator(*p) && i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = *p++;
            }
        } else {
            token->type = TOKEN_UNKNOWN;
            token->value[0] = *p++;
        }
        
        token_count++;
    }
    
    if (token_count < MAX_TOKENS) {
        tokens[token_count].type = TOKEN_EOF;
        strncpy(tokens[token_count].value, "EOF", MAX_TOKEN_LEN - 1);
        token_count++;
    }
}

const char* token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

void print_tokens() {
    for (size_t i = 0; i < token_count; i++) {
        printf("Token %zu: %s -> '%s'\n", i, token_type_name(tokens[i].type), tokens[i].value);
    }
}

int parse_expression();
int parse_term();
int parse_factor();

int parse_factor() {
    if (current_token >= token_count) return 0;
    
    Token* token = &tokens[current_token];
    if (token->type == TOKEN_NUMBER) {
        current_token++;
        return 1;
    } else if (token->type == TOKEN_IDENTIFIER) {
        current_token++;
        return 1;
    } else if (token->value[0] == '(') {
        current_token++;
        if (!parse_expression()) return 0;
        if (current_token >= token_count || tokens[current_token].value[0] != ')') return 0;
        current_token++;
        return 1;
    }
    return 0;
}

int parse_term() {
    if (!parse_factor()) return 0;
    
    while (current_token < token_count && 
           (tokens[current_token].value[0] == '*' || tokens[current_token].value[0] == '/')) {
        current_token++;
        if (!parse_factor()) return 0;
    }
    return 1;
}

int parse_expression() {
    if (!parse_term()) return 0;
    
    while (current_token < token_count && 
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
        if (current_token >= token_count || tokens[current_token].value[0] != '=') return 0;
        current_token++;
        return parse_expression();
    }
    return parse_expression();
}

int main() {
    char input[1024];
    
    printf("Enter