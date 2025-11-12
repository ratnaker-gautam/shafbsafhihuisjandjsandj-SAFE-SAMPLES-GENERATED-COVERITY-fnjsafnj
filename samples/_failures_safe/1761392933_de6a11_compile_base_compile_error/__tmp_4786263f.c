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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(char **input) {
    while (**input && isspace(**input)) {
        (*input)++;
    }
}

TokenType classify_token(const char *value) {
    if (isdigit(*value)) return TOKEN_NUMBER;
    if (isalpha(*value)) return TOKEN_IDENTIFIER;
    if (is_operator_char(*value)) return TOKEN_OPERATOR;
    if (*value == '"') return TOKEN_STRING;
    return TOKEN_INVALID;
}

int parse_string(char **input, char *output) {
    char *start = *input;
    if (*start != '"') return 0;
    start++;
    
    char *dest = output;
    while (*start && *start != '"') {
        if (dest - output >= MAX_TOKEN_LEN - 1) return 0;
        *dest++ = *start++;
    }
    
    if (*start != '"') return 0;
    *dest = '\0';
    *input = start + 1;
    return 1;
}

int tokenize(char *input, Token *tokens, int max_tokens) {
    Token *current = tokens;
    char *ptr = input;
    
    while (*ptr && (current - tokens) < max_tokens) {
        skip_whitespace(&ptr);
        if (!*ptr) break;
        
        char *token_start = ptr;
        TokenType type = classify_token(ptr);
        
        if (type == TOKEN_STRING) {
            current->type = TOKEN_STRING;
            if (!parse_string(&ptr, current->value)) {
                current->type = TOKEN_INVALID;
                strncpy(current->value, "INVALID_STRING", MAX_TOKEN_LEN - 1);
                current->value[MAX_TOKEN_LEN - 1] = '\0';
            }
            current++;
            continue;
        }
        
        if (type == TOKEN_NUMBER) {
            current->type = TOKEN_NUMBER;
            char *dest = current->value;
            while (*ptr && isdigit(*ptr) && (dest - current->value) < MAX_TOKEN_LEN - 1) {
                *dest++ = *ptr++;
            }
            *dest = '\0';
            current++;
            continue;
        }
        
        if (type == TOKEN_IDENTIFIER) {
            current->type = TOKEN_IDENTIFIER;
            char *dest = current->value;
            while (*ptr && (isalnum(*ptr) || *ptr == '_') && (dest - current->value) < MAX_TOKEN_LEN - 1) {
                *dest++ = *ptr++;
            }
            *dest = '\0';
            current++;
            continue;
        }
        
        if (type == TOKEN_OPERATOR) {
            current->type = TOKEN_OPERATOR;
            char *dest = current->value;
            while (*ptr && is_operator_char(*ptr) && (dest - current->value) < MAX_TOKEN_LEN - 1) {
                *dest++ = *ptr++;
            }
            *dest = '\0';
            current++;
            continue;
        }
        
        current->type = TOKEN_INVALID;
        current->value[0] = *ptr;
        current->value[1] = '\0';
        current++;
        ptr++;
    }
    
    if ((current - tokens) < max_tokens) {
        current->type = TOKEN_EOF;
        current->value[0] = '\0';
        current++;
    }
    
    return current - tokens;
}

void print_token_type(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_EOF: printf("EOF"); break;
        case TOKEN_INVALID: printf("INVALID"); break;
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
    
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count >= MAX_TOKENS) {
        fprintf(stderr, "Warning: Input too long, some tokens may be truncated\n");
    }
    
    printf("Tokens found: %d\n", token