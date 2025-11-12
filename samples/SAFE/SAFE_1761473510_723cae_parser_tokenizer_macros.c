//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50
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
    if (token_count >= MAX_TOKENS) {
        fprintf(stderr, "Error: Too many tokens\n");
        exit(1);
    }
    tokens[token_count].type = type;
    strncpy(tokens[token_count].value, value, MAX_TOKEN_LEN - 1);
    tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
    token_count++;
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void tokenize(const char* input) {
    const char* ptr = input;
    char buffer[MAX_TOKEN_LEN];
    size_t buf_index = 0;

    while (*ptr != '\0' && token_count < MAX_TOKENS) {
        if (isspace((unsigned char)*ptr)) {
            ptr++;
            continue;
        }

        if (isdigit((unsigned char)*ptr)) {
            buf_index = 0;
            while (isdigit((unsigned char)*ptr) && buf_index < MAX_TOKEN_LEN - 1) {
                buffer[buf_index++] = *ptr++;
            }
            buffer[buf_index] = '\0';
            add_token(TOKEN_NUMBER, buffer);
            continue;
        }

        if (isalpha((unsigned char)*ptr) || *ptr == '_') {
            buf_index = 0;
            while ((isalnum((unsigned char)*ptr) || *ptr == '_') && buf_index < MAX_TOKEN_LEN - 1) {
                buffer[buf_index++] = *ptr++;
            }
            buffer[buf_index] = '\0';
            add_token(TOKEN_IDENTIFIER, buffer);
            continue;
        }

        if (is_operator(*ptr)) {
            buf_index = 0;
            while (is_operator(*ptr) && buf_index < MAX_TOKEN_LEN - 1) {
                buffer[buf_index++] = *ptr++;
            }
            buffer[buf_index] = '\0';
            add_token(TOKEN_OPERATOR, buffer);
            continue;
        }

        if (*ptr == '"') {
            buf_index = 0;
            ptr++;
            while (*ptr != '"' && *ptr != '\0' && buf_index < MAX_TOKEN_LEN - 1) {
                buffer[buf_index++] = *ptr++;
            }
            if (*ptr != '"') {
                fprintf(stderr, "Error: Unclosed string literal\n");
                exit(1);
            }
            ptr++;
            buffer[buf_index] = '\0';
            add_token(TOKEN_STRING, buffer);
            continue;
        }

        fprintf(stderr, "Error: Invalid character '%c'\n", *ptr);
        exit(1);
    }

    add_token(TOKEN_EOF, "");
}

const char* token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

void print_tokens() {
    printf("Tokens:\n");
    for (size_t i = 0; i < token_count; i++) {
        printf("  %s: '%s'\n", token_type_name(tokens[i].type), tokens[i].value);
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