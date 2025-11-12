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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '(' || c == ')';
}

struct Token get_next_token(const char *input, int *pos) {
    struct Token token = {TOKEN_INVALID, "", *pos};
    int len = strlen(input);
    
    while (*pos < len && isspace(input[*pos])) {
        (*pos)++;
    }
    
    token.position = *pos;
    
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
        token.value[0] = input[*pos];
        token.value[1] = '\0';
        (*pos)++;
    } else {
        token.type = TOKEN_INVALID;
        token.value[0] = input[*pos];
        token.value[1] = '\0';
        (*pos)++;
    }
    
    return token;
}

void parse_expression(const char *input, int *pos);

void parse_primary(const char *input, int *pos) {
    struct Token token = get_next_token(input, pos);
    
    if (token.type == TOKEN_NUMBER) {
        printf("NUMBER: %s\n", token.value);
    } else if (token.type == TOKEN_IDENTIFIER) {
        printf("IDENTIFIER: %s\n", token.value);
    } else if (token.type == TOKEN_OPERATOR && token.value[0] == '(') {
        parse_expression(input, pos);
        token = get_next_token(input, pos);
        if (token.type != TOKEN_OPERATOR || token.value[0] != ')') {
            printf("ERROR: Expected ')' at position %d\n", token.position);
        }
    } else {
        printf("ERROR: Unexpected token '%s' at position %d\n", token.value, token.position);
    }
}

void parse_term(const char *input, int *pos) {
    parse_primary(input, pos);
    
    struct Token token = get_next_token(input, pos);
    
    if (token.type == TOKEN_OPERATOR && (token.value[0] == '*' || token.value[0] == '/')) {
        printf("OPERATOR: %s\n", token.value);
        parse_term(input, pos);
    } else {
        *pos = token.position;
    }
}

void parse_expression(const char *input, int *pos) {
    parse_term(input, pos);
    
    struct Token token = get_next_token(input, pos);
    
    if (token.type == TOKEN_OPERATOR && (token.value[0] == '+' || token.value[0] == '-')) {
        printf("OPERATOR: %s\n", token.value);
        parse_expression(input, pos);
    } else {
        *pos = token.position;
    }
}

void parse_assignment(const char *input, int *pos) {
    struct Token token = get_next_token(input, pos);
    
    if (token.type == TOKEN_IDENTIFIER) {
        printf("IDENTIFIER: %s\n", token.value);
        
        token = get_next_token(input, pos);
        if (token.type == TOKEN_OPERATOR && token.value[0] == '=') {
            printf("OPERATOR: =\n");
            parse_expression(input, pos);
        } else {
            printf("ERROR: Expected '=' at position %d\n", token.position);
        }
    } else {
        printf("ERROR: Expected identifier at position %d\n", token.position);
    }
}

int main() {
    char input[256];
    
    printf("Enter an expression (e.g., 'x = 5 + 3 * (2 - 1)'): ");
    
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
    printf("\nParsing result:\n");
    parse_assignment(input, &pos);
    
    struct Token final_token = get_next_token(input, &pos);
    if (final_token.type != TOKEN_EOF) {
        printf("ERROR: Unexpected token '%s