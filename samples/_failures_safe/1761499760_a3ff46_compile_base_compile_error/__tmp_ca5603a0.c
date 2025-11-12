//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int parse_operator(const char *input, char *output) {
    if (!input || !output) return 0;
    
    const char *start = input;
    char *out_ptr = output;
    
    if (is_operator_char(*input)) {
        *out_ptr++ = *input++;
        
        if ((*start == '&' && *input == '&') ||
            (*start == '|' && *input == '|') ||
            (*start == '=' && *input == '=') ||
            (*start == '!' && *input == '=') ||
            (*start == '<' && *input == '=') ||
            (*start == '>' && *input == '=')) {
            *out_ptr++ = *input++;
        }
    }
    
    *out_ptr = '\0';
    return out_ptr - output;
}

int parse_string(const char *input, char *output) {
    if (!input || !output || *input != '"') return 0;
    
    const char *ptr = input + 1;
    char *out_ptr = output;
    
    while (*ptr && *ptr != '"') {
        if (out_ptr - output >= MAX_TOKEN_LEN - 1) return 0;
        *out_ptr++ = *ptr++;
    }
    
    if (*ptr != '"') return 0;
    
    *out_ptr = '\0';
    return (ptr - input) + 1;
}

int parse_number(const char *input, char *output) {
    if (!input || !output || !isdigit(*input)) return 0;
    
    const char *ptr = input;
    char *out_ptr = output;
    
    while (*ptr && isdigit(*ptr)) {
        if (out_ptr - output >= MAX_TOKEN_LEN - 1) return 0;
        *out_ptr++ = *ptr++;
    }
    
    *out_ptr = '\0';
    return ptr - input;
}

int parse_identifier(const char *input, char *output) {
    if (!input || !output || !isalpha(*input)) return 0;
    
    const char *ptr = input;
    char *out_ptr = output;
    
    while (*ptr && (isalnum(*ptr) || *ptr == '_')) {
        if (out_ptr - output >= MAX_TOKEN_LEN - 1) return 0;
        *out_ptr++ = *ptr++;
    }
    
    *out_ptr = '\0';
    return ptr - input;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    if (!input || !tokens || max_tokens <= 0) return 0;
    
    const char *ptr = input;
    Token *token_ptr = tokens;
    int token_count = 0;
    
    while (*ptr && token_count < max_tokens) {
        while (*ptr && isspace(*ptr)) ptr++;
        
        if (!*ptr) break;
        
        char buffer[MAX_TOKEN_LEN];
        int consumed = 0;
        
        if (*ptr == '"') {
            consumed = parse_string(ptr, buffer);
            if (consumed > 0) {
                token_ptr->type = TOKEN_STRING;
                strncpy(token_ptr->value, buffer, MAX_TOKEN_LEN - 1);
                token_ptr->value[MAX_TOKEN_LEN - 1] = '\0';
            }
        } else if (isdigit(*ptr)) {
            consumed = parse_number(ptr, buffer);
            if (consumed > 0) {
                token_ptr->type = TOKEN_NUMBER;
                strncpy(token_ptr->value, buffer, MAX_TOKEN_LEN - 1);
                token_ptr->value[MAX_TOKEN_LEN - 1] = '\0';
            }
        } else if (is_operator_char(*ptr)) {
            consumed = parse_operator(ptr, buffer);
            if (consumed > 0) {
                token_ptr->type = TOKEN_OPERATOR;
                strncpy(token_ptr->value, buffer, MAX_TOKEN_LEN - 1);
                token_ptr->value[MAX_TOKEN_LEN - 1] = '\0';
            }
        } else if (isalpha(*ptr)) {
            consumed = parse_identifier(ptr, buffer);
            if (consumed > 0) {
                token_ptr->type = TOKEN_IDENTIFIER;
                strncpy(token_ptr->value, buffer, MAX_TOKEN_LEN - 1);
                token_ptr->value[MAX_TOKEN_LEN - 1] = '\0';
            }
        }
        
        if (consumed <= 0) {
            token_ptr->type = TOKEN_INVALID;
            token_ptr->value[0] = *ptr;
            token_ptr->value[1] = '\0';
            consumed = 1;
        }
        
        ptr += consumed;
        token_ptr++;