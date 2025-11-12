//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
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
    int len = strlen(input);
    
    while (*pos < len && isspace(input[*pos])) {
        (*pos)++;
    }
    
    if (*pos >= len) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char current = input[*pos];
    
    if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (*pos < len && i < 31 && isdigit(input[*pos])) {
            token.value[i++] = input[*pos];
            (*pos)++;
        }
        token.value[i] = '\0';
    } else if (isalpha(current)) {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (*pos < len && i < 31 && (isalnum(input[*pos]) || input[*pos] == '_')) {
            token.value[i++] = input[*pos];
            (*pos)++;
        }
        token.value[i] = '\0';
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (*pos < len && i < 31 && is_operator_char(input[*pos])) {
            token.value[i++] = input[*pos];
            (*pos)++;
        }
        token.value[i] = '\0';
    } else {
        token.value[0] = current;
        token.value[1] = '\0';
        (*pos)++;
    }
    
    token.position = *pos;
    return token;
}

void parse_expression(const char *input, int *pos);

void parse_primary(const char *input, int *pos) {
    struct Token token = get_next_token(input, pos);
    
    if (token.type == TOKEN_NUMBER) {
        printf("Number: %s\n", token.value);
    } else if (token.type == TOKEN_IDENTIFIER) {
        printf("Identifier: %s\n", token.value);
    } else if (token.type == TOKEN_OPERATOR && token.value[0] == '(') {
        printf("Opening parenthesis\n");
        parse_expression(input, pos);
        token = get_next_token(input, pos);
        if (token.type != TOKEN_OPERATOR || token.value[0] != ')') {
            printf("Error: Expected closing parenthesis at position %d\n", token.position);
            return;
        }
        printf("Closing parenthesis\n");
    } else {
        printf("Error: Unexpected token '%s' at position %d\n", token.value, token.position);
    }
}

void parse_term(const char *input, int *pos) {
    parse_primary(input, pos);
    
    struct Token token = get_next_token(input, pos);
    
    if (token.type == TOKEN_OPERATOR && (token.value[0] == '*' || token.value[0] == '/')) {
        printf("Operator: %s\n", token.value);
        parse_term(input, pos);
    } else {
        *pos = token.position - strlen(token.value);
    }
}

void parse_expression(const char *input, int *pos) {
    parse_term(input, pos);
    
    struct Token token = get_next_token(input, pos);
    
    if (token.type == TOKEN_OPERATOR && (token.value[0] == '+' || token.value[0] == '-')) {
        printf("Operator: %s\n", token.value);
        parse_expression(input, pos);
    } else {
        *pos = token.position - strlen(token.value);
    }
}

int main() {
    char input[256];
    
    printf("Enter an arithmetic expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    int pos = 0;
    printf("Parsing expression: %s\n", input);
    parse_expression(input, &pos);
    
    struct Token token = get_next_token(input, &pos);
    if (token.type != TOKEN_EOF) {
        printf("Error: Unexpected token '%s' at end of expression\n", token.value);
        return 1;
    }
    
    printf("Parsing completed successfully\n");
    return 0;
}