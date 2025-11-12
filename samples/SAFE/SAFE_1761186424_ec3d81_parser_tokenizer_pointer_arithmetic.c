//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(const char **input) {
    while (**input && isspace(**input)) {
        (*input)++;
    }
}

int parse_number(const char **input, struct Token *token) {
    char *end;
    double val = strtod(*input, &end);
    if (end == *input) {
        return 0;
    }
    token->type = TOKEN_NUMBER;
    size_t len = end - *input;
    if (len >= MAX_TOKEN_LEN) {
        return 0;
    }
    strncpy(token->value, *input, len);
    token->value[len] = '\0';
    *input = end;
    return 1;
}

int parse_identifier(const char **input, struct Token *token) {
    const char *start = *input;
    if (!isalpha(**input) && **input != '_') {
        return 0;
    }
    (*input)++;
    while (isalnum(**input) || **input == '_') {
        (*input)++;
    }
    size_t len = *input - start;
    if (len >= MAX_TOKEN_LEN) {
        return 0;
    }
    strncpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_operator(const char **input, struct Token *token) {
    if (!is_operator_char(**input)) {
        return 0;
    }
    token->value[0] = **input;
    token->value[1] = '\0';
    token->type = TOKEN_OPERATOR;
    (*input)++;
    return 1;
}

int parse_string(const char **input, struct Token *token) {
    if (**input != '"') {
        return 0;
    }
    (*input)++;
    const char *start = *input;
    while (**input && **input != '"') {
        (*input)++;
    }
    if (**input != '"') {
        return 0;
    }
    size_t len = *input - start;
    if (len >= MAX_TOKEN_LEN) {
        return 0;
    }
    strncpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_STRING;
    (*input)++;
    return 1;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int count = 0;
    while (*input && count < max_tokens) {
        skip_whitespace(&input);
        if (!*input) {
            break;
        }
        if (parse_number(&input, &tokens[count])) {
            count++;
        } else if (parse_identifier(&input, &tokens[count])) {
            count++;
        } else if (parse_operator(&input, &tokens[count])) {
            count++;
        } else if (parse_string(&input, &tokens[count])) {
            count++;
        } else {
            break;
        }
    }
    if (count < max_tokens) {
        tokens[count].type = TOKEN_EOF;
        strcpy(tokens[count].value, "");
        count++;
    }
    return count;
}

void print_token_type(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_EOF: printf("EOF"); break;
    }
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
        return 1;
    }
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    struct Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    printf("Tokens found: %d\n", token_count);
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: Type=", i);
        print_token_type(tokens[i].type);
        printf(", Value=\"%s\"\n", tokens[i].value);
    }
    return 0;
}