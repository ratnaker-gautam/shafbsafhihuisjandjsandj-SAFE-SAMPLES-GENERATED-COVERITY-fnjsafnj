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

int add_token(TokenList* list, TokenType type, const char* value) {
    if (list->count >= MAX_TOKENS) {
        return 0;
    }
    if (strlen(value) >= MAX_TOKEN_LEN) {
        return 0;
    }
    Token* token = &list->tokens[list->count];
    token->type = type;
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
    list->count++;
    return 1;
}

void tokenize_string(const char* input, TokenList* tokens) {
    int i = 0;
    int len = strlen(input);
    
    while (i < len) {
        if (isspace((unsigned char)input[i])) {
            i++;
            continue;
        }
        
        if (isdigit((unsigned char)input[i])) {
            char number[MAX_TOKEN_LEN] = {0};
            int j = 0;
            while (i < len && j < MAX_TOKEN_LEN - 1 && isdigit((unsigned char)input[i])) {
                number[j++] = input[i++];
            }
            if (!add_token(tokens, TOKEN_NUMBER, number)) {
                fprintf(stderr, "Too many tokens or token too long\n");
                return;
            }
            continue;
        }
        
        if (isalpha((unsigned char)input[i]) || input[i] == '_') {
            char identifier[MAX_TOKEN_LEN] = {0};
            int j = 0;
            while (i < len && j < MAX_TOKEN_LEN - 1 && 
                   (isalnum((unsigned char)input[i]) || input[i] == '_')) {
                identifier[j++] = input[i++];
            }
            if (!add_token(tokens, TOKEN_IDENTIFIER, identifier)) {
                fprintf(stderr, "Too many tokens or token too long\n");
                return;
            }
            continue;
        }
        
        if (input[i] == '"') {
            char string[MAX_TOKEN_LEN] = {0};
            int j = 0;
            i++;
            while (i < len && j < MAX_TOKEN_LEN - 1 && input[i] != '"') {
                string[j++] = input[i++];
            }
            if (i < len && input[i] == '"') {
                i++;
                if (!add_token(tokens, TOKEN_STRING, string)) {
                    fprintf(stderr, "Too many tokens or token too long\n");
                    return;
                }
            } else {
                fprintf(stderr, "Unterminated string literal\n");
                return;
            }
            continue;
        }
        
        if (is_operator_char(input[i])) {
            char op[2] = {input[i], '\0'};
            i++;
            if (!add_token(tokens, TOKEN_OPERATOR, op)) {
                fprintf(stderr, "Too many tokens or token too long\n");
                return;
            }
            continue;
        }
        
        fprintf(stderr, "Invalid character: %c\n", input[i]);
        i++;
    }
    
    add_token(tokens, TOKEN_EOF, "");
}

void print_tokens(const TokenList* tokens) {
    const char* type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF"
    };
    
    for (int i = 0; i < tokens->count; i++) {
        printf("Token %d: %s -> '%s'\n", i, type_names[tokens->tokens[i].type], 
               tokens->tokens[i].value);
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
    
    TokenList* tokens = create_token_list();
    tokenize_string(input, tokens);
    
    if (tokens->count > 0) {
        print_tokens(tokens);
    }
    
    free