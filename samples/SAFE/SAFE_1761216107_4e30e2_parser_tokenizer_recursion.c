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
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
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
    
    if (isdigit(**input)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (**input && isdigit(**input) && i < 31) {
            token.value[i++] = **input;
            (*input)++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    if (isalpha(**input)) {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (**input && (isalnum(**input) || **input == '_') && i < 31) {
            token.value[i++] = **input;
            (*input)++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    if (is_operator_char(**input)) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (**input && is_operator_char(**input) && i < 31) {
            token.value[i++] = **input;
            (*input)++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    token.type = TOKEN_INVALID;
    token.value[0] = **input;
    token.value[1] = '\0';
    (*input)++;
    return token;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "EOF", "INVALID"
    };
    printf("%-12s: %s\n", type_names[token.type], token.value);
}

void parse_expression(const char **input);

void parse_primary(const char **input) {
    struct Token token = get_next_token(input);
    
    if (token.type == TOKEN_NUMBER || token.type == TOKEN_IDENTIFIER) {
        print_token(token);
        return;
    }
    
    if (token.type == TOKEN_OPERATOR && token.value[0] == '(') {
        printf("(\n");
        parse_expression(input);
        token = get_next_token(input);
        if (token.type == TOKEN_OPERATOR && token.value[0] == ')') {
            printf(")\n");
        } else {
            printf("ERROR: Expected ')'\n");
        }
        return;
    }
    
    printf("ERROR: Unexpected token ");
    print_token(token);
}

void parse_expression(const char **input) {
    parse_primary(input);
    
    struct Token token = get_next_token(input);
    
    if (token.type == TOKEN_OPERATOR && 
        (strcmp(token.value, "+") == 0 || 
         strcmp(token.value, "-") == 0 ||
         strcmp(token.value, "*") == 0 ||
         strcmp(token.value, "/") == 0)) {
        print_token(token);
        parse_expression(input);
    } else {
        *input = *input - strlen(token.value);
    }
}

int main() {
    char input[256];
    
    printf("Enter expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    const char *cursor = input;
    printf("\nToken stream:\n");
    
    while (1) {
        struct Token token = get_next_token(&cursor);
        if (token.type == TOKEN_EOF) {
            break;
        }
        print_token(token);
    }
    
    printf("\nParse tree:\n");
    cursor = input;
    parse_expression(&cursor);
    
    struct Token final_token = get_next_token(&cursor);
    if (final_token.type != TOKEN_EOF) {
        printf("ERROR: Extra tokens after expression\n");
        return 1;
    }
    
    return 0;
}