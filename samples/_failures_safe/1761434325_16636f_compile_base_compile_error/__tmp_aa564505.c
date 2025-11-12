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
    TOKEN_ERROR
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int parse_number(const char *start, const char **end) {
    const char *p = start;
    int has_dot = 0;
    
    while (*p && (isdigit(*p) || (*p == '.' && !has_dot))) {
        if (*p == '.') has_dot = 1;
        p++;
    }
    
    *end = p;
    return p > start;
}

int parse_string(const char *start, const char **end) {
    if (*start != '"') return 0;
    
    const char *p = start + 1;
    while (*p && *p != '"') {
        if (*p == '\\' && *(p + 1)) p++;
        p++;
    }
    
    if (*p == '"') {
        *end = p + 1;
        return 1;
    }
    return 0;
}

int parse_identifier(const char *start, const char **end) {
    if (!isalpha(*start) && *start != '_') return 0;
    
    const char *p = start + 1;
    while (*p && (isalnum(*p) || *p == '_')) p++;
    
    *end = p;
    return 1;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    const char *p = input;
    int token_count = 0;
    
    while (*p && token_count < max_tokens) {
        while (isspace(*p)) p++;
        if (!*p) break;
        
        const char *token_start = p;
        const char *token_end = p;
        
        if (parse_number(p, &token_end)) {
            tokens[token_count].type = TOKEN_NUMBER;
        } else if (parse_string(p, &token_end)) {
            tokens[token_count].type = TOKEN_STRING;
        } else if (parse_identifier(p, &token_end)) {
            tokens[token_count].type = TOKEN_IDENTIFIER;
        } else if (is_operator_char(*p)) {
            token_end = p + 1;
            if ((*p == '&' && *(p + 1) == '&') || 
                (*p == '|' && *(p + 1) == '|') ||
                (*p == '=' && *(p + 1) == '=') ||
                (*p == '!' && *(p + 1) == '=') ||
                (*p == '<' && *(p + 1) == '=') ||
                (*p == '>' && *(p + 1) == '=')) {
                token_end = p + 2;
            }
            tokens[token_count].type = TOKEN_OPERATOR;
        } else {
            tokens[token_count].type = TOKEN_ERROR;
            token_end = p + 1;
        }
        
        size_t token_len = token_end - token_start;
        if (token_len >= MAX_TOKEN_LEN) token_len = MAX_TOKEN_LEN - 1;
        
        strncpy(tokens[token_count].value, token_start, token_len);
        tokens[token_count].value[token_len] = '\0';
        
        token_count++;
        p = token_end;
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "");
        token_count++;
    }
    
    return token_count;
}

const char *token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || len == 1) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    printf("Tokens found: %d\n", token_count);
    printf("%-12s %-20s %s\n", "Type",