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
} Token;

typedef struct {
    Token tokens[MAX_TOKENS];
    size_t count;
    size_t capacity;
} TokenStack;

TokenStack* create_token_stack(size_t capacity) {
    if (capacity == 0 || capacity > MAX_TOKENS) {
        return NULL;
    }
    TokenStack* stack = malloc(sizeof(TokenStack));
    if (!stack) {
        return NULL;
    }
    stack->count = 0;
    stack->capacity = capacity;
    return stack;
}

void destroy_token_stack(TokenStack* stack) {
    if (stack) {
        free(stack);
    }
}

int push_token(TokenStack* stack, Token token) {
    if (!stack || stack->count >= stack->capacity) {
        return 0;
    }
    stack->tokens[stack->count] = token;
    stack->count++;
    return 1;
}

int pop_token(TokenStack* stack, Token* token) {
    if (!stack || !token || stack->count == 0) {
        return 0;
    }
    stack->count--;
    *token = stack->tokens[stack->count];
    return 1;
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

int tokenize_expression(const char* input, TokenStack* stack) {
    if (!input || !stack) {
        return 0;
    }
    
    size_t len = strlen(input);
    if (len == 0) {
        return 0;
    }
    
    size_t i = 0;
    while (i < len && stack->count < stack->capacity) {
        while (i < len && isspace(input[i])) {
            i++;
        }
        if (i >= len) {
            break;
        }
        
        Token token;
        memset(&token, 0, sizeof(Token));
        
        if (isdigit(input[i])) {
            token.type = TOKEN_NUMBER;
            size_t j = 0;
            while (i < len && j < MAX_TOKEN_LEN - 1 && isdigit(input[i])) {
                token.value[j++] = input[i++];
            }
            token.value[j] = '\0';
        } else if (isalpha(input[i])) {
            token.type = TOKEN_IDENTIFIER;
            size_t j = 0;
            while (i < len && j < MAX_TOKEN_LEN - 1 && (isalnum(input[i]) || input[i] == '_')) {
                token.value[j++] = input[i++];
            }
            token.value[j] = '\0';
        } else if (input[i] == '"') {
            token.type = TOKEN_STRING;
            size_t j = 0;
            i++;
            while (i < len && j < MAX_TOKEN_LEN - 1 && input[i] != '"') {
                token.value[j++] = input[i++];
            }
            token.value[j] = '\0';
            if (i < len && input[i] == '"') {
                i++;
            }
        } else if (is_operator(input[i])) {
            token.type = TOKEN_OPERATOR;
            token.value[0] = input[i++];
            token.value[1] = '\0';
        } else {
            i++;
            continue;
        }
        
        if (!push_token(stack, token)) {
            return 0;
        }
    }
    
    Token eof_token;
    eof_token.type = TOKEN_EOF;
    eof_token.value[0] = '\0';
    push_token(stack, eof_token);
    
    return 1;
}

void print_tokens(TokenStack* stack) {
    if (!stack) {
        return;
    }
    
    for (size_t i = 0; i < stack->count; i++) {
        const char* type_str;
        switch (stack->tokens[i].type) {
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case TOKEN_IDENTIFIER: type_str = "IDENTIFIER"; break;
            case TOKEN_OPERATOR: type_str = "OPERATOR"; break;
            case TOKEN_STRING: type_str = "STRING"; break;
            case TOKEN_EOF: type_str = "EOF"; break;
            default: type_str = "UNKNOWN"; break;
        }
        printf("Token: %s, Value: '%s'\n", type_str, stack->tokens[i].value);
    }
}

int main(void) {
    char input[1024];
    
    printf("Enter an expression to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
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
    
    TokenStack* stack = create_token_stack(MAX_TOKENS