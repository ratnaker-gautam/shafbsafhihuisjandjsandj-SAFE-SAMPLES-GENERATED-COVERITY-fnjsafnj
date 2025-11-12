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

void add_token(TokenType type, const char* value) {
    if (token_count >= MAX_TOKENS) return;
    Token* token = &tokens[token_count++];
    token->type = type;
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void tokenize(const char* input) {
    token_count = 0;
    current_token = 0;
    const char* ptr = input;
    
    while (*ptr && token_count < MAX_TOKENS) {
        while (*ptr && strchr(DELIMITERS, *ptr)) ptr++;
        if (!*ptr) break;
        
        if (isalpha(*ptr) || *ptr == '_') {
            char buffer[MAX_TOKEN_LEN] = {0};
            size_t i = 0;
            while ((isalnum(*ptr) || *ptr == '_') && i < MAX_TOKEN_LEN - 1) {
                buffer[i++] = *ptr++;
            }
            add_token(TOKEN_IDENTIFIER, buffer);
        }
        else if (isdigit(*ptr)) {
            char buffer[MAX_TOKEN_LEN] = {0};
            size_t i = 0;
            while (isdigit(*ptr) && i < MAX_TOKEN_LEN - 1) {
                buffer[i++] = *ptr++;
            }
            add_token(TOKEN_NUMBER, buffer);
        }
        else if (*ptr == '"') {
            char buffer[MAX_TOKEN_LEN] = {0};
            size_t i = 0;
            ptr++;
            while (*ptr && *ptr != '"' && i < MAX_TOKEN_LEN - 1) {
                buffer[i++] = *ptr++;
            }
            if (*ptr == '"') ptr++;
            add_token(TOKEN_STRING, buffer);
        }
        else if (is_operator(*ptr)) {
            char buffer[2] = {*ptr, '\0'};
            add_token(TOKEN_OPERATOR, buffer);
            ptr++;
        }
        else {
            char buffer[2] = {*ptr, '\0'};
            add_token(TOKEN_UNKNOWN, buffer);
            ptr++;
        }
    }
    add_token(TOKEN_EOF, "");
}

const char* token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

void print_tokens(void) {
    for (size_t i = 0; i < token_count; i++) {
        printf("Token %zu: %s -> '%s'\n", i, token_type_name(tokens[i].type), tokens[i].value);
    }
}

int main(void) {
    char input[1024];
    
    printf("Enter text to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    tokenize(input);
    print_tokens();
    
    return 0;
}