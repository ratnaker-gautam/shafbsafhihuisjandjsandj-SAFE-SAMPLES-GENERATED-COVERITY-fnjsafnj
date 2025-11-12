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

void parse_primary(const char **input) {
    struct Token token = get_next_token(input);
    
    if (token.type == TOKEN_NUMBER) {
        printf("NUMBER: %s\n", token.value);
    } else if (token.type == TOKEN_IDENTIFIER) {
        printf("IDENTIFIER: %s\n", token.value);
    } else {
        printf("ERROR: Expected number or identifier, got '%.32s'\n", token.value);
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
    
    (*input) -= strlen(token.value);
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
    
    (*input) -= strlen(token.value);
}

void parse_statement(const char **input) {
    struct Token token = get_next_token(input);
    
    if (token.type == TOKEN_IDENTIFIER) {
        printf("IDENTIFIER: %s\n", token.value);
        
        struct Token next_token = get_next_token(input);
        if (next_token.type == TOKEN_OPERATOR && next_token.value[0] == '=') {
            printf("OPERATOR: =\n");
            parse_expression(input);
            
            struct Token end_token = get_next_token(input);
            if (end_token.type == TOKEN_OPERATOR && end_token.value[0] == ';') {
                printf("END STATEMENT\n");
            } else {
                printf("ERROR: Expected ';' at end of statement\n");
            }
        } else {
            printf("ERROR: Expected '=' after identifier\n");
        }
    } else {
        printf("ERROR: Expected identifier at start of statement\n");
    }
}

int main() {
    char input[256];
    
    printf("Enter an expression (e.g., 'x = 5 + 3 * 2;'): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    const char *input_ptr = input;
    printf("\nParsing tokens:\n");
    
    parse_statement(&input_ptr);
    
    struct Token final_token = get_next_token(&input_ptr);
    if (final_token.type != TOKEN_EOF) {
        printf("ERROR: Unexpected token '%.32s' at end\n", final_token.value);
    }
    
    return 0;
}