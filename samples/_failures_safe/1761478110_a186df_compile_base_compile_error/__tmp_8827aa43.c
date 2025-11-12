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
    return out - output;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    const char *p = input;
    Token *token = tokens;
    int token_count = 0;
    
    while (*p && token_count < max_tokens) {
        while (isspace((unsigned char)*p)) p++;
        
        if (!*p) break;
        
        if (isalpha((unsigned char)*p) || *p == '_') {
            char *value_ptr = token->value;
            while ((isalnum((unsigned char)*p) || *p == '_') && 
                   (value_ptr - token->value) < MAX_TOKEN_LEN - 1) {
                *value_ptr++ = *p++;
            }
            *value_ptr = '\0';
            token->type = TOKEN_IDENTIFIER;
        }
        else if (isdigit((unsigned char)*p)) {
            char *value_ptr = token->value;
            while (isdigit((unsigned char)*p) && 
                   (value_ptr - token->value) < MAX_TOKEN_LEN - 1) {
                *value_ptr++ = *p++;
            }
            *value_ptr = '\0';
            token->type = TOKEN_NUMBER;
        }
        else if (*p == '"') {
            char *value_ptr = token->value;
            p++;
            while (*p && *p != '"' && 
                   (value_ptr - token->value) < MAX_TOKEN_LEN - 1) {
                *value_ptr++ = *p++;
            }
            if (*p == '"') p++;
            *value_ptr = '\0';
            token->type = TOKEN_STRING;
        }
        else if (is_operator_char(*p)) {
            int len = parse_operator(p, token->value);
            if (len > 0) {
                p += len;
                token->type = TOKEN_OPERATOR;
            } else {
                token->value[0] = *p++;
                token->value[1] = '\0';
                token->type = TOKEN_INVALID;
            }
        }
        else {
            token->value[0] = *p++;
            token->value[1] = '\0';
            token->type = TOKEN_INVALID;
        }
        
        token_count++;
        token++;
    }
    
    if (token_count < max_tokens) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
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
        case TOKEN_INVALID: return "INVALID";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input[1024];
    Token tokens[MAX_TOKENS];
    
    printf("Enter expression to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    printf("Tokens found: %d\n", token_count);
    printf("%-12s %-12s %s\n", "Type", "Value", "Position");
    printf("------------ ------------ ----------\n");
    
    for (int i = 0; i < token_count; i++) {
        printf("%-12s