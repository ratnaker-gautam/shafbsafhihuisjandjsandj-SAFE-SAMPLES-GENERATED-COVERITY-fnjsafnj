//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_ERROR
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(char **input) {
    while (**input && isspace(**input)) {
        (*input)++;
    }
}

Token get_next_token(char **input) {
    Token token = {TOKEN_ERROR, ""};
    skip_whitespace(input);
    
    if (**input == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char *start = *input;
    
    if (isdigit(**input)) {
        token.type = TOKEN_NUMBER;
        char *ptr = *input;
        while (*ptr && isdigit(*ptr)) {
            ptr++;
        }
        size_t len = ptr - *input;
        if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
        strncpy(token.value, *input, len);
        token.value[len] = '\0';
        *input = ptr;
        return token;
    }
    
    if (isalpha(**input) || **input == '_') {
        token.type = TOKEN_IDENTIFIER;
        char *ptr = *input;
        while (*ptr && (isalnum(*ptr) || *ptr == '_')) {
            ptr++;
        }
        size_t len = ptr - *input;
        if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
        strncpy(token.value, *input, len);
        token.value[len] = '\0';
        *input = ptr;
        return token;
    }
    
    if (is_operator_char(**input)) {
        token.type = TOKEN_OPERATOR;
        char *ptr = *input;
        while (*ptr && is_operator_char(*ptr)) {
            ptr++;
        }
        size_t len = ptr - *input;
        if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
        strncpy(token.value, *input, len);
        token.value[len] = '\0';
        *input = ptr;
        return token;
    }
    
    if (**input == '"') {
        token.type = TOKEN_STRING;
        (*input)++;
        char *ptr = *input;
        while (*ptr && *ptr != '"') {
            ptr++;
        }
        if (*ptr != '"') {
            token.type = TOKEN_ERROR;
            return token;
        }
        size_t len = ptr - *input;
        if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
        strncpy(token.value, *input, len);
        token.value[len] = '\0';
        *input = ptr + 1;
        return token;
    }
    
    token.type = TOKEN_ERROR;
    return token;
}

int tokenize(char *input, Token *tokens, int max_tokens) {
    if (!input || !tokens || max_tokens <= 0) return -1;
    
    int count = 0;
    char *ptr = input;
    
    while (count < max_tokens) {
        Token token = get_next_token(&ptr);
        if (token.type == TOKEN_EOF) break;
        if (token.type == TOKEN_ERROR) return -1;
        
        tokens[count] = token;
        count++;
    }
    
    if (count < max_tokens) {
        tokens[count].type = TOKEN_EOF;
        tokens[count].value[0] = '\0';
    }
    
    return count;
}

void print_token_type(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_EOF: printf("EOF"); break;
        default: printf("ERROR"); break;
    }
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count < 0) {
        fprintf(stderr, "Tokenization failed\n");
        return 1;
    }
    
    printf("Tokens found: %d\n", token_count);
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: ", i + 1