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

int get_next_token(const char *input, int *pos, struct Token *token) {
    int len = strlen(input);
    
    while (*pos < len && isspace(input[*pos])) {
        (*pos)++;
    }
    
    if (*pos >= len) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }
    
    char current = input[*pos];
    
    if (isdigit(current)) {
        int i = 0;
        while (*pos < len && (isdigit(input[*pos]) || input[*pos] == '.')) {
            if (i < 31) {
                token->value[i++] = input[*pos];
            }
            (*pos)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_NUMBER;
        return 1;
    }
    
    if (isalpha(current) || current == '_') {
        int i = 0;
        while (*pos < len && (isalnum(input[*pos]) || input[*pos] == '_')) {
            if (i < 31) {
                token->value[i++] = input[*pos];
            }
            (*pos)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_IDENTIFIER;
        return 1;
    }
    
    if (is_operator_char(current)) {
        token->value[0] = current;
        token->value[1] = '\0';
        token->type = TOKEN_OPERATOR;
        (*pos)++;
        return 1;
    }
    
    if (current == '(' || current == ')') {
        token->value[0] = current;
        token->value[1] = '\0';
        token->type = TOKEN_PAREN;
        (*pos)++;
        return 1;
    }
    
    token->value[0] = current;
    token->value[1] = '\0';
    token->type = TOKEN_INVALID;
    (*pos)++;
    return 0;
}

void print_token(struct Token token) {
    switch (token.type) {
        case TOKEN_NUMBER:
            printf("NUMBER: %s\n", token.value);
            break;
        case TOKEN_IDENTIFIER:
            printf("IDENTIFIER: %s\n", token.value);
            break;
        case TOKEN_OPERATOR:
            printf("OPERATOR: %s\n", token.value);
            break;
        case TOKEN_PAREN:
            printf("PAREN: %s\n", token.value);
            break;
        case TOKEN_EOF:
            printf("EOF\n");
            break;
        case TOKEN_INVALID:
            printf("INVALID: %s\n", token.value);
            break;
    }
}

int main(void) {
    char input[256];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int pos = 0;
    struct Token token;
    int valid = 1;
    
    printf("Tokens:\n");
    while (get_next_token(input, &pos, &token)) {
        print_token(token);
        if (token.type == TOKEN_INVALID) {
            valid = 0;
        }
        if (token.type == TOKEN_EOF) {
            break;
        }
    }
    
    if (!valid) {
        fprintf(stderr, "Input contains invalid tokens\n");
        return 1;
    }
    
    return 0;
}