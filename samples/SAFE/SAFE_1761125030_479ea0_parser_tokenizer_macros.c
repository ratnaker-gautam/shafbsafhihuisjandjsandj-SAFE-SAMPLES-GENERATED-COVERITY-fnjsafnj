//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: parser_tokenizer
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

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void tokenize(const char* input, Token* tokens, int* token_count) {
    const char* p = input;
    int count = 0;
    
    while (*p && count < MAX_TOKENS - 1) {
        while (*p && strchr(DELIMITERS, *p)) p++;
        
        if (!*p) break;
        
        if (isdigit(*p)) {
            int i = 0;
            while (isdigit(*p) && i < MAX_TOKEN_LEN - 1) {
                tokens[count].value[i++] = *p++;
            }
            tokens[count].value[i] = '\0';
            tokens[count].type = TOKEN_NUMBER;
            count++;
        }
        else if (isalpha(*p) || *p == '_') {
            int i = 0;
            while ((isalnum(*p) || *p == '_') && i < MAX_TOKEN_LEN - 1) {
                tokens[count].value[i++] = *p++;
            }
            tokens[count].value[i] = '\0';
            tokens[count].type = TOKEN_IDENTIFIER;
            count++;
        }
        else if (*p == '"') {
            int i = 0;
            p++;
            while (*p && *p != '"' && i < MAX_TOKEN_LEN - 1) {
                tokens[count].value[i++] = *p++;
            }
            if (*p == '"') p++;
            tokens[count].value[i] = '\0';
            tokens[count].type = TOKEN_STRING;
            count++;
        }
        else if (is_operator_char(*p)) {
            int i = 0;
            while (is_operator_char(*p) && i < MAX_TOKEN_LEN - 1) {
                tokens[count].value[i++] = *p++;
            }
            tokens[count].value[i] = '\0';
            tokens[count].type = TOKEN_OPERATOR;
            count++;
        }
        else {
            p++;
        }
    }
    
    tokens[count].type = TOKEN_EOF;
    tokens[count].value[0] = '\0';
    *token_count = count + 1;
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
    int token_count = 0;
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (strlen(input) >= sizeof(input) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    tokenize(input, tokens, &token_count);
    
    printf("\nTokens found:\n");
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: Type=", i);
        print_token_type(tokens[i].type);
        printf(", Value='%s'\n", tokens[i].value);
    }
    
    return 0;
}