//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: parser_tokenizer
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
    int line;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(const char **input, int *line) {
    while (**input != '\0' && isspace(**input)) {
        if (**input == '\n') {
            (*line)++;
        }
        (*input)++;
    }
}

int parse_number(const char **input, struct Token *token) {
    int i = 0;
    while (**input != '\0' && isdigit(**input)) {
        if (i >= MAX_TOKEN_LEN - 1) {
            return 0;
        }
        token->value[i++] = **input;
        (*input)++;
    }
    token->value[i] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_identifier(const char **input, struct Token *token) {
    int i = 0;
    while (**input != '\0' && (isalnum(**input) || **input == '_')) {
        if (i >= MAX_TOKEN_LEN - 1) {
            return 0;
        }
        token->value[i++] = **input;
        (*input)++;
    }
    token->value[i] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_operator(const char **input, struct Token *token) {
    int i = 0;
    while (**input != '\0' && is_operator_char(**input)) {
        if (i >= MAX_TOKEN_LEN - 1) {
            return 0;
        }
        token->value[i++] = **input;
        (*input)++;
    }
    token->value[i] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

int parse_string(const char **input, struct Token *token, int *line) {
    int i = 0;
    (*input)++;
    while (**input != '\0' && **input != '"') {
        if (**input == '\n') {
            (*line)++;
        }
        if (i >= MAX_TOKEN_LEN - 1) {
            return 0;
        }
        token->value[i++] = **input;
        (*input)++;
    }
    if (**input != '"') {
        return 0;
    }
    (*input)++;
    token->value[i] = '\0';
    token->type = TOKEN_STRING;
    return 1;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int line = 1;
    int token_count = 0;
    
    while (*input != '\0' && token_count < max_tokens) {
        skip_whitespace(&input, &line);
        
        if (*input == '\0') {
            break;
        }
        
        tokens[token_count].line = line;
        
        if (isdigit(*input)) {
            if (!parse_number(&input, &tokens[token_count])) {
                return -1;
            }
        } else if (isalpha(*input) || *input == '_') {
            if (!parse_identifier(&input, &tokens[token_count])) {
                return -1;
            }
        } else if (is_operator_char(*input)) {
            if (!parse_operator(&input, &tokens[token_count])) {
                return -1;
            }
        } else if (*input == '"') {
            if (!parse_string(&input, &tokens[token_count], &line)) {
                return -1;
            }
        } else {
            return -1;
        }
        
        token_count++;
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].line = line;
        strcpy(tokens[token_count].value, "");
        token_count++;
    }
    
    return token_count;
}

void print_token_type(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER:
            printf("NUMBER");
            break;
        case TOKEN_IDENTIFIER:
            printf("IDENTIFIER");
            break;
        case TOKEN_OPERATOR:
            printf("OPERATOR");
            break;
        case TOKEN_STRING:
            printf("STRING");
            break;
        case TOKEN_EOF:
            printf("EOF");
            break;
    }
}

void print_tokens(const struct Token *tokens, int count) {
    for (int i = 0; i < count; i++) {
        printf("Line %d: ", tokens[i].line);
        print_token_type(tokens[i].type);
        printf(" '%s'\n", tokens[i].value);
    }
}

int main(void) {
    char input[1024];
    struct Token tokens[MAX_TOKENS];
    
    printf("Enter input to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");