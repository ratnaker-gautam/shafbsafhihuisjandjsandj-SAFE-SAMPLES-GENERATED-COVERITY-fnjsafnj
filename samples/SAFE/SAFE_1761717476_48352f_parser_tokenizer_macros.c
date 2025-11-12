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

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void add_token(TokenType type, const char* value) {
    if (token_count >= MAX_TOKENS) {
        fprintf(stderr, "Error: Too many tokens\n");
        exit(1);
    }
    tokens[token_count].type = type;
    strncpy(tokens[token_count].value, value, MAX_TOKEN_LEN - 1);
    tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
    token_count++;
}

void tokenize(const char* input) {
    const char* p = input;
    char buffer[MAX_TOKEN_LEN];
    size_t buf_index = 0;

    while (*p != '\0' && token_count < MAX_TOKENS) {
        while (isspace(*p)) p++;

        if (*p == '\0') break;

        if (isalpha(*p)) {
            buf_index = 0;
            while (isalnum(*p) && buf_index < MAX_TOKEN_LEN - 1) {
                buffer[buf_index++] = *p++;
            }
            buffer[buf_index] = '\0';
            add_token(TOKEN_IDENTIFIER, buffer);
        }
        else if (isdigit(*p)) {
            buf_index = 0;
            while (isdigit(*p) && buf_index < MAX_TOKEN_LEN - 1) {
                buffer[buf_index++] = *p++;
            }
            buffer[buf_index] = '\0';
            add_token(TOKEN_NUMBER, buffer);
        }
        else if (*p == '"') {
            buf_index = 0;
            p++;
            while (*p != '"' && *p != '\0' && buf_index < MAX_TOKEN_LEN - 1) {
                buffer[buf_index++] = *p++;
            }
            if (*p != '"') {
                fprintf(stderr, "Error: Unclosed string\n");
                exit(1);
            }
            p++;
            buffer[buf_index] = '\0';
            add_token(TOKEN_STRING, buffer);
        }
        else if (is_operator(*p)) {
            buf_index = 0;
            buffer[buf_index++] = *p++;
            if (is_operator(*p)) {
                buffer[buf_index++] = *p++;
            }
            buffer[buf_index] = '\0';
            add_token(TOKEN_OPERATOR, buffer);
        }
        else {
            buf_index = 0;
            buffer[buf_index++] = *p++;
            buffer[buf_index] = '\0';
            add_token(TOKEN_UNKNOWN, buffer);
        }
    }
    add_token(TOKEN_EOF, "");
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
    for (size_t i = 0; i < token_count; i++) {
        printf("Token %zu: ", i);
        print_token_type(tokens[i].type);
        printf(" '%s'\n", tokens[i].value);
    }
}

int main() {
    char input[1024];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Error: Empty input\n");
        return 1;
    }
    
    tokenize(input);
    print_tokens();
    
    return 0;
}