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
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

Token tokens[MAX_TOKENS];
size_t token_count = 0;
size_t current_token = 0;

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

TokenType classify_token(const char* str) {
    if (strlen(str) == 0) return TOKEN_UNKNOWN;
    
    if (isdigit(str[0])) {
        for (size_t i = 0; str[i]; i++) {
            if (!isdigit(str[i])) return TOKEN_UNKNOWN;
        }
        return TOKEN_NUMBER;
    }
    
    if (str[0] == '"' && str[strlen(str)-1] == '"') {
        return TOKEN_STRING;
    }
    
    if (is_operator_char(str[0])) {
        return TOKEN_OPERATOR;
    }
    
    if (isalpha(str[0]) || str[0] == '_') {
        for (size_t i = 0; str[i]; i++) {
            if (!isalnum(str[i]) && str[i] != '_') return TOKEN_UNKNOWN;
        }
        return TOKEN_IDENTIFIER;
    }
    
    return TOKEN_UNKNOWN;
}

int tokenize(const char* input) {
    if (!input) return 0;
    
    char buffer[MAX_TOKEN_LEN];
    size_t buf_pos = 0;
    size_t input_len = strlen(input);
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        
        if (strchr(DELIMITERS, c) || c == '\0' || is_operator_char(c)) {
            if (buf_pos > 0) {
                if (buf_pos >= MAX_TOKEN_LEN) return 0;
                buffer[buf_pos] = '\0';
                
                TokenType type = classify_token(buffer);
                if (type != TOKEN_UNKNOWN && token_count < MAX_TOKENS) {
                    strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN-1);
                    tokens[token_count].value[MAX_TOKEN_LEN-1] = '\0';
                    tokens[token_count].type = type;
                    token_count++;
                }
                buf_pos = 0;
            }
            
            if (is_operator_char(c) && token_count < MAX_TOKENS) {
                tokens[token_count].value[0] = c;
                tokens[token_count].value[1] = '\0';
                tokens[token_count].type = TOKEN_OPERATOR;
                token_count++;
            }
        } else {
            if (buf_pos < MAX_TOKEN_LEN - 1) {
                buffer[buf_pos++] = c;
            } else {
                return 0;
            }
        }
    }
    
    if (token_count < MAX_TOKENS) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "");
        token_count++;
    }
    
    return 1;
}

void print_token_type(TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_EOF: printf("EOF"); break;
        default: printf("UNKNOWN"); break;
    }
}

void print_tokens() {
    printf("Tokens found: %zu\n", token_count);
    for (size_t i = 0; i < token_count; i++) {
        printf("Token %zu: ", i);
        print_token_type(tokens[i].type);
        printf(" -> '%s'\n", tokens[i].value);
    }
}

int main() {
    char input[1024];
    
    printf("Enter text to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (!tokenize(input)) {
        fprintf(stderr, "Tokenization failed\n");
        return 1;
    }
    
    print_tokens();
    
    return 0;
}