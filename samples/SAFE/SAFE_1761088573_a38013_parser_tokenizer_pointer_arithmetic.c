//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    const char *p = input;
    int token_count = 0;
    
    while (*p && token_count < max_tokens) {
        while (isspace(*p)) p++;
        
        if (!*p) break;
        
        if (isdigit(*p)) {
            char *start = (char *)p;
            while (isdigit(*p)) p++;
            size_t len = p - start;
            if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
            tokens[token_count].type = TOKEN_NUMBER;
            strncpy(tokens[token_count].value, start, len);
            tokens[token_count].value[len] = '\0';
            token_count++;
        }
        else if (isalpha(*p) || *p == '_') {
            char *start = (char *)p;
            while (isalnum(*p) || *p == '_') p++;
            size_t len = p - start;
            if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
            tokens[token_count].type = TOKEN_IDENTIFIER;
            strncpy(tokens[token_count].value, start, len);
            tokens[token_count].value[len] = '\0';
            token_count++;
        }
        else if (is_operator_char(*p)) {
            char *start = (char *)p;
            while (is_operator_char(*p)) p++;
            size_t len = p - start;
            if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
            tokens[token_count].type = TOKEN_OPERATOR;
            strncpy(tokens[token_count].value, start, len);
            tokens[token_count].value[len] = '\0';
            token_count++;
        }
        else if (*p == '"') {
            char *start = (char *)p;
            p++;
            while (*p && *p != '"') p++;
            if (*p == '"') p++;
            size_t len = p - start;
            if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
            tokens[token_count].type = TOKEN_STRING;
            strncpy(tokens[token_count].value, start, len);
            tokens[token_count].value[len] = '\0';
            token_count++;
        }
        else {
            tokens[token_count].type = TOKEN_INVALID;
            tokens[token_count].value[0] = *p;
            tokens[token_count].value[1] = '\0';
            token_count++;
            p++;
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
    
    printf("Tokens found: %d\n", token_count);
    printf("Type\t\tValue\n");
    printf("----\t\t-----\n");
    
    for (int i = 0; i < token_count; i++) {
        print_token_type(tokens[i].type);
        printf("\t\t%s\n", tokens[i].value);
    }
    
    return 0;
}