//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64
#define DELIMITERS " \t\n\r"
#define IS_OPERATOR(c) (c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '(' || c == ')')

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_END
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

Token tokens[MAX_TOKENS];
size_t token_count = 0;
size_t current_token = 0;

void tokenize(const char* input) {
    if (input == NULL) return;
    
    const char* ptr = input;
    token_count = 0;
    
    while (*ptr != '\0' && token_count < MAX_TOKENS) {
        while (isspace((unsigned char)*ptr)) ptr++;
        
        if (*ptr == '\0') break;
        
        if (IS_OPERATOR(*ptr)) {
            if (token_count < MAX_TOKENS) {
                tokens[token_count].type = TOKEN_OPERATOR;
                tokens[token_count].value[0] = *ptr;
                tokens[token_count].value[1] = '\0';
                token_count++;
            }
            ptr++;
            continue;
        }
        
        if (isdigit((unsigned char)*ptr)) {
            size_t len = 0;
            while (isdigit((unsigned char)*ptr) && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *ptr++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
            continue;
        }
        
        if (isalpha((unsigned char)*ptr) || *ptr == '_') {
            size_t len = 0;
            while ((isalnum((unsigned char)*ptr) || *ptr == '_') && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *ptr++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            token_count++;
            continue;
        }
        
        ptr++;
    }
    
    if (token_count < MAX_TOKENS) {
        tokens[token_count].type = TOKEN_END;
        tokens[token_count].value[0] = '\0';
    }
}

int parse_expression(void);

int parse_factor(void) {
    if (current_token >= token_count) return 0;
    
    if (tokens[current_token].type == TOKEN_NUMBER) {
        current_token++;
        return 1;
    }
    
    if (tokens[current_token].type == TOKEN_IDENTIFIER) {
        current_token++;
        return 1;
    }
    
    if (tokens[current_token].type == TOKEN_OPERATOR && 
        tokens[current_token].value[0] == '(') {
        current_token++;
        if (!parse_expression()) return 0;
        if (current_token >= token_count || 
            tokens[current_token].type != TOKEN_OPERATOR || 
            tokens[current_token].value[0] != ')') return 0;
        current_token++;
        return 1;
    }
    
    return 0;
}

int parse_term(void) {
    if (!parse_factor()) return 0;
    
    while (current_token < token_count && 
           tokens[current_token].type == TOKEN_OPERATOR &&
           (tokens[current_token].value[0] == '*' || 
            tokens[current_token].value[0] == '/')) {
        current_token++;
        if (!parse_factor()) return 0;
    }
    
    return 1;
}

int parse_expression(void) {
    if (!parse_term()) return 0;
    
    while (current_token < token_count && 
           tokens[current_token].type == TOKEN_OPERATOR &&
           (tokens[current_token].value[0] == '+' || 
            tokens[current_token].value[0] == '-')) {
        current_token++;
        if (!parse_term()) return 0;
    }
    
    return 1;
}

int parse_assignment(void) {
    if (current_token >= token_count || tokens[current_token].type != TOKEN_IDENTIFIER) return 0;
    current_token++;
    
    if (current_token >= token_count || 
        tokens[current_token].type != TOKEN_OPERATOR || 
        tokens[current_token].value[0] != '=') return 0;
    current_token++;
    
    return parse_expression();
}

int parse_statement(void) {
    if (current_token >= token_count) return 0;
    
    if (tokens[current_token].type == TOKEN_IDENTIFIER && 
        current_token + 1 < token_count && 
        tokens[current_token + 1].type == TOKEN_OPERATOR && 
        tokens[current_token + 1].value[0] == '=') {
        return parse_assignment();
    }
    
    return parse_expression();
}

void print_tokens(void) {
    for (size_t i = 0; i < token_count; i++) {
        const char* type_str = "UNKNOWN";
        switch (tokens[i].type) {
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case TOKEN_IDENTIFIER