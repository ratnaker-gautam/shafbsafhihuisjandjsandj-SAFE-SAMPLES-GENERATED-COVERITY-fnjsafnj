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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '(' || c == ')' || c == ';';
}

struct Token get_next_token(const char *input, int *pos) {
    struct Token token = {TOKEN_INVALID, "", *pos};
    int len = strlen(input);
    
    if (*pos >= len) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    while (*pos < len && isspace(input[*pos])) {
        (*pos)++;
        token.position = *pos;
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
        *pos = token.position - (int)strlen(token.value);
    }
}

void parse_expression(const char *input, int *pos) {
    parse_term(input, pos);
    
    struct Token token = get_next_token(input, pos);
    
    if (token.type == TOKEN_OPERATOR && (token.value[0] == '+' || token.value[0] == '-')) {
        printf("OPERATOR: %s\n", token.value);
        parse_expression(input, pos);
    } else {
        *pos = token.position - (int)strlen(token.value);
    }
}

void parse_statement(const char *input, int *pos) {
    struct Token token = get_next_token(input, pos);
    
    if (token.type == TOKEN_IDENTIFIER) {
        printf("ASSIGNMENT: %s\n", token.value);
        
        token = get_next_token(input, pos);
        if (token.type != TOKEN_OPERATOR || token.value[0] != '=') {
            printf("ERROR: Expected '=' at position %d\n", token.position);
            return;
        }
        printf("OPERATOR: =\n");
        
        parse_expression(input, pos);
        
        token = get_next_token(input, pos);
        if (token.type != TOKEN_OPERATOR || token.value[0] != ';') {
            printf("ERROR: Expected ';' at position %d\n", token.position);
        }
    } else {
        printf("ERROR: Expected identifier at position %d\n", token.position);
    }
}

int main() {
    char input[256];
    
    printf("Enter an expression (e.g., 'x = 5 + 3 * (2 - 1);'): ");
    
    if (fgets(input, sizeof(input), stdin) ==