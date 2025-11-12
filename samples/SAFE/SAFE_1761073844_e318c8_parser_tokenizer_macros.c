//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64
#define DELIMITERS " \t\n\r"
#define IS_OPERATOR(c) (c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '(' || c == ')')

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_END
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

Token tokens[MAX_TOKENS];
size_t token_count = 0;
size_t current_token = 0;

void tokenize(const char* input) {
    const char* p = input;
    token_count = 0;
    
    while (*p && token_count < MAX_TOKENS) {
        while (*p && strchr(DELIMITERS, *p)) p++;
        if (!*p) break;
        
        if (IS_OPERATOR(*p)) {
            tokens[token_count].type = TOKEN_OPERATOR;
            tokens[token_count].value[0] = *p;
            tokens[token_count].value[1] = '\0';
            token_count++;
            p++;
            continue;
        }
        
        if (isdigit(*p)) {
            size_t len = 0;
            while (isdigit(*p) && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *p++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
            continue;
        }
        
        if (isalpha(*p) || *p == '_') {
            size_t len = 0;
            while ((isalnum(*p) || *p == '_') && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *p++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            token_count++;
            continue;
        }
        
        p++;
    }
    
    tokens[token_count].type = TOKEN_END;
}

Token* next_token(void) {
    if (current_token >= token_count) {
        return NULL;
    }
    return &tokens[current_token++];
}

void reset_parser(void) {
    current_token = 0;
}

void print_tokens(void) {
    for (size_t i = 0; i < token_count; i++) {
        const char* type_str = "UNKNOWN";
        switch (tokens[i].type) {
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case TOKEN_IDENTIFIER: type_str = "IDENTIFIER"; break;
            case TOKEN_OPERATOR: type_str = "OPERATOR"; break;
            case TOKEN_END: type_str = "END"; break;
        }
        printf("Token %zu: %s -> '%s'\n", i, type_str, tokens[i].value);
    }
}

int main(void) {
    char input[1024];
    
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
    
    tokenize(input);
    
    if (token_count == 0) {
        printf("No tokens found\n");
        return 0;
    }
    
    printf("Found %zu tokens:\n", token_count);
    print_tokens();
    
    printf("\nParsing demonstration:\n");
    reset_parser();
    Token* token;
    while ((token = next_token()) != NULL && token->type != TOKEN_END) {
        printf("Parsed: ");
        switch (token->type) {
            case TOKEN_NUMBER:
                printf("Number %s\n", token->value);
                break;
            case TOKEN_IDENTIFIER:
                printf("Identifier %s\n", token->value);
                break;
            case TOKEN_OPERATOR:
                printf("Operator %s\n", token->value);
                break;
            default:
                printf("Unknown\n");
                break;
        }
    }
    
    return 0;
}