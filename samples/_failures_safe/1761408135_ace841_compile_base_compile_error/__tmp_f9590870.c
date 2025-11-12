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
    const char* p = input;
    char buffer[MAX_TOKEN_LEN];
    size_t buf_index = 0;
    int in_string = 0;

    while (*p != '\0') {
        if (isspace((unsigned char)*p)) {
            p++;
            continue;
        }

        if (*p == '"') {
            in_string = !in_string;
            if (!in_string && buf_index > 0) {
                buffer[buf_index] = '\0';
                add_token(TOKEN_STRING, buffer);
                buf_index = 0;
            }
            p++;
            continue;
        }

        if (in_string) {
            if (buf_index >= MAX_TOKEN_LEN - 1) {
                fprintf(stderr, "Error: String too long\n");
                exit(1);
            }
            buffer[buf_index++] = *p++;
            continue;
        }

        if (is_operator(*p)) {
            if (buf_index > 0) {
                buffer[buf_index] = '\0';
                add_token(TOKEN_IDENTIFIER, buffer);
                buf_index = 0;
            }
            buffer[0] = *p;
            buffer[1] = '\0';
            add_token(TOKEN_OPERATOR, buffer);
            p++;
            continue;
        }

        if (isdigit((unsigned char)*p)) {
            if (buf_index > 0 && !isdigit((unsigned char)buffer[buf_index-1])) {
                buffer[buf_index] = '\0';
                add_token(TOKEN_IDENTIFIER, buffer);
                buf_index = 0;
            }
            if (buf_index >= MAX_TOKEN_LEN - 1) {
                fprintf(stderr, "Error: Number too long\n");
                exit(1);
            }
            buffer[buf_index++] = *p++;
            continue;
        }

        if (isalpha((unsigned char)*p) || *p == '_') {
            if (buf_index > 0 && !isalnum((unsigned char)buffer[buf_index-1]) && buffer[buf_index-1] != '_') {
                buffer[buf_index] = '\0';
                add_token(TOKEN_IDENTIFIER, buffer);
                buf_index = 0;
            }
            if (buf_index >= MAX_TOKEN_LEN - 1) {
                fprintf(stderr, "Error: Identifier too long\n");
                exit(1);
            }
            buffer[buf_index++] = *p++;
            continue;
        }

        if (buf_index >= MAX_TOKEN_LEN - 1) {
            fprintf(stderr, "Error: Token too long\n");
            exit(1);
        }
        buffer[buf_index++] = *p++;
    }

    if (buf_index > 0) {
        buffer[buf_index] = '\0';
        if (isdigit((unsigned char)buffer[0])) {
            add_token(TOKEN_NUMBER, buffer);
        } else {
            add_token(TOKEN_IDENTIFIER, buffer);
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

void print_tokens() {
    printf("Tokens:\n");
    for (size_t i = 0; i < token_count; i++) {
        printf("%zu: %s -> '%s'\n", i, token_type_name(tokens[i].type), tokens[i].value);
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
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (strlen(input) ==