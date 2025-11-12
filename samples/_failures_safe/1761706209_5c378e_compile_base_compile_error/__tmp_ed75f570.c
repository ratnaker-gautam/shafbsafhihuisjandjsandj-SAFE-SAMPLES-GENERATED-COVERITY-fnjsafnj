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

Token* tokenize_expression(const char* input, size_t* token_count) {
    if (!input || !token_count) return NULL;
    
    Token* tokens = malloc(sizeof(Token) * MAX_TOKENS);
    if (!tokens) return NULL;
    
    size_t count = 0;
    const char* ptr = input;
    
    while (*ptr && count < MAX_TOKENS) {
        while (isspace(*ptr)) ptr++;
        if (!*ptr) break;
        
        if (isdigit(*ptr)) {
            size_t len = 0;
            while (isdigit(ptr[len]) && len < MAX_TOKEN_LEN - 1) len++;
            if (len >= MAX_TOKEN_LEN) {
                free(tokens);
                return NULL;
            }
            strncpy(tokens[count].value, ptr, len);
            tokens[count].value[len] = '\0';
            tokens[count].type = TOKEN_NUMBER;
            ptr += len;
            count++;
        } else if (isalpha(*ptr)) {
            size_t len = 0;
            while (isalnum(ptr[len]) && len < MAX_TOKEN_LEN - 1) len++;
            if (len >= MAX_TOKEN_LEN) {
                free(tokens);
                return NULL;
            }
            strncpy(tokens[count].value, ptr, len);
            tokens[count].value[len] = '\0';
            tokens[count].type = TOKEN_IDENTIFIER;
            ptr += len;
            count++;
        } else if (is_operator(*ptr)) {
            tokens[count].value[0] = *ptr;
            tokens[count].value[1] = '\0';
            tokens[count].type = TOKEN_OPERATOR;
            ptr++;
            count++;
        } else if (*ptr == '"') {
            size_t len = 1;
            tokens[count].value[0] = '"';
            ptr++;
            while (*ptr && *ptr != '"' && len < MAX_TOKEN_LEN - 2) {
                tokens[count].value[len++] = *ptr++;
            }
            if (*ptr == '"') {
                tokens[count].value[len++] = '"';
                tokens[count].value[len] = '\0';
                tokens[count].type = TOKEN_STRING;
                ptr++;
                count++;
            } else {
                free(tokens);
                return NULL;
            }
        } else {
            free(tokens);
            return NULL;
        }
    }
    
    if (count < MAX_TOKENS) {
        tokens[count].type = TOKEN_EOF;
        tokens[count].value[0] = '\0';
        count++;
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
    char input[256];
    printf("Enter expression to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
        printf("Input error\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }