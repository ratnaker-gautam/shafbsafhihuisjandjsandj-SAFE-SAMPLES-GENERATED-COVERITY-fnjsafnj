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

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

int add_token(TokenList* list, TokenType type, const char* value) {
    if (list->count >= MAX_TOKENS) {
        fprintf(stderr, "Too many tokens\n");
        return 0;
    }
    if (strlen(value) >= MAX_TOKEN_LEN) {
        fprintf(stderr, "Token too long\n");
        return 0;
    }
    Token* token = &list->tokens[list->count];
    token->type = type;
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
    list->count++;
    return 1;
}

TokenList* tokenize(const char* input) {
    if (input == NULL) {
        fprintf(stderr, "Input is NULL\n");
        return NULL;
    }
    
    TokenList* list = create_token_list();
    int i = 0;
    int len = strlen(input);
    
    while (i < len && list->count < MAX_TOKENS) {
        if (isspace(input[i])) {
            i++;
            continue;
        }
        
        if (isdigit(input[i])) {
            char number[MAX_TOKEN_LEN] = {0};
            int j = 0;
            while (i < len && isdigit(input[i]) && j < MAX_TOKEN_LEN - 1) {
                number[j++] = input[i++];
            }
            number[j] = '\0';
            if (!add_token(list, TOKEN_NUMBER, number)) {
                free_token_list(list);
                return NULL;
            }
        }
        else if (isalpha(input[i])) {
            char identifier[MAX_TOKEN_LEN] = {0};
            int j = 0;
            while (i < len && (isalnum(input[i]) || input[i] == '_') && j < MAX_TOKEN_LEN - 1) {
                identifier[j++] = input[i++];
            }
            identifier[j] = '\0';
            if (!add_token(list, TOKEN_IDENTIFIER, identifier)) {
                free_token_list(list);
                return NULL;
            }
        }
        else if (is_operator(input[i])) {
            char op[2] = {input[i++], '\0'};
            if (!add_token(list, TOKEN_OPERATOR, op)) {
                free_token_list(list);
                return NULL;
            }
        }
        else {
            fprintf(stderr, "Invalid character: %c\n", input[i]);
            free_token_list(list);
            return NULL;
        }
    }
    
    if (!add_token(list, TOKEN_EOF, "")) {
        free_token_list(list);
        return NULL;
    }
    
    return list;
}

void print_tokens(const TokenList* list) {
    if (list == NULL) {
        fprintf(stderr, "Token list is NULL\n");
        return;
    }
    
    for (int i = 0; i < list->count; i++) {
        const Token* token = &list->tokens[i];
        switch (token->type) {
            case TOKEN_NUMBER:
                printf("NUMBER: %s\n", token->value);
                break;
            case TOKEN_IDENTIFIER:
                printf("IDENTIFIER: %s\n", token->value);
                break;
            case TOKEN_OPERATOR:
                printf("OPERATOR: %s\n", token->value);
                break;
            case TOKEN_EOF:
                printf("EOF\n");
                break;
        }
    }
}

int main(void) {
    char input[256];
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
    if (tokens == NULL) {
        return EXIT_FAILURE;
    }
    
    printf("Tokens:\n");
    print_tokens(tokens);
    
    free_token_list(tokens);
    return EXIT_SUCCESS;
}