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
    
    (*input)++;
    return token;
}

void parse_expression(const char **input);

void parse_primary(const char **input) {
    struct Token token = get_next_token(input);
    
    if (token.type == TOKEN_NUMBER) {
        printf("NUMBER: %s\n", token.value);
    } else if (token.type == TOKEN_IDENTIFIER) {
        printf("IDENTIFIER: %s\n", token.value);
    } else {
        printf("ERROR: Expected number or identifier\n");
    }
}

void parse_term(const char **input) {
    parse_primary(input);
    
    struct Token token = get_next_token(input);
    while (token.type == TOKEN_OPERATOR && 
           (token.value[0] == '*' || token.value[0] == '/')) {
        printf("OPERATOR: %s\n", token.value);
        parse_primary(input);
        token = get_next_token(input);
    }
    
    (*input)--;
}

void parse_expression(const char **input) {
    parse_term(input);
    
    struct Token token = get_next_token(input);
    while (token.type == TOKEN_OPERATOR && 
           (token.value[0] == '+' || token.value[0] == '-')) {
        printf("OPERATOR: %s\n", token.value);
        parse_term(input);
        token = get_next_token(input);
    }
    
    (*input)--;
}

void parse_statement(const char **input) {
    struct Token token = get_next_token(input);
    
    if (token.type == TOKEN_IDENTIFIER) {
        printf("IDENTIFIER: %s\n", token.value);
        
        token = get_next_token(input);
        if (token.type == TOKEN_OPERATOR && token.value[0] == '=') {
            printf("OPERATOR: %s\n", token.value);
            parse_expression(input);
        } else {
            printf("ERROR: Expected '=' after identifier\n");
            return;
        }
        
        token = get_next_token(input);
        if (token.type != TOKEN_EOF) {
            printf("ERROR: Expected end of input\n");
        }
    } else {
        printf("ERROR: Expected identifier at start\n");
    }
}

int main() {
    char input_buffer[1024];
    
    printf("Enter an expression (e.g., 'x = 10 + 5 * 2'): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (strlen(input_buffer) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    const char *input_ptr = input_buffer;
    parse_statement(&input_ptr);
    
    return 0;
}