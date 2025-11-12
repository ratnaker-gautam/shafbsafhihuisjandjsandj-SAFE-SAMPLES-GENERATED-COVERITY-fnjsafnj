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
    TOKEN_UNKNOWN
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
    if (!input || !*input || !token) return 0;
    
    char *start = *input;
    char op[3] = {0};
    
    op[0] = *start;
    if (*(start + 1) && is_operator_char(*(start + 1)) && 
        ((*start == '&' && *(start + 1) == '&') ||
         (*start == '|' && *(start + 1) == '|') ||
         (*start == '=' && *(start + 1) == '=') ||
         (*start == '!' && *(start + 1) == '=') ||
         (*start == '<' && *(start + 1) == '=') ||
         (*start == '>' && *(start + 1) == '='))) {
        op[1] = *(start + 1);
        *input += 2;
    } else {
        *input += 1;
    }
    
    strncpy(token->value, op, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

int parse_number(char **input, Token *token) {
    if (!input || !*input || !token) return 0;
    
    char *start = *input;
    char *end = start;
    
    while (*end && (isdigit(*end) || *end == '.')) {
        end++;
    }
    
    size_t len = end - start;
    if (len >= MAX_TOKEN_LEN || len == 0) {
        return 0;
    }
    
    strncpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_NUMBER;
    *input = end;
    return 1;
}

int parse_string(char **input, Token *token) {
    if (!input || !*input || !token) return 0;
    
    char *start = *input;
    if (*start != '"') return 0;
    
    char *end = start + 1;
    while (*end && *end != '"') {
        if (*end == '\\' && *(end + 1)) {
            end += 2;
        } else {
            end++;
        }
    }
    
    if (!*end) return 0;
    
    size_t len = end - start + 1;
    if (len >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    strncpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_STRING;
    *input = end + 1;
    return 1;
}

int parse_identifier(char **input, Token *token) {
    if (!input || !*input || !token) return 0;
    
    char *start = *input;
    if (!isalpha(*start) && *start != '_') return 0;
    
    char *end = start + 1;
    while (*end && (isalnum(*end) || *end == '_')) {
        end++;
    }
    
    size_t len = end - start;
    if (len >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    strncpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_IDENTIFIER;
    *input = end;
    return 1;
}

int tokenize(char *input, Token *tokens, int max_tokens) {
    if (!input || !tokens || max_tokens <= 0) return 0;
    
    int count = 0;
    
    while (*input && count < max_tokens) {
        while (*input && isspace(*input)) {
            input++;
        }
        
        if (!*input) break;
        
        Token token = {TOKEN_UNKNOWN, ""};
        int parsed = 0;
        
        if (*input == '"') {
            parsed = parse_string(&input, &token);
        } else if (isdigit(*input)) {
            parsed = parse_number(&input, &token);
        } else if (is_operator_char(*input)) {
            parsed = parse_operator(&input, &token);
        } else if (isalpha(*input) || *input == '_') {
            parsed = parse_identifier(&input, &token);
        }
        
        if (parsed) {
            tokens[count] = token;
            count++;
        } else {
            if (count < max_tokens) {
                token.value[0] = *input;
                token.value[1] = '\0';
                token