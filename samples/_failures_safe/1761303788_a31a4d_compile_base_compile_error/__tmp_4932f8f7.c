//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
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

int parse_string(char **input, Token *token) {
    char *start = *input;
    (*input)++;
    
    while (**input && **input != '"') {
        if (**input == '\\' && *(*input + 1)) {
            (*input) += 2;
        } else {
            (*input)++;
        }
    }
    
    if (**input != '"') {
        return 0;
    }
    
    size_t len = *input - start - 1;
    if (len >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    strncpy(token->value, start + 1, len);
    token->value[len] = '\0';
    token->type = TOKEN_STRING;
    (*input)++;
    return 1;
}

int parse_number(char **input, Token *token) {
    char *start = *input;
    
    while (**input && isdigit(**input)) {
        (*input)++;
    }
    
    if (**input == '.' && isdigit(*(*input + 1))) {
        (*input)++;
        while (**input && isdigit(**input)) {
            (*input)++;
        }
    }
    
    size_t len = *input - start;
    if (len >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    strncpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_identifier(char **input, Token *token) {
    char *start = *input;
    
    if (!isalpha(**input) && **input != '_') {
        return 0;
    }
    
    (*input)++;
    
    while (**input && (isalnum(**input) || **input == '_')) {
        (*input)++;
    }
    
    size_t len = *input - start;
    if (len >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    strncpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_operator(char **input, Token *token) {
    char *start = *input;
    
    if (!is_operator_char(**input)) {
        return 0;
    }
    
    (*input)++;
    
    if (is_operator_char(**input)) {
        (*input)++;
    }
    
    size_t len = *input - start;
    if (len >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    strncpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

int get_next_token(char **input, Token *token) {
    skip_whitespace(input);
    
    if (!**input) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }
    
    if (**input == '"') {
        return parse_string(input, token);
    }
    
    if (isdigit(**input)) {
        return parse_number(input, token);
    }
    
    if (is_operator_char(**input)) {
        return parse_operator(input, token);
    }
    
    if (isalpha(**input) || **input == '_') {
        return parse_identifier(input, token);
    }
    
    return 0;
}

const char* token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input_buffer[1024];
    Token tokens[MAX_TOKENS];
    int token_count = 0;
    
    printf("Enter expression to tokenize: ");
    if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *input_ptr = input_buffer;
    
    while (token_count < MAX_TOKENS - 1) {
        Token token;
        if (!get_next_token(&input_ptr, &token)) {
            fprintf(stderr, "Invalid token at position: %ld\n", input_ptr - input_buffer);
            return 1;
        }
        
        if (token.type == TOKEN_EOF) {
            break;
        }
        
        tokens[token_count] = token;
        token_count++;
    }
    
    if (token_count >= MAX