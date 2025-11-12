//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
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

void print_token(const struct Token *token) {
    switch (token->type) {
        case TOKEN_NUMBER:
            printf("NUMBER: %s\n", token->value);
            break;
        case TOKEN_IDENTIFIER:
            printf("IDENTIFIER: %s\n", token->value);
            break;
        case TOKEN_OPERATOR:
            printf("OPERATOR: %s\n", token->value);
            break;
        case TOKEN_PAREN:
            printf("PAREN: %s\n", token->value);
            break;
        case TOKEN_EOF:
            printf("EOF\n");
            break;
        case TOKEN_INVALID:
            printf("INVALID: %s\n", token->value);
            break;
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
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    const char *input_ptr = input_buffer;
    struct Token token;
    int token_count = 0;
    
    printf("Tokens:\n");
    do {
        token = get_next_token(&input_ptr);
        print_token(&token);
        token_count++;
        
        if (token_count > 100) {
            fprintf(stderr, "Too many tokens\n");
            return 1;
        }
    } while (token.type != TOKEN_EOF && token.type != TOKEN_INVALID);
    
    if (token.type == TOKEN_INVALID) {
        fprintf(stderr, "Invalid token encountered\n");
        return 1;
    }
    
    printf("Total tokens processed: %d\n", token_count);
    return 0;
}