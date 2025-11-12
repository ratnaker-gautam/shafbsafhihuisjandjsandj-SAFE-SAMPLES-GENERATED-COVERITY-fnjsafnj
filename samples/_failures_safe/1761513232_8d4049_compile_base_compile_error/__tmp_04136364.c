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
    Token* tokens;
    size_t capacity;
    size_t count;
} TokenStack;

TokenStack* create_stack(size_t capacity) {
    if (capacity == 0 || capacity > MAX_TOKENS) return NULL;
    TokenStack* stack = malloc(sizeof(TokenStack));
    if (!stack) return NULL;
    stack->tokens = malloc(sizeof(Token) * capacity);
    if (!stack->tokens) {
        free(stack);
        return NULL;
    }
    stack->capacity = capacity;
    stack->count = 0;
    return stack;
}

void destroy_stack(TokenStack* stack) {
    if (stack) {
        free(stack->tokens);
        free(stack);
    }
}

int push_token(TokenStack* stack, Token token) {
    if (!stack || stack->count >= stack->capacity) return 0;
    if (strlen(token.value) >= MAX_TOKEN_LEN) return 0;
    stack->tokens[stack->count] = token;
    stack->count++;
    return 1;
}

int pop_token(TokenStack* stack, Token* result) {
    if (!stack || !result || stack->count == 0) return 0;
    stack->count--;
    *result = stack->tokens[stack->count];
    return 1;
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenType classify_token(const char* str) {
    if (str[0] == '\0') return TOKEN_EOF;
    if (isdigit(str[0])) return TOKEN_NUMBER;
    if (str[0] == '"') return TOKEN_STRING;
    if (is_operator(str[0])) return TOKEN_OPERATOR;
    if (isalpha(str[0]) || str[0] == '_') return TOKEN_IDENTIFIER;
    return TOKEN_EOF;
}

int tokenize_expression(const char* input, Token** tokens, size_t* token_count) {
    if (!input || !tokens || !token_count) return 0;
    
    size_t capacity = 10;
    *tokens = malloc(sizeof(Token) * capacity);
    if (!*tokens) return 0;
    
    *token_count = 0;
    size_t i = 0;
    
    while (input[i] != '\0') {
        while (isspace(input[i])) i++;
        if (input[i] == '\0') break;
        
        if (*token_count >= capacity) {
            if (capacity >= MAX_TOKENS) {
                free(*tokens);
                return 0;
            }
            size_t new_capacity = capacity * 2;
            if (new_capacity > MAX_TOKENS) new_capacity = MAX_TOKENS;
            Token* new_tokens = realloc(*tokens, sizeof(Token) * new_capacity);
            if (!new_tokens) {
                free(*tokens);
                return 0;
            }
            *tokens = new_tokens;
            capacity = new_capacity;
        }
        
        Token token;
        size_t j = 0;
        
        if (isdigit(input[i])) {
            token.type = TOKEN_NUMBER;
            while (isdigit(input[i]) && j < MAX_TOKEN_LEN - 1) {
                token.value[j++] = input[i++];
            }
        } else if (isalpha(input[i]) || input[i] == '_') {
            token.type = TOKEN_IDENTIFIER;
            while ((isalnum(input[i]) || input[i] == '_') && j < MAX_TOKEN_LEN - 1) {
                token.value[j++] = input[i++];
            }
        } else if (input[i] == '"') {
            token.type = TOKEN_STRING;
            token.value[j++] = input[i++];
            while (input[i] != '"' && input[i] != '\0' && j < MAX_TOKEN_LEN - 1) {
                token.value[j++] = input[i++];
            }
            if (input[i] == '"') {
                token.value[j++] = input[i++];
            }
        } else if (is_operator(input[i])) {
            token.type = TOKEN_OPERATOR;
            token.value[j++] = input[i++];
        } else {
            i++;
            continue;
        }
        
        token.value[j] = '\0';
        (*tokens)[(*token_count)++] = token;
    }
    
    return 1;
}

void print_token(const Token* token) {
    if (!token) return;
    const char* type_names[] = {"NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF"};
    printf("Type: %-12s Value: %s\n", type_names[token->type], token->value);
}

int main(void) {
    char input[256];
    printf("Enter expression to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
        printf("Input error\n");
        return 1;
    }
    
    size