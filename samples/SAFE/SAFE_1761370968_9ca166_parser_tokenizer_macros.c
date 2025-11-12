//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64
#define DELIMITERS " \t\n\r"

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_ERROR
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

Token tokens[MAX_TOKENS];
size_t token_count = 0;
size_t current_token = 0;

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void add_token(TokenType type, const char* value) {
    if (token_count >= MAX_TOKENS) return;
    Token* token = &tokens[token_count++];
    token->type = type;
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
}

int tokenize(const char* input) {
    token_count = 0;
    size_t len = strlen(input);
    size_t pos = 0;
    
    while (pos < len && token_count < MAX_TOKENS) {
        while (pos < len && strchr(DELIMITERS, input[pos])) pos++;
        if (pos >= len) break;
        
        char current = input[pos];
        
        if (isalpha(current) || current == '_') {
            char buffer[MAX_TOKEN_LEN] = {0};
            size_t buf_pos = 0;
            
            while (pos < len && (isalnum(input[pos]) || input[pos] == '_') && buf_pos < MAX_TOKEN_LEN - 1) {
                buffer[buf_pos++] = input[pos++];
            }
            buffer[buf_pos] = '\0';
            add_token(TOKEN_IDENTIFIER, buffer);
        }
        else if (isdigit(current)) {
            char buffer[MAX_TOKEN_LEN] = {0};
            size_t buf_pos = 0;
            
            while (pos < len && isdigit(input[pos]) && buf_pos < MAX_TOKEN_LEN - 1) {
                buffer[buf_pos++] = input[pos++];
            }
            buffer[buf_pos] = '\0';
            add_token(TOKEN_NUMBER, buffer);
        }
        else if (current == '"') {
            char buffer[MAX_TOKEN_LEN] = {0};
            size_t buf_pos = 0;
            pos++;
            
            while (pos < len && input[pos] != '"' && buf_pos < MAX_TOKEN_LEN - 1) {
                buffer[buf_pos++] = input[pos++];
            }
            
            if (pos < len && input[pos] == '"') {
                pos++;
                buffer[buf_pos] = '\0';
                add_token(TOKEN_STRING, buffer);
            } else {
                add_token(TOKEN_ERROR, "Unterminated string");
                return -1;
            }
        }
        else if (is_operator(current)) {
            char op[2] = {current, '\0'};
            add_token(TOKEN_OPERATOR, op);
            pos++;
        }
        else {
            char unknown[2] = {current, '\0'};
            add_token(TOKEN_ERROR, unknown);
            return -1;
        }
    }
    
    add_token(TOKEN_EOF, "");
    return 0;
}

void print_token_type(TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_EOF: printf("EOF"); break;
        case TOKEN_ERROR: printf("ERROR"); break;
    }
}

void print_tokens(void) {
    for (size_t i = 0; i < token_count; i++) {
        printf("Token %zu: ", i);
        print_token_type(tokens[i].type);
        if (tokens[i].value[0] != '\0') {
            printf(" ('%s')", tokens[i].value);
        }
        printf("\n");
    }
}

int main(void) {
    char input[1024];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (tokenize(input) != 0) {
        fprintf(stderr, "Tokenization failed\n");
        return 1;
    }
    
    printf("\nTokens:\n");
    print_tokens();
    
    return 0;
}