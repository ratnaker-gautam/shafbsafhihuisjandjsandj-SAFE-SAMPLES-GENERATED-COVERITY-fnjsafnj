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
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void tokenize(const char* input, Token* tokens, int* token_count) {
    const char* p = input;
    *token_count = 0;
    
    while (*p && *token_count < MAX_TOKENS) {
        while (*p && strchr(DELIMITERS, *p)) p++;
        if (!*p) break;
        
        Token* token = &tokens[(*token_count)++];
        
        if (isalpha(*p) || *p == '_') {
            int i = 0;
            while ((isalnum(*p) || *p == '_') && i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = *p++;
            }
            token->value[i] = '\0';
            token->type = TOKEN_IDENTIFIER;
        }
        else if (isdigit(*p)) {
            int i = 0;
            while (isdigit(*p) && i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = *p++;
            }
            token->value[i] = '\0';
            token->type = TOKEN_NUMBER;
        }
        else if (*p == '"') {
            int i = 0;
            p++;
            while (*p && *p != '"' && i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = *p++;
            }
            if (*p == '"') p++;
            token->value[i] = '\0';
            token->type = TOKEN_STRING;
        }
        else if (is_operator(*p)) {
            token->value[0] = *p++;
            token->value[1] = '\0';
            token->type = TOKEN_OPERATOR;
        }
        else {
            token->value[0] = *p++;
            token->value[1] = '\0';
            token->type = TOKEN_OPERATOR;
        }
    }
    
    if (*token_count < MAX_TOKENS) {
        tokens[*token_count].type = TOKEN_EOF;
        strcpy(tokens[*token_count].value, "EOF");
        (*token_count)++;
    }
}

void print_tokens(const Token* tokens, int token_count) {
    for (int i = 0; i < token_count; i++) {
        const char* type_str;
        switch (tokens[i].type) {
            case TOKEN_IDENTIFIER: type_str = "IDENTIFIER"; break;
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case TOKEN_OPERATOR: type_str = "OPERATOR"; break;
            case TOKEN_STRING: type_str = "STRING"; break;
            case TOKEN_EOF: type_str = "EOF"; break;
            default: type_str = "UNKNOWN"; break;
        }
        printf("Token %d: %s -> '%s'\n", i, type_str, tokens[i].value);
    }
}

int main() {
    char input[1024];
    Token tokens[MAX_TOKENS];
    int token_count = 0;
    
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
    
    tokenize(input, tokens, &token_count);
    
    if (token_count >= MAX_TOKENS) {
        fprintf(stderr, "Too many tokens\n");
        return 1;
    }
    
    printf("\nTokenized output:\n");
    print_tokens(tokens, token_count);
    
    return 0;
}