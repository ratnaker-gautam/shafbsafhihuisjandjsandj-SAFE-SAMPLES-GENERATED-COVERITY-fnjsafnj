//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

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
    int line;
} Token;

typedef struct {
    Token* tokens;
    size_t capacity;
    size_t size;
} TokenStack;

TokenStack* create_stack(size_t capacity) {
    if (capacity == 0 || capacity > 1000) return NULL;
    TokenStack* stack = malloc(sizeof(TokenStack));
    if (!stack) return NULL;
    stack->tokens = malloc(capacity * sizeof(Token));
    if (!stack->tokens) {
        free(stack);
        return NULL;
    }
    stack->capacity = capacity;
    stack->size = 0;
    return stack;
}

void destroy_stack(TokenStack* stack) {
    if (stack) {
        free(stack->tokens);
        free(stack);
    }
}

int push_token(TokenStack* stack, Token token) {
    if (!stack || stack->size >= stack->capacity) return 0;
    stack->tokens[stack->size] = token;
    stack->size++;
    return 1;
}

int pop_token(TokenStack* stack, Token* result) {
    if (!stack || !result || stack->size == 0) return 0;
    stack->size--;
    *result = stack->tokens[stack->size];
    return 1;
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

Token* tokenize(const char* input, size_t* token_count) {
    if (!input || !token_count) return NULL;
    
    Token* tokens = malloc(MAX_TOKENS * sizeof(Token));
    if (!tokens) return NULL;
    
    size_t count = 0;
    int line = 1;
    const char* ptr = input;
    
    while (*ptr && count < MAX_TOKENS) {
        while (isspace(*ptr)) {
            if (*ptr == '\n') line++;
            ptr++;
        }
        if (!*ptr) break;
        
        Token token = {0};
        token.line = line;
        
        if (isdigit(*ptr)) {
            token.type = TOKEN_NUMBER;
            size_t i = 0;
            while (isdigit(*ptr) && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = *ptr++;
            }
            token.value[i] = '\0';
        }
        else if (isalpha(*ptr) || *ptr == '_') {
            token.type = TOKEN_IDENTIFIER;
            size_t i = 0;
            while ((isalnum(*ptr) || *ptr == '_') && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = *ptr++;
            }
            token.value[i] = '\0';
        }
        else if (is_operator(*ptr)) {
            token.type = TOKEN_OPERATOR;
            token.value[0] = *ptr++;
            token.value[1] = '\0';
        }
        else if (*ptr == '"') {
            token.type = TOKEN_STRING;
            ptr++;
            size_t i = 0;
            while (*ptr != '"' && *ptr && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = *ptr++;
            }
            token.value[i] = '\0';
            if (*ptr == '"') ptr++;
        }
        else {
            ptr++;
            continue;
        }
        
        tokens[count++] = token;
    }
    
    if (count < MAX_TOKENS) {
        Token eof_token = {TOKEN_EOF, "", line};
        tokens[count++] = eof_token;
    }
    
    *token_count = count;
    return tokens;
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
        printf("Input error\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    size_t token_count = 0;
    Token* tokens = tokenize(input, &token_count);
    if (!tokens) {
        printf("Tokenization failed\n");
        return 1;
    }
    
    TokenStack* stack = create_stack(MAX_TOKENS);
    if (!stack) {
        free(tokens);
        printf("Stack creation failed\n");
        return 1;
    }
    
    printf("Tokens:\n");
    for (size_t i = 0; i < token_count; i++) {
        printf("Line %d: ", tokens[i].line);
        print_token_type(tokens[i].type);
        printf(" '