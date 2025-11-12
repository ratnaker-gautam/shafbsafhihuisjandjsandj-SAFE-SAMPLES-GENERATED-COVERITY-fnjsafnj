//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_PAREN,
    TOKEN_EOF,
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char value[32];
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

struct Token get_next_token(const char **input) {
    struct Token token = {TOKEN_INVALID, ""};
    
    while (**input && isspace(**input)) {
        (*input)++;
    }
    
    if (**input == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char c = **input;
    
    if (isdigit(c)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (**input && (isdigit(**input) || **input == '.')) {
            if (i < 31) {
                token.value[i++] = **input;
            }
            (*input)++;
        }
        token.value[i] = '\0';
    } else if (isalpha(c)) {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (**input && (isalnum(**input) || **input == '_')) {
            if (i < 31) {
                token.value[i++] = **input;
            }
            (*input)++;
        }
        token.value[i] = '\0';
    } else if (is_operator_char(c)) {
        token.type = TOKEN_OPERATOR;
        token.value[0] = c;
        token.value[1] = '\0';
        (*input)++;
    } else if (c == '(' || c == ')') {
        token.type = TOKEN_PAREN;
        token.value[0] = c;
        token.value[1] = '\0';
        (*input)++;
    } else {
        token.type = TOKEN_INVALID;
        token.value[0] = c;
        token.value[1] = '\0';
        (*input)++;
    }
    
    return token;
}

void print_token(const struct Token *token) {
    switch (token->type) {
        case TOKEN_NUMBER:
            printf("NUMBER: %s\n", token->value);
            break;
        case TOKEN_IDENTIFIER:
            printf("IDENTIFIER: %s\n", token->value);
            break;
        case TOKEN_OPERATOR:
            printf("OPERATOR: %s\n", token->value);
            break;
        case TOKEN_PAREN:
            printf("PAREN: %s\n", token->value);
            break;
        case TOKEN_EOF:
            printf("EOF\n");
            break;
        case TOKEN_INVALID:
            printf("INVALID: %s\n", token->value);
            break;
    }
}

int main(void) {
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
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    const char *cursor = input;
    int token_count = 0;
    
    printf("Tokens:\n");
    while (1) {
        struct Token token = get_next_token(&cursor);
        print_token(&token);
        token_count++;
        
        if (token.type == TOKEN_EOF || token.type == TOKEN_INVALID) {
            break;
        }
        
        if (token_count > 100) {
            fprintf(stderr, "Too many tokens\n");
            return 1;
        }
    }
    
    return 0;
}