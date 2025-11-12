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
    if (!str || strlen(str) == 0) return TOKEN_EOF;
    if (isdigit(str[0])) return TOKEN_NUMBER;
    if (str[0] == '"' && str[strlen(str)-1] == '"') return TOKEN_STRING;
    if (is_operator(str[0]) && strlen(str) == 1) return TOKEN_OPERATOR;
    if (isalpha(str[0])) return TOKEN_IDENTIFIER;
    return TOKEN_EOF;
}

char** tokenize_input(const char* input, size_t* token_count) {
    if (!input || !token_count) return NULL;
    char** tokens = malloc(sizeof(char*) * MAX_TOKENS);
    if (!tokens) return NULL;
    for (size_t i = 0; i < MAX_TOKENS; i++) {
        tokens[i] = malloc(MAX_TOKEN_LEN);
        if (!tokens[i]) {
            for (size_t j = 0; j < i; j++) free(tokens[j]);
            free(tokens);
            return NULL;
        }
        tokens[i][0] = '\0';
    }
    size_t count = 0;
    size_t pos = 0;
    while (input[pos] != '\0' && count < MAX_TOKENS) {
        while (isspace(input[pos])) pos++;
        if (input[pos] == '\0') break;
        size_t start = pos;
        if (input[pos] == '"') {
            pos++;
            while (input[pos] != '"' && input[pos] != '\0') pos++;
            if (input[pos] == '"') pos++;
        } else if (is_operator(input[pos])) {
            pos++;
        } else {
            while (!isspace(input[pos]) && !is_operator(input[pos]) && input[pos] != '\0') pos++;
        }
        size_t len = pos - start;
        if (len > 0 && len < MAX_TOKEN_LEN) {
            strncpy(tokens[count], &input[start], len);
            tokens[count][len] = '\0';
            count++;
        }
    }
    *token_count = count;
    return tokens;
}

void free_tokens(char** tokens, size_t count) {
    if (!tokens) return;
    for (size_t i = 0; i < count; i++) {
        free(tokens[i]);
    }
    free(tokens);
}

int main(void) {
    char input[1024];
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
    size_t token_count = 0;
    char** tokens = tokenize_input(input, &token_count);
    if (!tokens) {
        printf("Tokenization failed\n");
        return 1;
    }
    TokenStack* stack = create_stack(MAX_TOKENS);
    if (!stack) {
        printf("Stack creation failed\n");
        free_tokens(tokens, token_count);
        return 1;
    }
    for (size_t i = 0; i < token_count; i++) {
        Token token;
        token.type = classify_token(tokens[i]);
        strncpy(token.value, tokens[i], MAX_TOKEN_LEN-1);
        token.value[MAX_TOKEN_LEN-1] = '\0';
        if (!push_token