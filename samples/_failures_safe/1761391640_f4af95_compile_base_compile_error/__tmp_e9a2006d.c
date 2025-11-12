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
    const char *p = input;
    char *out = output;
    
    if (!is_operator_char(*p)) return 0;
    
    *out++ = *p++;
    
    if ((*p == '=' && (*(p-1) == '=' || *(p-1) == '!' || *(p-1) == '<' || *(p-1) == '>')) ||
        (*p == '&' && *(p-1) == '&') ||
        (*p == '|' && *(p-1) == '|')) {
        *out++ = *p++;
    }
    
    *out = '\0';
    return (int)(p - input);
}

int parse_number(const char *input, char *output) {
    const char *p = input;
    char *out = output;
    
    if (!isdigit(*p)) return 0;
    
    while (isdigit(*p)) {
        if ((size_t)(out - output) >= MAX_TOKEN_LEN - 1) break;
        *out++ = *p++;
    }
    
    *out = '\0';
    return (int)(p - input);
}

int parse_string(const char *input, char *output) {
    const char *p = input;
    char *out = output;
    
    if (*p != '"') return 0;
    
    *out++ = *p++;
    
    while (*p && *p != '"') {
        if ((size_t)(out - output) >= MAX_TOKEN_LEN - 1) break;
        *out++ = *p++;
    }
    
    if (*p == '"') {
        *out++ = *p++;
    }
    
    *out = '\0';
    return (int)(p - input);
}

int parse_identifier(const char *input, char *output) {
    const char *p = input;
    char *out = output;
    
    if (!isalpha(*p) && *p != '_') return 0;
    
    *out++ = *p++;
    
    while (isalnum(*p) || *p == '_') {
        if ((size_t)(out - output) >= MAX_TOKEN_LEN - 1) break;
        *out++ = *p++;
    }
    
    *out = '\0';
    return (int)(p - input);
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    const char *p = input;
    Token *current = tokens;
    int token_count = 0;
    
    while (*p && token_count < max_tokens) {
        while (isspace(*p)) p++;
        
        if (!*p) break;
        
        int consumed = 0;
        
        consumed = parse_operator(p, current->value);
        if (consumed > 0) {
            current->type = TOKEN_OPERATOR;
            p += consumed;
            current++;
            token_count++;
            continue;
        }
        
        consumed = parse_number(p, current->value);
        if (consumed > 0) {
            current->type = TOKEN_NUMBER;
            p += consumed;
            current++;
            token_count++;
            continue;
        }
        
        consumed = parse_string(p, current->value);
        if (consumed > 0) {
            current->type = TOKEN_STRING;
            p += consumed;
            current++;
            token_count++;
            continue;
        }
        
        consumed = parse_identifier(p, current->value);
        if (consumed > 0) {
            current->type = TOKEN_IDENTIFIER;
            p += consumed;
            current++;
            token_count++;
            continue;
        }
        
        current->type = TOKEN_INVALID;
        current->value[0] = *p;
        current->value[1] = '\0';
        p++;
        current++;
        token_count++;
    }
    
    if (token_count < max_tokens) {
        current->type = TOKEN_EOF;
        strcpy(current->value, "EOF");
        token_count++;
    }
    
    return token_count;
}

void print_token_type(TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_EOF: printf("EOF"); break;
        case TOKEN_INVALID: printf("INVALID"); break;
    }
}

int main(void) {
    char input[1024];
    Token tokens[MAX_TOKENS];
    
    printf("Enter expression to tokenize: ");
    if (!fgets(input, sizeof(input), stdin