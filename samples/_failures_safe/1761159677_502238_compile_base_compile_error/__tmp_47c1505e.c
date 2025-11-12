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
    stack->tokens[stack->count] = token;
    stack->count++;
    return 1;
}

Token pop_token(TokenStack* stack) {
    Token empty = {TOKEN_EOF, ""};
    if (!stack || stack->count == 0) return empty;
    stack->count--;
    return stack->tokens[stack->count];
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

Token* tokenize(const char* input, size_t* token_count) {
    if (!input || !token_count) return NULL;
    
    Token* tokens = malloc(sizeof(Token) * MAX_TOKENS);
    if (!tokens) return NULL;
    
    size_t count = 0;
    const char* ptr = input;
    
    while (*ptr && count < MAX_TOKENS) {
        while (isspace(*ptr)) ptr++;
        if (!*ptr) break;
        
        Token token = {TOKEN_EOF, ""};
        
        if (isdigit(*ptr)) {
            token.type = TOKEN_NUMBER;
            size_t i = 0;
            while (isdigit(*ptr) && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = *ptr++;
            }
            token.value[i] = '\0';
        }
        else if (isalpha(*ptr)) {
            token.type = TOKEN_IDENTIFIER;
            size_t i = 0;
            while ((isalnum(*ptr) || *ptr == '_') && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = *ptr++;
            }
            token.value[i] = '\0';
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
        else if (is_operator(*ptr)) {
            token.type = TOKEN_OPERATOR;
            token.value[0] = *ptr;
            token.value[1] = '\0';
            ptr++;
        }
        else {
            ptr++;
            continue;
        }
        
        if (token.type != TOKEN_EOF) {
            tokens[count++] = token;
        }
    }
    
    *token_count = count;
    return tokens;
}

void print_tokens(const Token* tokens, size_t count) {
    if (!tokens) return;
    for (size_t i = 0; i < count; i++) {
        const char* type_str = "";
        switch (tokens[i].type) {
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case TOKEN_IDENTIFIER: type_str = "IDENTIFIER"; break;
            case TOKEN_OPERATOR: type_str = "OPERATOR"; break;
            case TOKEN_STRING: type_str = "STRING"; break;
            case TOKEN_EOF: type_str = "EOF"; break;
        }
        printf("Token %zu: %s -> '%s'\n", i, type_str, tokens[i].value);
    }
}

int main(void) {
    char input[1024];
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
    
    size_t token_count = 0;
    Token* tokens = tokenize(input, &token_count);
    
    if (!tokens) {
        fprintf(stderr, "Tokenization failed\n");
        return 1;
    }
    
    printf("\nTokens found (%zu):\n", token_count);
    print_tokens(tokens, token_count);
    
    TokenStack* stack = create_stack(MAX_TOKENS);
    if (!stack) {
        fprintf(stderr, "