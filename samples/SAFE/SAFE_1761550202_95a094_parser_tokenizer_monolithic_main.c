//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_NUMBER, TOKEN_STRING, TOKEN_SYMBOL, TOKEN_EOF, TOKEN_ERROR };

struct Token {
    enum TokenType type;
    char text[64];
    double number_value;
};

int is_symbol_char(char c) {
    return isalnum(c) || c == '_' || c == '-';
}

struct Token get_next_token(const char **input) {
    struct Token token = {TOKEN_ERROR, "", 0.0};
    
    if (**input == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    while (isspace(**input)) {
        (*input)++;
    }
    
    if (**input == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    if (isdigit(**input) || **input == '.') {
        char *end;
        token.number_value = strtod(*input, &end);
        if (end != *input) {
            token.type = TOKEN_NUMBER;
            size_t len = end - *input;
            if (len >= sizeof(token.text)) len = sizeof(token.text) - 1;
            strncpy(token.text, *input, len);
            token.text[len] = '\0';
            *input = end;
            return token;
        }
    }
    
    if (**input == '"') {
        (*input)++;
        size_t i = 0;
        while (**input != '"' && **input != '\0' && i < sizeof(token.text) - 1) {
            token.text[i++] = **input;
            (*input)++;
        }
        if (**input == '"') {
            (*input)++;
            token.text[i] = '\0';
            token.type = TOKEN_STRING;
            return token;
        } else {
            token.type = TOKEN_ERROR;
            return token;
        }
    }
    
    if (is_symbol_char(**input)) {
        size_t i = 0;
        while (is_symbol_char(**input) && i < sizeof(token.text) - 1) {
            token.text[i++] = **input;
            (*input)++;
        }
        token.text[i] = '\0';
        token.type = TOKEN_SYMBOL;
        return token;
    }
    
    token.text[0] = **input;
    token.text[1] = '\0';
    token.type = TOKEN_SYMBOL;
    (*input)++;
    return token;
}

int main(void) {
    char input_buffer[1024];
    
    printf("Enter text to tokenize: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    const char *current = input_buffer;
    int token_count = 0;
    
    printf("Tokens:\n");
    printf("-------\n");
    
    while (1) {
        struct Token token = get_next_token(&current);
        
        if (token.type == TOKEN_EOF) {
            break;
        }
        
        if (token.type == TOKEN_ERROR) {
            printf("ERROR: Invalid token\n");
            break;
        }
        
        token_count++;
        
        switch (token.type) {
            case TOKEN_NUMBER:
                printf("NUMBER: %s (value: %g)\n", token.text, token.number_value);
                break;
            case TOKEN_STRING:
                printf("STRING: \"%s\"\n", token.text);
                break;
            case TOKEN_SYMBOL:
                printf("SYMBOL: %s\n", token.text);
                break;
            default:
                break;
        }
        
        if (token_count >= 100) {
            printf("Maximum token count reached\n");
            break;
        }
    }
    
    printf("-------\n");
    printf("Total tokens: %d\n", token_count);
    
    return 0;
}