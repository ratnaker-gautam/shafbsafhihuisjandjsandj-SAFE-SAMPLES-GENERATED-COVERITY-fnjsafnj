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
    int line;
    int column;
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
    if (strlen(token.value) >= MAX_TOKEN_LEN) return 0;
    stack->tokens[stack->size] = token;
    stack->size++;
    return 1;
}

int pop_token(TokenStack* stack, Token* token) {
    if (!stack || !token || stack->size == 0) return 0;
    stack->size--;
    *token = stack->tokens[stack->size];
    return 1;
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

Token* tokenize(const char* input, size_t* token_count) {
    if (!input || !token_count) return NULL;
    *token_count = 0;
    
    Token* tokens = malloc(MAX_TOKENS * sizeof(Token));
    if (!tokens) return NULL;
    
    size_t input_len = strlen(input);
    size_t pos = 0;
    int line = 1;
    int column = 1;
    
    while (pos < input_len && *token_count < MAX_TOKENS) {
        while (pos < input_len && isspace(input[pos])) {
            if (input[pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
        }
        
        if (pos >= input_len) break;
        
        Token token = {0};
        token.line = line;
        token.column = column;
        
        if (isdigit(input[pos])) {
            token.type = TOKEN_NUMBER;
            size_t i = 0;
            while (pos < input_len && i < MAX_TOKEN_LEN - 1 && isdigit(input[pos])) {
                token.value[i++] = input[pos++];
                column++;
            }
            token.value[i] = '\0';
        } else if (isalpha(input[pos]) || input[pos] == '_') {
            token.type = TOKEN_IDENTIFIER;
            size_t i = 0;
            while (pos < input_len && i < MAX_TOKEN_LEN - 1 && 
                   (isalnum(input[pos]) || input[pos] == '_')) {
                token.value[i++] = input[pos++];
                column++;
            }
            token.value[i] = '\0';
        } else if (input[pos] == '"') {
            token.type = TOKEN_STRING;
            size_t i = 0;
            token.value[i++] = input[pos++];
            column++;
            while (pos < input_len && i < MAX_TOKEN_LEN - 1 && input[pos] != '"') {
                token.value[i++] = input[pos++];
                column++;
            }
            if (pos < input_len && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = input[pos++];
                column++;
            }
            token.value[i] = '\0';
        } else if (is_operator(input[pos])) {
            token.type = TOKEN_OPERATOR;
            size_t i = 0;
            while (pos < input_len && i < MAX_TOKEN_LEN - 1 && is_operator(input[pos])) {
                token.value[i++] = input[pos++];
                column++;
            }
            token.value[i] = '\0';
        } else {
            pos++;
            column++;
            continue;
        }
        
        if (strlen(token.value) > 0) {
            tokens[*token_count] = token;
            (*token_count)++;
        }
    }
    
    if (*token_count < MAX_TOKENS) {
        Token eof_token = {TOKEN_EOF, "", line, column};
        tokens[*token_count] = eof_token;
        (*token_count)++;
    }
    
    return tokens;
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    size_t token_count;
    Token* tokens = tokenize(input, &token_count);
    if (!tokens) {
        fprintf(stderr, "