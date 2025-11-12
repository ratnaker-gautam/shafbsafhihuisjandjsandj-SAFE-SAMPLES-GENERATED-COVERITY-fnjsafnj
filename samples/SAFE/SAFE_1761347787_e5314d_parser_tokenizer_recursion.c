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
    
    if (**input == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    while (isspace((unsigned char)**input)) {
        (*input)++;
    }
    
    if (**input == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    if (isdigit((unsigned char)**input)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (isdigit((unsigned char)**input) && i < 31) {
            token.value[i++] = **input;
            (*input)++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    if (isalpha((unsigned char)**input) || **input == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while ((isalnum((unsigned char)**input) || **input == '_') && i < 31) {
            token.value[i++] = **input;
            (*input)++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    if (is_operator_char(**input)) {
        token.type = TOKEN_OPERATOR;
        token.value[0] = **input;
        token.value[1] = '\0';
        (*input)++;
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
        parse_expression(input);
        token = get_next_token(input);
        if (token.type != TOKEN_OPERATOR || token.value[0] != ')') {
            printf("Error: Expected ')'\n");
            return;
        }
        return;
    }
    
    printf("Error: Unexpected token ");
    print_token(token);
}

void parse_term(const char **input) {
    parse_primary(input);
    
    struct Token token = get_next_token(input);
    if (token.type == TOKEN_OPERATOR && 
        (token.value[0] == '*' || token.value[0] == '/')) {
        print_token(token);
        parse_primary(input);
        parse_term(input);
    } else {
        *input -= strlen(token.value);
    }
}

void parse_expression(const char **input) {
    parse_term(input);
    
    struct Token token = get_next_token(input);
    if (token.type == TOKEN_OPERATOR && 
        (token.value[0] == '+' || token.value[0] == '-')) {
        print_token(token);
        parse_term(input);
        parse_expression(input);
    } else {
        *input -= strlen(token.value);
    }
}

int main() {
    char input[256];
    
    printf("Enter expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    const char *cursor = input;
    printf("\nToken stream:\n");
    
    while (1) {
        struct Token token = get_next_token(&cursor);
        print_token(token);
        if (token.type == TOKEN_EOF || token.type == TOKEN_INVALID) {
            break;
        }
    }
    
    printf("\nParse tree:\n");
    cursor = input;
    parse_expression(&cursor);
    
    struct Token final_token = get_next_token(&cursor);
    if (final_token.type != TOKEN_EOF) {
        printf("Error: Extra tokens after expression\n");
    }
    
    return 0;
}