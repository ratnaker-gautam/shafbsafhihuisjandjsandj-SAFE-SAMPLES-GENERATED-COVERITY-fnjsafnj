//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: parser_tokenizer
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

typedef struct {
    Token tokens[MAX_TOKENS];
    int count;
} TokenList;

TokenList* create_token_list(void) {
    TokenList* list = malloc(sizeof(TokenList));
    if (list == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    list->count = 0;
    return list;
}

void free_token_list(TokenList* list) {
    free(list);
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void add_token(TokenList* list, TokenType type, const char* value) {
    if (list->count >= MAX_TOKENS) {
        fprintf(stderr, "Too many tokens\n");
        return;
    }
    if (strlen(value) >= MAX_TOKEN_LEN) {
        fprintf(stderr, "Token too long\n");
        return;
    }
    Token* token = &list->tokens[list->count];
    token->type = type;
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
    list->count++;
}

TokenList* tokenize(const char* input) {
    TokenList* tokens = create_token_list();
    const char* p = input;
    
    while (*p != '\0') {
        if (isspace((unsigned char)*p)) {
            p++;
            continue;
        }
        
        if (isdigit((unsigned char)*p)) {
            char buffer[MAX_TOKEN_LEN] = {0};
            int i = 0;
            while (isdigit((unsigned char)*p) && i < MAX_TOKEN_LEN - 1) {
                buffer[i++] = *p++;
            }
            add_token(tokens, TOKEN_NUMBER, buffer);
            continue;
        }
        
        if (isalpha((unsigned char)*p) || *p == '_') {
            char buffer[MAX_TOKEN_LEN] = {0};
            int i = 0;
            while ((isalnum((unsigned char)*p) || *p == '_') && i < MAX_TOKEN_LEN - 1) {
                buffer[i++] = *p++;
            }
            add_token(tokens, TOKEN_IDENTIFIER, buffer);
            continue;
        }
        
        if (*p == '"') {
            char buffer[MAX_TOKEN_LEN] = {0};
            int i = 0;
            p++;
            while (*p != '"' && *p != '\0' && i < MAX_TOKEN_LEN - 1) {
                buffer[i++] = *p++;
            }
            if (*p == '"') {
                p++;
                add_token(tokens, TOKEN_STRING, buffer);
            } else {
                fprintf(stderr, "Unterminated string\n");
            }
            continue;
        }
        
        if (is_operator_char(*p)) {
            char buffer[2] = {*p, '\0'};
            add_token(tokens, TOKEN_OPERATOR, buffer);
            p++;
            continue;
        }
        
        fprintf(stderr, "Invalid character: %c\n", *p);
        p++;
    }
    
    add_token(tokens, TOKEN_EOF, "");
    return tokens;
}

const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

void print_tokens(const TokenList* tokens) {
    printf("Tokens found:\n");
    for (int i = 0; i < tokens->count; i++) {
        const Token* token = &tokens->tokens[i];
        printf("  %s: '%s'\n", token_type_to_string(token->type), token->value);
    }
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Failed to read input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return EXIT_FAILURE;
    }
    
    TokenList* tokens = tokenize(input);
    print_tokens(tokens);
    free_token_list(tokens);
    
    return EXIT_SUCCESS;
}