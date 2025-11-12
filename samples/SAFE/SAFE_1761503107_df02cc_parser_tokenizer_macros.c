//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64
#define DELIMITERS " \t\n\r"

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
    return strchr("+-*/=<>!&|", c) != NULL;
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    if (!input || !tokens || max_tokens <= 0) return -1;
    
    int token_count = 0;
    const char* pos = input;
    
    while (*pos && token_count < max_tokens) {
        while (isspace((unsigned char)*pos)) pos++;
        if (!*pos) break;
        
        Token* current = &tokens[token_count];
        memset(current->value, 0, MAX_TOKEN_LEN);
        
        if (isalpha((unsigned char)*pos) || *pos == '_') {
            current->type = TOKEN_IDENTIFIER;
            int i = 0;
            while ((isalnum((unsigned char)*pos) || *pos == '_') && i < MAX_TOKEN_LEN - 1) {
                current->value[i++] = *pos++;
            }
            current->value[i] = '\0';
            token_count++;
        }
        else if (isdigit((unsigned char)*pos)) {
            current->type = TOKEN_NUMBER;
            int i = 0;
            while (isdigit((unsigned char)*pos) && i < MAX_TOKEN_LEN - 1) {
                current->value[i++] = *pos++;
            }
            current->value[i] = '\0';
            token_count++;
        }
        else if (is_operator_char(*pos)) {
            current->type = TOKEN_OPERATOR;
            int i = 0;
            while (is_operator_char(*pos) && i < MAX_TOKEN_LEN - 1) {
                current->value[i++] = *pos++;
            }
            current->value[i] = '\0';
            token_count++;
        }
        else if (*pos == '"') {
            current->type = TOKEN_STRING;
            int i = 0;
            pos++;
            while (*pos && *pos != '"' && i < MAX_TOKEN_LEN - 1) {
                current->value[i++] = *pos++;
            }
            current->value[i] = '\0';
            if (*pos == '"') pos++;
            else current->type = TOKEN_ERROR;
            token_count++;
        }
        else {
            current->type = TOKEN_ERROR;
            current->value[0] = *pos++;
            current->value[1] = '\0';
            token_count++;
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
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_EOF: printf("EOF"); break;
        case TOKEN_ERROR: printf("ERROR"); break;
    }
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];
    int count = tokenize(input, tokens, MAX_TOKENS);
    
    if (count < 0) {
        fprintf(stderr, "Tokenization failed\n");
        return 1;
    }
    
    printf("Tokens found: %d\n", count);
    for (int i = 0; i < count; i++) {
        printf("Token %d: ", i + 1);
        print_token_type(tokens[i].type);
        printf(" -> '%s'\n", tokens[i].value);
    }
    
    return 0;
}