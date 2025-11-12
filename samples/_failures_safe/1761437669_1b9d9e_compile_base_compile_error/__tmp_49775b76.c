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

int parse_operator(const char **input, Token *token) {
    const char *start = *input;
    char op[3] = {0};
    
    if (!input || !*input || !**input || !token) {
        return 0;
    }
    
    op[0] = **input;
    (*input)++;
    
    if (**input && is_operator_char(**input)) {
        if ((*start == '<' && **input == '=') ||
            (*start == '>' && **input == '=') ||
            (*start == '!' && **input == '=') ||
            (*start == '=' && **input == '=') ||
            (*start == '&' && **input == '&') ||
            (*start == '|' && **input == '|')) {
            op[1] = **input;
            (*input)++;
        }
    }
    
    if (strlen(op) >= sizeof(token->value)) {
        return 0;
    }
    strncpy(token->value, op, sizeof(token->value) - 1);
    token->value[sizeof(token->value) - 1] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

int parse_number(const char **input, Token *token) {
    const char *start;
    char *end;
    
    if (!input || !*input || !**input || !token) {
        return 0;
    }
    
    start = *input;
    strtol(*input, &end, 10);
    
    if (end == *input) {
        return 0;
    }
    
    size_t len = end - start;
    if (len >= sizeof(token->value)) {
        return 0;
    }
    
    memcpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_NUMBER;
    *input = end;
    return 1;
}

int parse_string(const char **input, Token *token) {
    const char *start;
    size_t len;
    
    if (!input || !*input || !**input || !token) {
        return 0;
    }
    
    if (**input != '"') {
        return 0;
    }
    
    (*input)++;
    start = *input;
    len = 0;
    
    while (**input && **input != '"' && len < sizeof(token->value) - 1) {
        (*input)++;
        len++;
    }
    
    if (**input != '"') {
        return 0;
    }
    
    if (len >= sizeof(token->value)) {
        return 0;
    }
    
    memcpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_STRING;
    (*input)++;
    return 1;
}

int parse_identifier(const char **input, Token *token) {
    const char *start;
    size_t len;
    
    if (!input || !*input || !**input || !token) {
        return 0;
    }
    
    if (!isalpha(**input) && **input != '_') {
        return 0;
    }
    
    start = *input;
    len = 0;
    
    while (**input && (isalnum(**input) || **input == '_') && len < sizeof(token->value) - 1) {
        (*input)++;
        len++;
    }
    
    if (len >= sizeof(token->value)) {
        return 0;
    }
    
    memcpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

void skip_whitespace(const char **input) {
    if (!input || !*input) {
        return;
    }
    
    while (**input && isspace(**input)) {
        (*input)++;
    }
}

int tokenize(const char *input, Token *tokens, size_t max_tokens) {
    size_t token_count = 0;
    
    if (!input || !tokens || max_tokens == 0) {
        return 0;
    }
    
    while (*input && token_count < max_tokens) {
        skip_whitespace(&input);
        
        if (!*input) {
            break;
        }
        
        Token token = {TOKEN_INVALID, ""};
        int parsed = 0;
        
        if (parse_number(&input, &token)) {
            parsed = 1;
        } else if (parse_string(&input, &token)) {
            parsed = 1;
        } else if (parse_operator(&input, &token)) {
            parsed = 1;
        } else if (parse_identifier(&input, &token