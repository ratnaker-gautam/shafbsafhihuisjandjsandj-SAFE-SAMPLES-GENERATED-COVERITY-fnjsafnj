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
    
    if (!input || !*input || !token) return 0;
    
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
    
    strncpy(token->value, op, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

int parse_number(const char **input, Token *token) {
    const char *start = *input;
    char *dest = token->value;
    int len = 0;
    
    if (!input || !*input || !token) return 0;
    
    while (**input && isdigit(**input)) {
        if (len < MAX_TOKEN_LEN - 1) {
            *dest++ = **input;
            len++;
        }
        (*input)++;
    }
    
    if (len == 0) return 0;
    
    *dest = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_string(const char **input, Token *token) {
    if (!input || !*input || !token) return 0;
    if (**input != '"') return 0;
    
    (*input)++;
    char *dest = token->value;
    int len = 0;
    
    *dest++ = '"';
    len++;
    
    while (**input && **input != '"') {
        if (len < MAX_TOKEN_LEN - 2) {
            *dest++ = **input;
            len++;
        }
        (*input)++;
    }
    
    if (**input == '"') {
        *dest++ = '"';
        len++;
        (*input)++;
    }
    
    *dest = '\0';
    token->type = TOKEN_STRING;
    return 1;
}

int parse_identifier(const char **input, Token *token) {
    const char *start = *input;
    char *dest = token->value;
    int len = 0;
    
    if (!input || !*input || !token) return 0;
    if (!isalpha(**input) && **input != '_') return 0;
    
    while (**input && (isalnum(**input) || **input == '_')) {
        if (len < MAX_TOKEN_LEN - 1) {
            *dest++ = **input;
            len++;
        }
        (*input)++;
    }
    
    if (len == 0) return 0;
    
    *dest = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int get_next_token(const char **input, Token *token) {
    if (!input || !*input || !token) return 0;
    
    while (**input && isspace(**input)) {
        (*input)++;
    }
    
    if (**input == '\0') {
        strncpy(token->value, "EOF", MAX_TOKEN_LEN - 1);
        token->value[MAX_TOKEN_LEN - 1] = '\0';
        token->type = TOKEN_EOF;
        return 1;
    }
    
    if (parse_string(input, token)) return 1;
    if (parse_number(input, token)) return 1;
    if (parse_operator(input, token)) return 1;
    if (parse_identifier(input, token)) return 1;
    
    token->value[0] = **input;
    token->value[1] = '\0';
    token->type = TOKEN_INVALID;
    (*input)++;
    return 1;
}

void tokenize(const char *input, Token *tokens, int *token_count) {
    const char *current = input;
    
    if (!input || !tokens || !token_count) return;
    
    *token_count = 0;
    
    while (*current && *token_count < MAX_TOKENS - 1) {
        if (!get_next_token(&current, &tokens[*token_count])) {
            break;
        }
        (*token_count)++;
    }
    
    if (*token_count < MAX_TOKENS) {
        strnc