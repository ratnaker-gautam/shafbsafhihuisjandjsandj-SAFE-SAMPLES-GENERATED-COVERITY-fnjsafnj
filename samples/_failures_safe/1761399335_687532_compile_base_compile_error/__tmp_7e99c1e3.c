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

int parse_operator(char **input, Token *token) {
    char *start = *input;
    char op[3] = {0};
    
    if (!start || !*start) return 0;
    
    op[0] = *start;
    if (*(start + 1) && is_operator_char(*(start + 1))) {
        op[1] = *(start + 1);
        if ((op[0] == '&' && op[1] == '&') || (op[0] == '|' && op[1] == '|') ||
            (op[0] == '=' && op[1] == '=') || (op[0] == '!' && op[1] == '=') ||
            (op[0] == '<' && op[1] == '=') || (op[0] == '>' && op[1] == '=')) {
            if (strlen(op) < MAX_TOKEN_LEN) {
                strncpy(token->value, op, 2);
                token->value[2] = '\0';
                *input += 2;
                return 1;
            }
        }
    }
    
    if (strlen(op) < MAX_TOKEN_LEN) {
        strncpy(token->value, op, 1);
        token->value[1] = '\0';
        *input += 1;
        return 1;
    }
    return 0;
}

int parse_number(char **input, Token *token) {
    char *start = *input;
    char *end = start;
    
    if (!start || !*start) return 0;
    
    while (*end && isdigit(*end)) {
        end++;
    }
    
    if (end - start >= MAX_TOKEN_LEN || end == start) {
        return 0;
    }
    
    strncpy(token->value, start, end - start);
    token->value[end - start] = '\0';
    *input = end;
    return 1;
}

int parse_string(char **input, Token *token) {
    char *start = *input;
    
    if (!start || !*start || *start != '"') {
        return 0;
    }
    
    start++;
    char *end = start;
    
    while (*end && *end != '"') {
        end++;
        if (end - start >= MAX_TOKEN_LEN - 1) {
            return 0;
        }
    }
    
    if (*end != '"') {
        return 0;
    }
    
    strncpy(token->value, start, end - start);
    token->value[end - start] = '\0';
    *input = end + 1;
    return 1;
}

int parse_identifier(char **input, Token *token) {
    char *start = *input;
    
    if (!start || !*start || (!isalpha(*start) && *start != '_')) {
        return 0;
    }
    
    char *end = start + 1;
    while (*end && (isalnum(*end) || *end == '_')) {
        end++;
        if (end - start >= MAX_TOKEN_LEN) {
            return 0;
        }
    }
    
    strncpy(token->value, start, end - start);
    token->value[end - start] = '\0';
    *input = end;
    return 1;
}

int get_next_token(char **input, Token *token) {
    if (!input || !*input || !token) return 0;
    
    char *ptr = *input;
    
    while (*ptr && isspace(*ptr)) {
        ptr++;
    }
    
    if (!*ptr) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        *input = ptr;
        return 1;
    }
    
    if (parse_identifier(&ptr, token)) {
        token->type = TOKEN_IDENTIFIER;
        *input = ptr;
        return 1;
    }
    
    if (parse_number(&ptr, token)) {
        token->type = TOKEN_NUMBER;
        *input = ptr;
        return 1;
    }
    
    if (parse_string(&ptr, token)) {
        token->type = TOKEN_STRING;
        *input = ptr;
        return 1;
    }
    
    if (is_operator_char(*ptr)) {
        if (parse_operator(&ptr, token)) {
            token->type = TOKEN_OPERATOR;
            *input = ptr;
            return 1;
        }
    }
    
    token->type = TOKEN_INVALID;
    token->value[0] = *ptr;
    token->value[1] = '\0';
    ptr++;
    *input = ptr;
    return 0