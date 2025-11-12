//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_PAREN,
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
    
    if (**input == '(' || **input == ')') {
        token.type = TOKEN_PAREN;
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

const char* token_type_to_string(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_PAREN: return "PAREN";
        case TOKEN_EOF: return "EOF";
        case TOKEN_INVALID: return "INVALID";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input_buffer[1024];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (strlen(input_buffer) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    const char *input_ptr = input_buffer;
    int token_count = 0;
    
    printf("\nToken stream:\n");
    printf("%-12s %-12s %s\n", "Token #", "Type", "Value");
    printf("------------ ------------ ------------\n");
    
    while (1) {
        struct Token token = get_next_token(&input_ptr);
        
        if (token.type == TOKEN_EOF) {
            break;
        }
        
        printf("%-12d %-12s %s\n", 
               ++token_count, 
               token_type_to_string(token.type),
               token.value);
        
        if (token.type == TOKEN_INVALID) {
            printf("Invalid token encountered. Stopping.\n");
            break;
        }
        
        if (token_count >= 100) {
            printf("Too many tokens. Stopping.\n");
            break;
        }
    }
    
    if (token_count == 0) {
        printf("No valid tokens found\n");
    } else {
        printf("\nTotal tokens: %d\n", token_count);
    }
    
    return 0;
}