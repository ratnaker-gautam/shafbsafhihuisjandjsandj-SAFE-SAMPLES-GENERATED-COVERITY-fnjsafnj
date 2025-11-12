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
    TOKEN_INVALID
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
    Token token = {TOKEN_INVALID, ""};
    skip_whitespace(input);
    
    if (**input == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char *start = *input;
    
    if (isdigit(**input)) {
        token.type = TOKEN_NUMBER;
        char *ptr = token.value;
        while (**input && isdigit(**input)) {
            if (ptr - token.value < MAX_TOKEN_LEN - 1) {
                *ptr++ = **input;
            }
            (*input)++;
        }
        *ptr = '\0';
        return token;
    }
    
    if (isalpha(**input) || **input == '_') {
        token.type = TOKEN_IDENTIFIER;
        char *ptr = token.value;
        while (**input && (isalnum(**input) || **input == '_')) {
            if (ptr - token.value < MAX_TOKEN_LEN - 1) {
                *ptr++ = **input;
            }
            (*input)++;
        }
        *ptr = '\0';
        return token;
    }
    
    if (**input == '"') {
        token.type = TOKEN_STRING;
        (*input)++;
        char *ptr = token.value;
        while (**input && **input != '"') {
            if (ptr - token.value < MAX_TOKEN_LEN - 1) {
                *ptr++ = **input;
            }
            (*input)++;
        }
        *ptr = '\0';
        if (**input == '"') {
            (*input)++;
        }
        return token;
    }
    
    if (is_operator_char(**input)) {
        token.type = TOKEN_OPERATOR;
        char *ptr = token.value;
        while (**input && is_operator_char(**input)) {
            if (ptr - token.value < MAX_TOKEN_LEN - 1) {
                *ptr++ = **input;
            }
            (*input)++;
        }
        *ptr = '\0';
        return token;
    }
    
    (*input)++;
    return token;
}

const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "INVALID";
    }
}

int tokenize_input(char *input, Token *tokens, int max_tokens) {
    int count = 0;
    char *ptr = input;
    
    while (count < max_tokens) {
        Token token = get_next_token(&ptr);
        tokens[count] = token;
        count++;
        
        if (token.type == TOKEN_EOF) {
            break;
        }
    }
    
    return count;
}

int main(void) {
    char input[1024];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *newline = strchr(input, '\n');
    if (newline) {
        *newline = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];
    int token_count = tokenize_input(input, tokens, MAX_TOKENS);
    
    printf("Tokens found: %d\n", token_count);
    printf("Type\tValue\n");
    printf("----\t-----\n");
    
    for (int i = 0; i < token_count; i++) {
        printf("%s\t'%s'\n", token_type_to_string(tokens[i].type), tokens[i].value);
    }
    
    return 0;
}