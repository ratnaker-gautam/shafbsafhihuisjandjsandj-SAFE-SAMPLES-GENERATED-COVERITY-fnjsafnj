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
    
    while (*input && (isdigit(*input) || *input == '.')) {
        if (*input == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        }
        *end++ = *input++;
        if (end - output >= MAX_TOKEN_LEN - 1) return 0;
    }
    *end = '\0';
    return end - output > 0;
}

int parse_string(const char *input, char *output) {
    if (*input != '"') return 0;
    
    char *end = output;
    input++;
    
    while (*input && *input != '"') {
        *end++ = *input++;
        if (end - output >= MAX_TOKEN_LEN - 1) return 0;
    }
    
    if (*input != '"') return 0;
    *end = '\0';
    return 1;
}

int parse_identifier(const char *input, char *output) {
    if (!isalpha(*input) && *input != '_') return 0;
    
    char *end = output;
    *end++ = *input++;
    
    while (*input && (isalnum(*input) || *input == '_')) {
        *end++ = *input++;
        if (end - output >= MAX_TOKEN_LEN - 1) return 0;
    }
    *end = '\0';
    return 1;
}

int parse_operator(const char *input, char *output) {
    if (!is_operator_char(*input)) return 0;
    
    char *end = output;
    *end++ = *input++;
    
    if (is_operator_char(*input) && (*(end - 1) == '=' || *(end - 1) == '<' || *(end - 1) == '>' || *(end - 1) == '!')) {
        *end++ = *input++;
        if (end - output >= MAX_TOKEN_LEN - 1) return 0;
    }
    
    *end = '\0';
    return 1;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    const char *ptr = input;
    Token *token_ptr = tokens;
    int token_count = 0;
    
    while (*ptr && token_count < max_tokens) {
        while (isspace(*ptr)) ptr++;
        if (!*ptr) break;
        
        char buffer[MAX_TOKEN_LEN];
        
        if (parse_number(ptr, buffer)) {
            token_ptr->type = TOKEN_NUMBER;
            strncpy(token_ptr->value, buffer, MAX_TOKEN_LEN - 1);
            token_ptr->value[MAX_TOKEN_LEN - 1] = '\0';
            ptr += strlen(buffer);
        } else if (parse_string(ptr, buffer)) {
            token_ptr->type = TOKEN_STRING;
            strncpy(token_ptr->value, buffer, MAX_TOKEN_LEN - 1);
            token_ptr->value[MAX_TOKEN_LEN - 1] = '\0';
            ptr += strlen(buffer) + 2;
        } else if (parse_identifier(ptr, buffer)) {
            token_ptr->type = TOKEN_IDENTIFIER;
            strncpy(token_ptr->value, buffer, MAX_TOKEN_LEN - 1);
            token_ptr->value[MAX_TOKEN_LEN - 1] = '\0';
            ptr += strlen(buffer);
        } else if (parse_operator(ptr, buffer)) {
            token_ptr->type = TOKEN_OPERATOR;
            strncpy(token_ptr->value, buffer, MAX_TOKEN_LEN - 1);
            token_ptr->value[MAX_TOKEN_LEN - 1] = '\0';
            ptr += strlen(buffer);
        } else {
            token_ptr->type = TOKEN_INVALID;
            token_ptr->value[0] = *ptr;
            token_ptr->value[1] = '\0';
            ptr++;
        }
        
        token_ptr++;
        token_count++;
    }
    
    if (token_count < max_tokens) {
        token_ptr->type = TOKEN_EOF;
        token_ptr->value[0] = '\0';
        token_count++;
    }
    
    return token_count;
}

const char *token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_INVALID: return "INVALID";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input[1024];