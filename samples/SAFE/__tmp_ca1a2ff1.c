//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '(' || c == ')';
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
                fprintf(stderr, "Error: Unclosed string literal\n");
                exit(1);
            }
            buffer[buf_index] = '\0';
            add_token(TOKEN_STRING, buffer);
            p++;
            continue;
        }

        if (is_operator(*p)) {
            buf_index = 0;
            buffer[buf_index++] = *p;
            if (is_operator(*p) && is_operator(*(p + 1)) && buf_index < MAX_TOKEN_LEN - 1) {
                buffer[buf_index++] = *(p + 1);
                p += 2;
            } else {
                p++;
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

void parse_expression(void);
void parse_term(void);
void parse_factor(void);

void parse_expression(void) {
    parse_term();
    while (current_token < token_count && tokens[current_token].type == TOKEN_OPERATOR && 
           (strcmp(tokens[current_token].value, "+") == 0 || strcmp(tokens[current_token].value, "-") == 0)) {
        printf("Operator: %s\n", tokens[current_token].value);
        current_token++;
        parse_term();
    }
}

void parse_term(void) {
    parse_factor();
    while (current_token < token_count && tokens[current_token].type == TOKEN_OPERATOR && 
           (strcmp(tokens[current_token].value, "*") == 0 || strcmp(tokens[current_token].value, "/") == 0)) {
        printf("Operator: %s\n", tokens[current_token].value);
        current_token++;
        parse_factor();
    }
}

void parse_factor(void) {
    if (current_token >= token_count) {
        fprintf(stderr, "Error: Unexpected end of input\n");
        exit(1);
    }

    Token token = tokens[current_token];
    if (token.type == TOKEN_NUMBER) {
        printf("Number: %s\n", token.value);
        current_token++;
    } else if (token.type == TOKEN_IDENTIFIER) {
        printf("Identifier: %s\n", token.value);
        current_token++;
    } else if (token.type == TOKEN_STRING) {
        printf("String: \"%s\"\n", token.value);
        current_token++;
    } else if (token.type == TOKEN_