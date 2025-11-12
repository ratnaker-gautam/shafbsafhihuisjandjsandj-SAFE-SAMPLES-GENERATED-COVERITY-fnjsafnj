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

    while (*p != '\0') {
        if (isspace(*p)) {
            p++;
            continue;
        }

        if (isalpha(*p) || *p == '_') {
            buf_index = 0;
            while ((isalnum(*p) || *p == '_') && buf_index < MAX_TOKEN_LEN - 1) {
                buffer[buf_index++] = *p++;
            }
            buffer[buf_index] = '\0';
            add_token(TOKEN_IDENTIFIER, buffer);
            continue;
        }

        if (isdigit(*p)) {
            buf_index = 0;
            while (isdigit(*p) && buf_index < MAX_TOKEN_LEN - 1) {
                buffer[buf_index++] = *p++;
            }
            buffer[buf_index] = '\0';
            add_token(TOKEN_NUMBER, buffer);
            continue;
        }

        if (*p == '"') {
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
            continue;
        }

        if (is_operator(*p)) {
            buf_index = 0;
            while (is_operator(*p) && buf_index < MAX_TOKEN_LEN - 1) {
                buffer[buf_index++] = *p++;
            }
            buffer[buf_index] = '\0';
            add_token(TOKEN_OPERATOR, buffer);
            continue;
        }

        buf_index = 0;
        buffer[buf_index++] = *p++;
        buffer[buf_index] = '\0';
        add_token(TOKEN_UNKNOWN, buffer);
    }

    add_token(TOKEN_EOF, "");
}

Token* next_token() {
    if (current_token >= token_count) {
        return NULL;
    }
    return &tokens[current_token++];
}

void print_token(const Token* token) {
    const char* type_names[] = {
        "IDENTIFIER", "NUMBER", "OPERATOR", "STRING", "EOF", "UNKNOWN"
    };
    printf("%-12s: %s\n", type_names[token->type], token->value);
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

    printf("\nTokens:\n");
    Token* token;
    while ((token = next_token()) != NULL) {
        print_token(token);
    }

    return 0;
}