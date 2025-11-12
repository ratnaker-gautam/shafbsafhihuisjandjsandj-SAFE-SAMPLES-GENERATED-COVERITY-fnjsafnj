//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64
#define DELIMITERS " \t\n\r"

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

Token tokens[MAX_TOKENS];
size_t token_count = 0;
size_t current_token = 0;

void add_token(TokenType type, const char* value) {
    if (token_count >= MAX_TOKENS) return;
    tokens[token_count].type = type;
    strncpy(tokens[token_count].value, value, MAX_TOKEN_LEN - 1);
    tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
    token_count++;
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void tokenize(const char* input) {
    const char* p = input;
    char buffer[MAX_TOKEN_LEN];
    size_t buf_index = 0;

    while (*p && token_count < MAX_TOKENS) {
        while (*p && strchr(DELIMITERS, *p)) p++;
        if (!*p) break;

        if (isdigit(*p)) {
            buf_index = 0;
            while (*p && isdigit(*p) && buf_index < MAX_TOKEN_LEN - 1) {
                buffer[buf_index++] = *p++;
            }
            buffer[buf_index] = '\0';
            add_token(TOKEN_NUMBER, buffer);
        } else if (isalpha(*p) || *p == '_') {
            buf_index = 0;
            while (*p && (isalnum(*p) || *p == '_') && buf_index < MAX_TOKEN_LEN - 1) {
                buffer[buf_index++] = *p++;
            }
            buffer[buf_index] = '\0';
            add_token(TOKEN_IDENTIFIER, buffer);
        } else if (is_operator(*p)) {
            buf_index = 0;
            while (*p && is_operator(*p) && buf_index < MAX_TOKEN_LEN - 1) {
                buffer[buf_index++] = *p++;
            }
            buffer[buf_index] = '\0';
            add_token(TOKEN_OPERATOR, buffer);
        } else if (*p == '"') {
            buf_index = 0;
            p++;
            while (*p && *p != '"' && buf_index < MAX_TOKEN_LEN - 1) {
                buffer[buf_index++] = *p++;
            }
            buffer[buf_index] = '\0';
            if (*p == '"') p++;
            add_token(TOKEN_STRING, buffer);
        } else {
            p++;
        }
    }
    add_token(TOKEN_EOF, "");
}

void print_token_type(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_EOF: printf("EOF"); break;
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
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    if (strlen(input) == 0 || (strlen(input) == 1 && input[0] == '\n')) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }

    tokenize(input);
    print_tokens();

    return 0;
}