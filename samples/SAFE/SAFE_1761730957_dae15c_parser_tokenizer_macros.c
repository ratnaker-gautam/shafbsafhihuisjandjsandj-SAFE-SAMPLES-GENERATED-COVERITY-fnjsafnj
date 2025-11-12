//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50
#define DELIMITERS " \t\n"

typedef enum {
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_IDENTIFIER,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

Token tokens[MAX_TOKENS];
int token_count = 0;

#define IS_OPERATOR(c) (c == '+' || c == '-' || c == '*' || c == '/' || c == '=')
#define IS_DIGIT(c) (c >= '0' && c <= '9')
#define IS_ALPHA(c) ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))

void add_token(TokenType type, const char* value) {
    if (token_count >= MAX_TOKENS) {
        fprintf(stderr, "Error: Too many tokens\n");
        exit(1);
    }
    strncpy(tokens[token_count].value, value, MAX_TOKEN_LEN - 1);
    tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
    tokens[token_count].type = type;
    token_count++;
}

void tokenize(const char* input) {
    const char* p = input;
    char buffer[MAX_TOKEN_LEN];
    int buf_index = 0;

    while (*p != '\0') {
        if (isspace(*p)) {
            p++;
            continue;
        }

        if (IS_DIGIT(*p)) {
            buf_index = 0;
            while (IS_DIGIT(*p) && buf_index < MAX_TOKEN_LEN - 1) {
                buffer[buf_index++] = *p++;
            }
            buffer[buf_index] = '\0';
            add_token(TOKEN_NUMBER, buffer);
            continue;
        }

        if (IS_ALPHA(*p)) {
            buf_index = 0;
            while ((IS_ALPHA(*p) || IS_DIGIT(*p)) && buf_index < MAX_TOKEN_LEN - 1) {
                buffer[buf_index++] = *p++;
            }
            buffer[buf_index] = '\0';
            add_token(TOKEN_IDENTIFIER, buffer);
            continue;
        }

        if (IS_OPERATOR(*p)) {
            buffer[0] = *p++;
            buffer[1] = '\0';
            add_token(TOKEN_OPERATOR, buffer);
            continue;
        }

        buffer[0] = *p++;
        buffer[1] = '\0';
        add_token(TOKEN_UNKNOWN, buffer);
    }
}

void print_tokens() {
    for (int i = 0; i < token_count; i++) {
        const char* type_str;
        switch (tokens[i].type) {
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case TOKEN_OPERATOR: type_str = "OPERATOR"; break;
            case TOKEN_IDENTIFIER: type_str = "IDENTIFIER"; break;
            default: type_str = "UNKNOWN"; break;
        }
        printf("Token %d: %s -> %s\n", i, type_str, tokens[i].value);
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

    if (strlen(input) == 0) {
        fprintf(stderr, "Error: Empty input\n");
        return 1;
    }

    tokenize(input);
    print_tokens();

    return 0;
}