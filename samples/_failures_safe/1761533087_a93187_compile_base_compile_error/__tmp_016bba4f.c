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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '(' || c == ')';
}

struct Token get_next_token(const char *input, int *pos) {
    struct Token token = {TOKEN_INVALID, "", *pos};
    int i = 0;
    int input_len = strlen(input);
    
    if (*pos >= input_len || input[*pos] == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    while (*pos < input_len && isspace((unsigned char)input[*pos])) {
        (*pos)++;
        token.position = *pos;
    }
    
    if (*pos >= input_len || input[*pos] == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    if (isdigit((unsigned char)input[*pos])) {
        token.type = TOKEN_NUMBER;
        while (*pos < input_len && isdigit((unsigned char)input[*pos]) && i < 31) {
            token.value[i++] = input[*pos];
            (*pos)++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    if (isalpha((unsigned char)input[*pos]) || input[*pos] == '_') {
        token.type = TOKEN_IDENTIFIER;
        while (*pos < input_len && (isalnum((unsigned char)input[*pos]) || input[*pos] == '_') && i < 31) {
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
        if (*pos < input_len && is_operator_char(input[*pos]) && i < 31) {
            token.value[i++] = input[*pos];
            (*pos)++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    token.value[0] = input[*pos];
    token.value[1] = '\0';
    (*pos)++;
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
        printf("Entering subexpression\n");
        parse_expression(input, pos);
        token = get_next_token(input, pos);
        if (token.type != TOKEN_OPERATOR || token.value[0] != ')') {
            printf("Error: Expected closing parenthesis at position %d\n", *pos);
        } else {
            printf("Exiting subexpression\n");
        }
    } else {
        printf("Error: Unexpected token at position %d\n", token.position);
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
    } else {
        *pos = token.position;
        if (strlen(token.value) > 0) {
            *pos -= strlen(token.value);
        }
    }
}

int main() {
    char input[256];
    printf("Enter expression to parse: ");
    
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
    
    printf("Parsing: %s\n", input);
    printf("Token stream:\n");
    
    int pos = 0;
    struct Token token;
    
    do {
        token = get_next_token(input, &pos);