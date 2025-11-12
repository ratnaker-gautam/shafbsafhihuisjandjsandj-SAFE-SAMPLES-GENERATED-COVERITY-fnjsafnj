//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_NUMBER, TOKEN_STRING, TOKEN_SYMBOL, TOKEN_EOF };

struct Token {
    enum TokenType type;
    char value[64];
};

int is_symbol_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(const char **input) {
    while (**input && isspace(**input)) {
        (*input)++;
    }
}

int parse_token(const char **input, struct Token *token) {
    skip_whitespace(input);
    
    if (**input == '\0') {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }
    
    if (isdigit(**input)) {
        token->type = TOKEN_NUMBER;
        int i = 0;
        while (**input && isdigit(**input) && i < 63) {
            token->value[i++] = **input;
            (*input)++;
        }
        token->value[i] = '\0';
        return 1;
    }
    
    if (**input == '"') {
        token->type = TOKEN_STRING;
        (*input)++;
        int i = 0;
        while (**input && **input != '"' && i < 63) {
            token->value[i++] = **input;
            (*input)++;
        }
        if (**input == '"') {
            (*input)++;
        }
        token->value[i] = '\0';
        return 1;
    }
    
    if (is_symbol_char(**input)) {
        token->type = TOKEN_SYMBOL;
        int i = 0;
        while (**input && is_symbol_char(**input) && i < 63) {
            token->value[i++] = **input;
            (*input)++;
        }
        token->value[i] = '\0';
        return 1;
    }
    
    return 0;
}

void print_token(const struct Token *token) {
    switch (token->type) {
        case TOKEN_NUMBER:
            printf("NUMBER: %s\n", token->value);
            break;
        case TOKEN_STRING:
            printf("STRING: \"%s\"\n", token->value);
            break;
        case TOKEN_SYMBOL:
            printf("SYMBOL: %s\n", token->value);
            break;
        case TOKEN_EOF:
            printf("EOF\n");
            break;
    }
}

int main(void) {
    char input_buffer[1024];
    
    printf("Enter text to tokenize (max 1023 characters): ");
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
    struct Token token;
    int token_count = 0;
    
    printf("\nTokens:\n");
    while (parse_token(&input_ptr, &token)) {
        if (token.type == TOKEN_EOF) {
            break;
        }
        print_token(&token);
        token_count++;
        
        if (token_count > 100) {
            printf("Too many tokens, stopping\n");
            break;
        }
    }
    
    printf("Total tokens: %d\n", token_count);
    return 0;
}