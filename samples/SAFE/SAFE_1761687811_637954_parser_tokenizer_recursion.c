//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <ctype.h>
#include <string.h>

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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
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
    
    if (isalpha((unsigned char)**input)) {
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

void parse_expression(const char **input);

void parse_factor(const char **input) {
    struct Token token = get_next_token(input);
    
    if (token.type == TOKEN_NUMBER) {
        printf("NUMBER: %s\n", token.value);
    } else if (token.type == TOKEN_IDENTIFIER) {
        printf("IDENTIFIER: %s\n", token.value);
    } else if (token.type == TOKEN_OPERATOR && token.value[0] == '(') {
        parse_expression(input);
        token = get_next_token(input);
        if (token.type != TOKEN_OPERATOR || token.value[0] != ')') {
            printf("ERROR: Expected ')'\n");
        }
    } else {
        printf("ERROR: Unexpected token: %s\n", token.value);
    }
}

void parse_term(const char **input) {
    parse_factor(input);
    
    struct Token token = get_next_token(input);
    if (token.type == TOKEN_OPERATOR && (token.value[0] == '*' || token.value[0] == '/')) {
        printf("OPERATOR: %s\n", token.value);
        parse_term(input);
    } else {
        (*input)--;
    }
}

void parse_expression(const char **input) {
    parse_term(input);
    
    struct Token token = get_next_token(input);
    if (token.type == TOKEN_OPERATOR && (token.value[0] == '+' || token.value[0] == '-')) {
        printf("OPERATOR: %s\n", token.value);
        parse_expression(input);
    } else {
        (*input)--;
    }
}

void parse_assignment(const char **input) {
    struct Token token = get_next_token(input);
    
    if (token.type == TOKEN_IDENTIFIER) {
        printf("IDENTIFIER: %s\n", token.value);
        
        token = get_next_token(input);
        if (token.type == TOKEN_OPERATOR && token.value[0] == '=') {
            printf("OPERATOR: =\n");
            parse_expression(input);
        } else {
            printf("ERROR: Expected '=' after identifier\n");
        }
    } else {
        printf("ERROR: Expected identifier at start\n");
    }
}

int main() {
    char input[256];
    
    printf("Enter an expression (e.g., 'x = 5 + 3 * (2 - 1)'): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    const char *input_ptr = input;
    printf("\nParsing tokens:\n");
    
    parse_assignment(&input_ptr);
    
    struct Token final_token = get_next_token(&input_ptr);
    if (final_token.type != TOKEN_EOF) {
        printf("ERROR: Unexpected token at end: %s\n", final_token.value);
    }
    
    return 0;
}