//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_EOF,
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char value[32];
    int position;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

struct Token get_next_token(const char *input, int *pos) {
    struct Token token = {TOKEN_INVALID, "", *pos};
    int i = 0;
    
    while (isspace(input[*pos])) {
        (*pos)++;
    }
    
    if (input[*pos] == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    if (isdigit(input[*pos])) {
        token.type = TOKEN_NUMBER;
        while (isdigit(input[*pos]) && i < 31) {
            token.value[i++] = input[*pos];
            (*pos)++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    if (isalpha(input[*pos])) {
        token.type = TOKEN_IDENTIFIER;
        while ((isalnum(input[*pos]) || input[*pos] == '_') && i < 31) {
            token.value[i++] = input[*pos];
            (*pos)++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    if (is_operator_char(input[*pos])) {
        token.type = TOKEN_OPERATOR;
        token.value[i++] = input[*pos];
        (*pos)++;
        token.value[i] = '\0';
        return token;
    }
    
    token.value[i++] = input[*pos];
    (*pos)++;
    token.value[i] = '\0';
    return token;
}

void parse_expression(const char *input, int *pos);

void parse_primary(const char *input, int *pos) {
    struct Token token = get_next_token(input, pos);
    
    if (token.type == TOKEN_NUMBER) {
        printf("NUMBER: %s\n", token.value);
    } else if (token.type == TOKEN_IDENTIFIER) {
        printf("IDENTIFIER: %s\n", token.value);
    } else if (token.value[0] == '(') {
        parse_expression(input, pos);
        token = get_next_token(input, pos);
        if (token.value[0] != ')') {
            printf("ERROR: Expected ')' at position %d\n", token.position);
        }
    } else {
        printf("ERROR: Unexpected token '%s' at position %d\n", token.value, token.position);
    }
}

void parse_term(const char *input, int *pos) {
    parse_primary(input, pos);
    
    struct Token token = get_next_token(input, pos);
    while (token.type == TOKEN_OPERATOR && (token.value[0] == '*' || token.value[0] == '/')) {
        printf("OPERATOR: %s\n", token.value);
        parse_primary(input, pos);
        token = get_next_token(input, pos);
    }
    (*pos) = token.position;
}

void parse_expression(const char *input, int *pos) {
    parse_term(input, pos);
    
    struct Token token = get_next_token(input, pos);
    while (token.type == TOKEN_OPERATOR && (token.value[0] == '+' || token.value[0] == '-')) {
        printf("OPERATOR: %s\n", token.value);
        parse_term(input, pos);
        token = get_next_token(input, pos);
    }
    (*pos) = token.position;
}

int main() {
    char input[256];
    
    printf("Enter an arithmetic expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    int pos = 0;
    printf("Parsing expression: %s\n", input);
    parse_expression(input, &pos);
    
    struct Token token = get_next_token(input, &pos);
    if (token.type != TOKEN_EOF) {
        printf("ERROR: Unexpected token '%s' at position %d\n", token.value, token.position);
        return 1;
    }
    
    printf("Parsing completed successfully\n");
    return 0;
}