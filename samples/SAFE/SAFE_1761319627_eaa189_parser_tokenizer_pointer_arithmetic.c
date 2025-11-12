//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
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
        token.value[0] = **input;
        token.value[1] = '\0';
        (*input)++;
        return token;
    }
    
    if (**input == '"') {
        token.type = TOKEN_STRING;
        (*input)++;
        char *ptr = *input;
        char *dest = token.value;
        size_t count = 0;
        
        while (*ptr && *ptr != '"' && count < MAX_TOKEN_LEN - 1) {
            *dest++ = *ptr++;
            count++;
        }
        *dest = '\0';
        
        if (*ptr == '"') {
            *input = ptr + 1;
        } else {
            token.type = TOKEN_ERROR;
        }
        return token;
    }
    
    token.type = TOKEN_ERROR;
    return token;
}

int tokenize(char *input, Token *tokens, int max_tokens) {
    int count = 0;
    
    while (*input && count < max_tokens) {
        Token token = get_next_token(&input);
        if (token.type == TOKEN_EOF) {
            break;
        }
        if (token.type == TOKEN_ERROR) {
            return -1;
        }
        tokens[count] = token;
        count++;
    }
    
    if (count < max_tokens) {
        tokens[count].type = TOKEN_EOF;
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
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
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
    
    if (token_count == -1) {
        fprintf(stderr, "Error during tokenization\n");
        return 1;
    }
    
    printf("Tokens found: %d\n", token_count);
    printf("Type\tValue\n");
    printf("----\t-----\n");
    
    for (int i = 0; i < token_count; i++) {
        print_token_type(tokens[i].type);
        printf("\t%s\n", tokens[i].value);
    }
    
    return 0;
}