//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_INVALID
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_number(const char *input, char *output) {
    char *end = output;
    int has_dot = 0;
    
    if (!input || !output) return 0;
    
    while (*input && (isdigit(*input) || *input == '.')) {
        if (*input == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        }
        if ((size_t)(end - output) >= MAX_TOKEN_LEN - 1) return 0;
        *end++ = *input++;
    }
    *end = '\0';
    return end - output > 0;
}

int parse_identifier(const char *input, char *output) {
    char *end = output;
    
    if (!input || !output) return 0;
    
    if (*input && (isalpha(*input) || *input == '_')) {
        if ((size_t)(end - output) >= MAX_TOKEN_LEN - 1) return 0;
        *end++ = *input++;
        while (*input && (isalnum(*input) || *input == '_')) {
            if ((size_t)(end - output) >= MAX_TOKEN_LEN - 1) return 0;
            *end++ = *input++;
        }
        *end = '\0';
        return 1;
    }
    return 0;
}

int parse_operator(const char *input, char *output) {
    char *end = output;
    
    if (!input || !output) return 0;
    
    if (is_operator_char(*input)) {
        if ((size_t)(end - output) >= MAX_TOKEN_LEN - 1) return 0;
        *end++ = *input++;
        if (is_operator_char(*input)) {
            if ((size_t)(end - output) >= MAX_TOKEN_LEN - 1) return 0;
            *end++ = *input++;
        }
        *end = '\0';
        return 1;
    }
    return 0;
}

int parse_string(const char *input, char *output) {
    char *end = output;
    
    if (!input || !output) return 0;
    
    if (*input == '"') {
        if ((size_t)(end - output) >= MAX_TOKEN_LEN - 1) return 0;
        *end++ = *input++;
        while (*input && *input != '"') {
            if ((size_t)(end - output) >= MAX_TOKEN_LEN - 1) return 0;
            *end++ = *input++;
        }
        if (*input == '"') {
            if ((size_t)(end - output) >= MAX_TOKEN_LEN - 1) return 0;
            *end++ = *input++;
            *end = '\0';
            return 1;
        }
    }
    return 0;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    const char *ptr = input;
    Token *token_ptr = tokens;
    int token_count = 0;
    
    if (!input || !tokens || max_tokens <= 0) return 0;
    
    while (*ptr && token_count < max_tokens) {
        while (isspace(*ptr)) ptr++;
        
        if (!*ptr) break;
        
        if (parse_number(ptr, token_ptr->value)) {
            token_ptr->type = TOKEN_NUMBER;
        } else if (parse_identifier(ptr, token_ptr->value)) {
            token_ptr->type = TOKEN_IDENTIFIER;
        } else if (parse_operator(ptr, token_ptr->value)) {
            token_ptr->type = TOKEN_OPERATOR;
        } else if (parse_string(ptr, token_ptr->value)) {
            token_ptr->type = TOKEN_STRING;
        } else {
            token_ptr->type = TOKEN_INVALID;
            token_ptr->value[0] = *ptr;
            token_ptr->value[1] = '\0';
            ptr++;
        }
        
        if (token_ptr->type != TOKEN_INVALID) {
            size_t advance = strlen(token_ptr->value);
            size_t input_len = strlen(input);
            if (ptr + advance > input + input_len) break;
            ptr += advance;
        }
        
        token_ptr++;
        token_count++;
        
        if (token_count >= max_tokens) break;
    }
    
    if (token_count < max_tokens) {
        token_ptr->type = TOKEN_EOF;
        strncpy(token_ptr->value, "EOF", MAX_TOKEN_LEN - 1);
        token_ptr->value[MAX_TOKEN_LEN - 1] = '\0';
        token_count++;
    }
    
    return token_count;
}

const char* token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";