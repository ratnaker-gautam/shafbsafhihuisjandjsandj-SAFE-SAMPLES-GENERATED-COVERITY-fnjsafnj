//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
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
    if (input == NULL) return;
    
    const char* p = input;
    token_count = 0;
    
    while (*p != '\0' && token_count < MAX_TOKENS) {
        while (isspace((unsigned char)*p)) p++;
        if (*p == '\0') break;
        
        if (IS_OPERATOR(*p)) {
            if (token_count < MAX_TOKENS) {
                tokens[token_count].type = TOKEN_OPERATOR;
                tokens[token_count].value[0] = *p;
                tokens[token_count].value[1] = '\0';
                token_count++;
            }
            p++;
            continue;
        }
        
        if (isdigit((unsigned char)*p)) {
            size_t len = 0;
            while (isdigit((unsigned char)*p) && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *p++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
            continue;
        }
        
        if (isalpha((unsigned char)*p) || *p == '_') {
            size_t len = 0;
            while ((isalnum((unsigned char)*p) || *p == '_') && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *p++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            token_count++;
            continue;
        }
        
        p++;
    }
    
    if (token_count < MAX_TOKENS) {
        tokens[token_count].type = TOKEN_END;
        tokens[token_count].value[0] = '\0';
        token_count++;
    }
}

Token get_next_token(void) {
    if (current_token < token_count) {
        return tokens[current_token++];
    }
    Token end_token = {TOKEN_END, ""};
    return end_token;
}

void parse_expression(void) {
    Token token = get_next_token();
    
    while (token.type != TOKEN_END) {
        switch (token.type) {
            case TOKEN_NUMBER:
                printf("Number: %s\n", token.value);
                break;
            case TOKEN_IDENTIFIER:
                printf("Identifier: %s\n", token.value);
                break;
            case TOKEN_OPERATOR:
                printf("Operator: %s\n", token.value);
                break;
            case TOKEN_END:
                break;
        }
        token = get_next_token();
    }
}

int main(void) {
    char input[1024];
    
    printf("Enter expression to parse: ");
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
        fprintf(stderr, "No tokens found\n");
        return 1;
    }
    
    printf("Parsed tokens:\n");
    parse_expression();
    
    return 0;
}