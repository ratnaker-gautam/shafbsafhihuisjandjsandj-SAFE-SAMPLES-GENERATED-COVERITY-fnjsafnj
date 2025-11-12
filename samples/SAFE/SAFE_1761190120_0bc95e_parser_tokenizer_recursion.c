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

void parse_expression(const char **input);

void parse_factor(const char **input) {
    struct Token token = get_next_token(input);
    
    if (token.type == TOKEN_NUMBER || token.type == TOKEN_IDENTIFIER) {
        printf("Factor: %s\n", token.value);
    } else if (token.type == TOKEN_OPERATOR && token.value[0] == '(') {
        parse_expression(input);
        token = get_next_token(input);
        if (!(token.type == TOKEN_OPERATOR && token.value[0] == ')')) {
            printf("Error: Expected ')'\n");
        }
    } else {
        printf("Error: Unexpected token in factor: %s\n", token.value);
    }
}

void parse_term(const char **input) {
    parse_factor(input);
    
    struct Token token = get_next_token(input);
    
    if (token.type == TOKEN_OPERATOR && (token.value[0] == '*' || token.value[0] == '/')) {
        printf("Operator: %s\n", token.value);
        parse_term(input);
    } else {
        *input -= strlen(token.value);
    }
}

void parse_expression(const char **input) {
    parse_term(input);
    
    struct Token token = get_next_token(input);
    
    if (token.type == TOKEN_OPERATOR && (token.value[0] == '+' || token.value[0] == '-')) {
        printf("Operator: %s\n", token.value);
        parse_expression(input);
    } else {
        *input -= strlen(token.value);
    }
}

void parse_statement(const char **input) {
    struct Token token = get_next_token(input);
    
    if (token.type == TOKEN_IDENTIFIER) {
        printf("Assignment to: %s\n", token.value);
        
        token = get_next_token(input);
        if (!(token.type == TOKEN_OPERATOR && token.value[0] == '=')) {
            printf("Error: Expected '=' after identifier\n");
            return;
        }
        
        printf("Operator: =\n");
        parse_expression(input);
        
        token = get_next_token(input);
        if (!(token.type == TOKEN_OPERATOR && token.value[0] == ';')) {
            printf("Error: Expected ';' after expression\n");
        }
    } else {
        printf("Error: Expected identifier at start of statement\n");
    }
}

int main() {
    char input_buffer[1024];
    
    printf("Enter a simple assignment statement (e.g., 'x = 5 + 3 * (2 - 1);'): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    input_buffer[strcspn(input_buffer, "\n")] = '\0';
    
    if (strlen(input_buffer) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    const char *input_ptr = input_buffer;
    
    printf("\nParsing result:\n");
    parse_statement(&input_ptr);
    
    struct Token final_token = get_next_token(&input_ptr);
    if (final_token.type != TOKEN_EOF) {
        printf("Error: Extra tokens after statement: %s\n", final_token.value);
    }
    
    return 0;
}