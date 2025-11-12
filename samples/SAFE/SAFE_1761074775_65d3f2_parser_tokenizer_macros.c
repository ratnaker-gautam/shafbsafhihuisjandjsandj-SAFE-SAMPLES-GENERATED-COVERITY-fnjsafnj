//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50
#define DELIMITERS " \t\n"

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

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

Token* tokenize(const char* input, int* token_count) {
    static Token tokens[MAX_TOKENS];
    *token_count = 0;
    const char* ptr = input;
    
    while (*ptr && *token_count < MAX_TOKENS) {
        while (*ptr && strchr(DELIMITERS, *ptr)) ptr++;
        if (!*ptr) break;
        
        Token* token = &tokens[(*token_count)++];
        
        if (isdigit(*ptr)) {
            token->type = TOKEN_NUMBER;
            int i = 0;
            while (isdigit(*ptr) && i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = *ptr++;
            }
            token->value[i] = '\0';
        }
        else if (isalpha(*ptr)) {
            token->type = TOKEN_IDENTIFIER;
            int i = 0;
            while ((isalnum(*ptr) || *ptr == '_') && i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = *ptr++;
            }
            token->value[i] = '\0';
        }
        else if (*ptr == '"') {
            token->type = TOKEN_STRING;
            ptr++;
            int i = 0;
            while (*ptr && *ptr != '"' && i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = *ptr++;
            }
            token->value[i] = '\0';
            if (*ptr == '"') ptr++;
        }
        else if (is_operator(*ptr)) {
            token->type = TOKEN_OPERATOR;
            token->value[0] = *ptr++;
            token->value[1] = '\0';
        }
        else {
            ptr++;
            (*token_count)--;
        }
    }
    
    if (*token_count < MAX_TOKENS) {
        tokens[*token_count].type = TOKEN_EOF;
        strcpy(tokens[*token_count].value, "EOF");
        (*token_count)++;
    }
    
    return tokens;
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

int main() {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (strlen(input) >= sizeof(input) - 1 && input[strlen(input) - 1] != '\n') {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int token_count;
    Token* tokens = tokenize(input, &token_count);
    
    printf("Tokens found: %d\n", token_count);
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: Type=", i + 1);
        print_token_type(tokens[i].type);
        printf(", Value='%s'\n", tokens[i].value);
    }
    
    return 0;
}