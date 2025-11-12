//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

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

typedef void (*TokenHandler)(Token*);

void handle_number(Token* token) {
    if (token) {
        printf("Number: %s\n", token->value);
    }
}

void handle_identifier(Token* token) {
    if (token) {
        printf("Identifier: %s\n", token->value);
    }
}

void handle_operator(Token* token) {
    if (token) {
        printf("Operator: %s\n", token->value);
    }
}

void handle_string(Token* token) {
    if (token) {
        printf("String: %s\n", token->value);
    }
}

void skip_whitespace(const char** input) {
    if (input && *input) {
        while (**input && isspace((unsigned char)**input)) {
            (*input)++;
        }
    }
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int parse_number(const char** input, Token* token) {
    if (!input || !*input || !token) return 0;
    
    const char* start = *input;
    int has_digit = 0;
    
    while (**input && (isdigit((unsigned char)**input) || **input == '.')) {
        if (**input == '.') {
            if (has_digit == 2) return 0;
            has_digit = 2;
        } else if (has_digit == 0) {
            has_digit = 1;
        }
        (*input)++;
    }
    
    if (has_digit) {
        size_t len = *input - start;
        if (len >= MAX_TOKEN_LEN) return 0;
        strncpy(token->value, start, len);
        token->value[len] = '\0';
        token->type = TOKEN_NUMBER;
        return 1;
    }
    
    return 0;
}

int parse_identifier(const char** input, Token* token) {
    if (!input || !*input || !token) return 0;
    
    const char* start = *input;
    
    if (**input && (isalpha((unsigned char)**input) || **input == '_')) {
        (*input)++;
        while (**input && (isalnum((unsigned char)**input) || **input == '_')) {
            (*input)++;
        }
        
        size_t len = *input - start;
        if (len >= MAX_TOKEN_LEN) return 0;
        strncpy(token->value, start, len);
        token->value[len] = '\0';
        token->type = TOKEN_IDENTIFIER;
        return 1;
    }
    
    return 0;
}

int parse_operator(const char** input, Token* token) {
    if (!input || !*input || !token) return 0;
    
    const char* start = *input;
    
    if (**input && is_operator_char(**input)) {
        (*input)++;
        while (**input && is_operator_char(**input)) {
            (*input)++;
        }
        
        size_t len = *input - start;
        if (len >= MAX_TOKEN_LEN) return 0;
        strncpy(token->value, start, len);
        token->value[len] = '\0';
        token->type = TOKEN_OPERATOR;
        return 1;
    }
    
    return 0;
}

int parse_string(const char** input, Token* token) {
    if (!input || !*input || !token || **input != '"') return 0;
    
    const char* start = *input;
    (*input)++;
    
    while (**input && **input != '"') {
        if (**input == '\\' && *(*input + 1)) {
            (*input) += 2;
        } else {
            (*input)++;
        }
    }
    
    if (**input != '"') return 0;
    (*input)++;
    
    size_t len = *input - start;
    if (len >= MAX_TOKEN_LEN) return 0;
    strncpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_STRING;
    return 1;
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    if (!input || !tokens || max_tokens <= 0) return 0;
    
    int count = 0;
    
    while (*input && count < max_tokens) {
        skip_whitespace(&input);
        if (!*input) break;
        
        Token token;
        int parsed = 0;
        
        if (parse_number(&input, &token)) parsed = 1;
        else if (parse_string(&input, &token)) parsed = 1;
        else if (parse_identifier(&input, &token)) parsed = 1;
        else if (parse_operator(&input, &token)) parsed = 1;
        
        if (parsed) {
            if (count < max_tokens)