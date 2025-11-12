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
    const char* p = input;
    char buffer[MAX_TOKEN_LEN];
    size_t buf_index = 0;

    while (*p != '\0') {
        if (isspace(*p)) {
            p++;
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

        if (isalpha(*p) || *p == '_') {
            buf_index = 0;
            while ((isalnum(*p) || *p == '_') && buf_index < MAX_TOKEN_LEN - 1) {
                buffer[buf_index++] = *p++;
            }
            buffer[buf_index] = '\0';
            add_token(TOKEN_IDENTIFIER, buffer);
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
            buffer[buf_index++] = *p++;
            if (is_operator(*p)) {
                if (buf_index < MAX_TOKEN_LEN - 1) {
                    buffer[buf_index++] = *p++;
                }
            }
            buffer[buf_index] = '\0';
            add_token(TOKEN_OPERATOR, buffer);
            continue;
        }

        fprintf(stderr, "Error: Invalid character '%c'\n", *p);
        exit(1);
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

void parse_expression();

void parse_factor() {
    if (current_token >= token_count) {
        fprintf(stderr, "Error: Unexpected end of input\n");
        exit(1);
    }
    Token token = tokens[current_token];
    if (token.type == TOKEN_NUMBER || token.type == TOKEN_IDENTIFIER) {
        printf("Factor: %s\n", token.value);
        current_token++;
    } else if (token.type == TOKEN_STRING) {
        printf("String: \"%s\"\n", token.value);
        current_token++;
    } else {
        fprintf(stderr, "Error: Expected factor, got ");
        print_token_type(token.type);
        printf("\n");
        exit(1);
    }
}

void parse_term() {
    parse_factor();
    while (current_token < token_count) {
        Token token = tokens[current_token];
        if (token.type == TOKEN_OPERATOR && 
            (strcmp(token.value, "*") == 0 || strcmp(token.value, "/") == 0)) {
            printf("Operator: %s\n", token.value);
            current_token++;
            parse_factor();
        } else {
            break;
        }
    }
}

void parse_expression() {
    parse_term();
    while (current_token < token_count) {
        Token token = tokens[current_token];
        if (token.type == TOKEN_OPERATOR && 
            (strcmp(token.value, "+") == 0 || strcmp(token.value, "-") == 0)) {
            printf("Operator: %s\n", token.value);
            current_token++;
            parse_term();
        } else {
            break;
        }
    }
}

int main() {
    char input[1024];
    printf("Enter expression: