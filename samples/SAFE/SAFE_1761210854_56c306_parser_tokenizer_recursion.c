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
        token.value[0] = input[*pos];
        token.value[1] = '\0';
        (*pos)++;
    }
    
    return token;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "EOF", "INVALID"
    };
    printf("Token: %-12s Value: %-10s Position: %d\n", 
           type_names[token.type], token.value, token.position);
}

void parse_expression(const char *input, int *pos);

void parse_primary(const char *input, int *pos) {
    struct Token token = get_next_token(input, pos);
    
    if (token.type == TOKEN_NUMBER) {
        printf("Found number: %s\n", token.value);
    } else if (token.type == TOKEN_IDENTIFIER) {
        printf("Found identifier: %s\n", token.value);
    } else if (token.type == TOKEN_OPERATOR && token.value[0] == '(') {
        printf("Entering parentheses\n");
        parse_expression(input, pos);
        token = get_next_token(input, pos);
        if (token.type != TOKEN_OPERATOR || token.value[0] != ')') {
            printf("Error: Expected closing parenthesis\n");
        } else {
            printf("Exiting parentheses\n");
        }
    } else {
        printf("Error: Unexpected token in primary expression\n");
        print_token(token);
    }
}

void parse_expression(const char *input, int *pos) {
    parse_primary(input, pos);
    
    struct Token token = get_next_token(input, pos);
    
    if (token.type == TOKEN_OPERATOR && 
        (strcmp(token.value, "+") == 0 || strcmp(token.value, "-") == 0 || 
         strcmp(token.value, "*") == 0 || strcmp(token.value, "/") == 0)) {
        printf("Found operator: %s\n", token.value);
        parse_expression(input, pos);
    } else if (token.type != TOKEN_EOF && token.type != TOKEN_INVALID) {
        (*pos) = token.position;
    }
}

int main() {
    char input[256];
    
    printf("Enter an expression to parse: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    printf("Parsing: %s\n", input);
    printf("Token stream:\n");
    
    int pos = 0;
    struct Token token;
    
    do {
        token = get_next_token(input, &pos);
        print_token(token);
    } while (token.type != TOKEN_EOF && token.type != TOKEN_INVALID);
    
    printf("\nRecursive descent parsing:\n");
    pos = 0;
    parse_expression(input, &pos);
    
    token = get_next_token(input, &pos);
    if (token.type != TOKEN_EOF) {
        printf("Error: Unexpected tokens at end of expression\n");
        print_token(token);
    }
    
    return 0;
}