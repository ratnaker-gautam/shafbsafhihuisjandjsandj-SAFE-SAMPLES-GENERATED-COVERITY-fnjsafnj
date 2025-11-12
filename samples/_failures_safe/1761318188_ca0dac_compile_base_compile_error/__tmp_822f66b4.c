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
    char *start = *input;
    char *end = start;
    
    while (*end && (isdigit(*end) || *end == '.')) {
        end++;
    }
    
    size_t len = end - start;
    if (len >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    strncpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_NUMBER;
    *input = end;
    return 1;
}

int parse_string(char **input, Token *token) {
    char *start = *input;
    if (*start != '"') return 0;
    
    start++;
    char *end = start;
    
    while (*end && *end != '"') {
        end++;
    }
    
    if (!*end) return 0;
    
    size_t len = end - start;
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

int tokenize(const char *input, Token *tokens, int max_tokens) {
    const char *ptr = input;
    int token_count = 0;
    
    while (*ptr && token_count < max_tokens) {
        while (*ptr && isspace(*ptr)) {
            ptr++;
        }
        
        if (!*ptr) break;
        
        Token token = {TOKEN_UNKNOWN, {0}};
        char *current = (char*)ptr;
        int parsed = 0;
        
        if (*current == '"') {
            parsed = parse_string(&current, &token);
        } else if (isdigit(*current)) {
            parsed = parse_number(&current, &token);
        } else if (is_operator_char(*current)) {
            parsed = parse_operator(&current, &token);
        } else if (isalpha(*current) || *current == '_') {
            parsed = parse_identifier(&current, &token);
        }
        
        if (parsed) {
            tokens[token_count++] = token;
            ptr = current;
        } else {
            token.value[0] = *ptr;
            token.value[1] = '\0';
            token.type = TOKEN_UNKNOWN;
            tokens[token_count++] = token;
            ptr++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        token_count++;
    }
    
    return token_count;
}

void print_token_type(TokenType type) {
    const char *names[] = {
        "IDENTIFIER", "NUMBER", "OPERATOR", "