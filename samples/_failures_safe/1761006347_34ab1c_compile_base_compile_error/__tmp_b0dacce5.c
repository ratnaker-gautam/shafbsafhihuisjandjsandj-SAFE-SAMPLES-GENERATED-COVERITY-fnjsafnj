//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
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
        if (is_operator_char(*input) && 
            ((*start == '<' && *input == '=') ||
             (*start == '>' && *input == '=') ||
             (*start == '!' && *input == '=') ||
             (*start == '=' && *input == '=') ||
             (*start == '&' && *input == '&') ||
             (*start == '|' && *input == '|'))) {
            *out_ptr++ = *input++;
        }
        *out_ptr = '\0';
        return (int)(input - start);
    }
    return 0;
}

int parse_number(const char *input, char *output) {
    if (!input || !output) return 0;
    
    const char *start = input;
    char *out_ptr = output;
    
    while (*input && isdigit(*input)) {
        if ((out_ptr - output) < MAX_TOKEN_LEN - 1) {
            *out_ptr++ = *input++;
        } else {
            break;
        }
    }
    
    if (out_ptr > output) {
        *out_ptr = '\0';
        return (int)(input - start);
    }
    return 0;
}

int parse_string(const char *input, char *output) {
    if (!input || !output || *input != '"') return 0;
    
    const char *start = input;
    char *out_ptr = output;
    
    *out_ptr++ = *input++;
    
    while (*input && *input != '"') {
        if ((out_ptr - output) < MAX_TOKEN_LEN - 2) {
            *out_ptr++ = *input++;
        } else {
            break;
        }
    }
    
    if (*input == '"') {
        *out_ptr++ = *input++;
        *out_ptr = '\0';
        return (int)(input - start);
    }
    return 0;
}

int parse_identifier(const char *input, char *output) {
    if (!input || !output || (!isalpha(*input) && *input != '_')) return 0;
    
    const char *start = input;
    char *out_ptr = output;
    
    while (*input && (isalnum(*input) || *input == '_')) {
        if ((out_ptr - output) < MAX_TOKEN_LEN - 1) {
            *out_ptr++ = *input++;
        } else {
            break;
        }
    }
    
    if (out_ptr > output) {
        *out_ptr = '\0';
        return (int)(input - start);
    }
    return 0;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    if (!input || !tokens || max_tokens <= 0) return 0;
    
    const char *ptr = input;
    Token *token_ptr = tokens;
    int token_count = 0;
    
    while (*ptr && token_count < max_tokens) {
        while (*ptr && isspace(*ptr)) {
            ptr++;
        }
        
        if (!*ptr) break;
        
        char buffer[MAX_TOKEN_LEN];
        int consumed = 0;
        
        if ((consumed = parse_string(ptr, buffer)) > 0) {
            token_ptr->type = TOKEN_STRING;
            strncpy(token_ptr->value, buffer, MAX_TOKEN_LEN - 1);
            token_ptr->value[MAX_TOKEN_LEN - 1] = '\0';
        } else if ((consumed = parse_number(ptr, buffer)) > 0) {
            token_ptr->type = TOKEN_NUMBER;
            strncpy(token_ptr->value, buffer, MAX_TOKEN_LEN - 1);
            token_ptr->value[MAX_TOKEN_LEN - 1] = '\0';
        } else if ((consumed = parse_operator(ptr, buffer)) > 0) {
            token_ptr->type = TOKEN_OPERATOR;
            strncpy(token_ptr->value, buffer, MAX_TOKEN_LEN - 1);
            token_ptr->value[MAX_TOKEN_LEN - 1] = '\0';
        } else if ((consumed = parse_identifier(ptr, buffer)) > 0) {
            token_ptr->type = TOKEN_IDENTIFIER;
            strncpy(token_ptr->value, buffer, MAX_TOKEN_LEN - 1);
            token_ptr->value[MAX_TOKEN_LEN - 1] = '\0';
        } else {
            token_ptr->type = TOKEN_INVALID;
            token_ptr->value[0