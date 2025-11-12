//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50
#define DELIMITERS " \t\n\r"

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

int tokenize(const char* input, Token* tokens, int max_tokens) {
    if (!input || !tokens || max_tokens <= 0) return -1;
    
    int token_count = 0;
    const char* pos = input;
    
    while (*pos && token_count < max_tokens) {
        while (*pos && strchr(DELIMITERS, *pos)) pos++;
        if (!*pos) break;
        
        Token* current = &tokens[token_count];
        memset(current->value, 0, MAX_TOKEN_LEN);
        
        if (isdigit(*pos)) {
            current->type = TOKEN_NUMBER;
            int i = 0;
            while (isdigit(*pos) && i < MAX_TOKEN_LEN - 1) {
                current->value[i++] = *pos++;
            }
        } else if (isalpha(*pos) || *pos == '_') {
            current->type = TOKEN_IDENTIFIER;
            int i = 0;
            while ((isalnum(*pos) || *pos == '_') && i < MAX_TOKEN_LEN - 1) {
                current->value[i++] = *pos++;
            }
        } else if (*pos == '"') {
            current->type = TOKEN_STRING;
            int i = 0;
            pos++;
            while (*pos && *pos != '"' && i < MAX_TOKEN_LEN - 1) {
                current->value[i++] = *pos++;
            }
            if (*pos == '"') pos++;
        } else {
            current->type = TOKEN_OPERATOR;
            int i = 0;
            while (!isspace(*pos) && !isalnum(*pos) && *pos != '"' && i < MAX_TOKEN_LEN - 1) {
                current->value[i++] = *pos++;
            }
        }
        
        if (strlen(current->value) > 0) {
            token_count++;
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
        default: printf("UNKNOWN"); break;
    }
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (strlen(input) >= sizeof(input) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];
    int count = tokenize(input, tokens, MAX_TOKENS);
    
    if (count < 0) {
        fprintf(stderr, "Tokenization failed\n");
        return 1;
    }
    
    printf("Tokens found: %d\n", count);
    printf("Type\t\tValue\n");
    printf("----\t\t-----\n");
    
    for (int i = 0; i < count && i < MAX_TOKENS; i++) {
        print_token_type(tokens[i].type);
        printf("\t\t%s\n", tokens[i].value);
    }
    
    return 0;
}