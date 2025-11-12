//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64
#define DELIMITERS " \t\n\r"
#define IS_OPERATOR(c) (c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '(' || c == ')')

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_END
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

int is_identifier_char(char c) {
    return isalnum(c) || c == '_';
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

        if (IS_OPERATOR(*p)) {
            buffer[0] = *p;
            buffer[1] = '\0';
            add_token(TOKEN_OPERATOR, buffer);
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
            while (is_identifier_char(*p) && buf_index < MAX_TOKEN_LEN - 1) {
                buffer[buf_index++] = *p++;
            }
            buffer[buf_index] = '\0';
            add_token(TOKEN_IDENTIFIER, buffer);
            continue;
        }

        fprintf(stderr, "Error: Invalid character '%c'\n", *p);
        exit(1);
    }
    add_token(TOKEN_END, "");
}

Token* next_token() {
    if (current_token >= token_count) {
        return NULL;
    }
    return &tokens[current_token++];
}

void parse_expression();

void parse_factor() {
    Token* token = next_token();
    if (token == NULL) {
        fprintf(stderr, "Error: Unexpected end of input\n");
        exit(1);
    }

    if (token->type == TOKEN_NUMBER) {
        printf("Number: %s\n", token->value);
    } else if (token->type == TOKEN_IDENTIFIER) {
        printf("Identifier: %s\n", token->value);
    } else if (token->type == TOKEN_OPERATOR && token->value[0] == '(') {
        parse_expression();
        token = next_token();
        if (token == NULL || token->type != TOKEN_OPERATOR || token->value[0] != ')') {
            fprintf(stderr, "Error: Expected ')'\n");
            exit(1);
        }
    } else {
        fprintf(stderr, "Error: Expected number, identifier, or '('\n");
        exit(1);
    }
}

void parse_term() {
    parse_factor();
    Token* token = next_token();
    while (token != NULL && token->type == TOKEN_OPERATOR && 
           (token->value[0] == '*' || token->value[0] == '/')) {
        printf("Operator: %s\n", token->value);
        parse_factor();
        token = next_token();
    }
    if (token != NULL) {
        current_token--;
    }
}

void parse_expression() {
    parse_term();
    Token* token = next_token();
    while (token != NULL && token->type == TOKEN_OPERATOR && 
           (token->value[0] == '+' || token->value[0] == '-')) {
        printf("Operator: %s\n", token->value);
        parse_term();
        token = next_token();
    }
    if (token != NULL) {
        current_token--;
    }
}

void parse_assignment() {
    Token* token = next_token();
    if (token == NULL || token->type != TOKEN_IDENTIFIER) {
        fprintf(stderr, "Error: Expected identifier\n");
        exit(1);
    }
    printf("Identifier: %s\n", token->value);

    token = next_token();
    if (token == NULL || token->type != TOKEN_OPERATOR || token->value[0] != '=') {
        fprintf(stderr, "Error: Expected '='\n");
        exit(1);
    }
    printf("Operator: =\n");

    parse_expression();

    token = next_token();
    if (token == NULL || token->type != TOKEN_END) {
        fprintf(stderr, "Error: Expected end of input\n