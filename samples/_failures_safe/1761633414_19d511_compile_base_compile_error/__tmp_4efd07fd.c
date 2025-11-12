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

int parse_operator(const char *input, char *output) {
    const char *p = input;
    char *out = output;
    
    if (*p == '\0') return 0;
    
    if ((*p == '&' && *(p + 1) == '&') || 
        (*p == '|' && *(p + 1) == '|') ||
        (*p == '=' && *(p + 1) == '=') ||
        (*p == '!' && *(p + 1) == '=') ||
        (*p == '<' && *(p + 1) == '=') ||
        (*p == '>' && *(p + 1) == '=')) {
        if (out) {
            *out++ = *p++;
            *out++ = *p;
        }
        return 2;
    }
    
    if (is_operator_char(*p)) {
        if (out) *out = *p;
        return 1;
    }
    
    return 0;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    const char *p = input;
    Token *current = tokens;
    int token_count = 0;
    
    if (!input || !tokens || max_tokens <= 0) return -1;
    
    while (*p != '\0' && token_count < max_tokens) {
        while (isspace((unsigned char)*p)) p++;
        
        if (*p == '\0') break;
        
        if (isalpha((unsigned char)*p) || *p == '_') {
            char *val_ptr = current->value;
            int len = 0;
            
            while ((isalnum((unsigned char)*p) || *p == '_') && len < MAX_TOKEN_LEN - 1) {
                *val_ptr++ = *p++;
                len++;
            }
            *val_ptr = '\0';
            current->type = TOKEN_IDENTIFIER;
        }
        else if (isdigit((unsigned char)*p)) {
            char *val_ptr = current->value;
            int len = 0;
            
            while (isdigit((unsigned char)*p) && len < MAX_TOKEN_LEN - 1) {
                *val_ptr++ = *p++;
                len++;
            }
            *val_ptr = '\0';
            current->type = TOKEN_NUMBER;
        }
        else if (*p == '"') {
            char *val_ptr = current->value;
            int len = 0;
            
            p++;
            while (*p != '"' && *p != '\0' && len < MAX_TOKEN_LEN - 1) {
                *val_ptr++ = *p++;
                len++;
            }
            *val_ptr = '\0';
            
            if (*p == '"') p++;
            current->type = TOKEN_STRING;
        }
        else {
            int op_len = parse_operator(p, current->value);
            if (op_len > 0) {
                current->value[op_len] = '\0';
                current->type = TOKEN_OPERATOR;
                p += op_len;
            }
            else {
                current->value[0] = *p;
                current->value[1] = '\0';
                current->type = TOKEN_UNKNOWN;
                p++;
            }
        }
        
        current++;
        token_count++;
    }
    
    if (token_count < max_tokens) {
        current->type = TOKEN_EOF;
        current->value[0] = '\0';
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
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int count = tokenize(input, tokens, MAX_TOKENS);
    if (count < 0) {
        fprintf(stderr, "Tokenization failed\n");
        return