//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    const char *p = input;
    int token_count = 0;
    
    while (*p && token_count < max_tokens) {
        while (*p && isspace(*p)) {
            p++;
        }
        
        if (!*p) break;
        
        if (isdigit(*p)) {
            char *token_ptr = tokens[token_count].value;
            while (*p && isdigit(*p) && (token_ptr - tokens[token_count].value) < MAX_TOKEN_LEN - 1) {
                *token_ptr++ = *p++;
            }
            *token_ptr = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
        } else if (isalpha(*p) || *p == '_') {
            char *token_ptr = tokens[token_count].value;
            while (*p && (isalnum(*p) || *p == '_') && (token_ptr - tokens[token_count].value) < MAX_TOKEN_LEN - 1) {
                *token_ptr++ = *p++;
            }
            *token_ptr = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            token_count++;
        } else if (*p == '"') {
            char *token_ptr = tokens[token_count].value;
            p++;
            while (*p && *p != '"' && (token_ptr - tokens[token_count].value) < MAX_TOKEN_LEN - 1) {
                *token_ptr++ = *p++;
            }
            if (*p == '"') {
                p++;
            }
            *token_ptr = '\0';
            tokens[token_count].type = TOKEN_STRING;
            token_count++;
        } else if (is_operator_char(*p)) {
            char *token_ptr = tokens[token_count].value;
            *token_ptr++ = *p++;
            if (is_operator_char(*p) && (token_ptr - tokens[token_count].value) < MAX_TOKEN_LEN - 1) {
                *token_ptr++ = *p++;
            }
            *token_ptr = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
            token_count++;
        } else {
            p++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "");
        token_count++;
    }
    
    return token_count;
}

void print_token_type(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_EOF: printf("EOF"); break;
    }
}

int main(void) {
    char input[1024];
    Token tokens[MAX_TOKENS];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
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
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count >= MAX_TOKENS) {
        fprintf(stderr, "Too many tokens\n");
        return 1;
    }
    
    printf("Tokens found:\n");
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: Type=", i);
        print_token_type(tokens[i].type);
        printf(", Value='%s'\n", tokens[i].value);
    }
    
    return 0;
}