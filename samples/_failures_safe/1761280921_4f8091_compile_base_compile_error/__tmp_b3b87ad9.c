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

Token get_number_token(char **input) {
    Token token = {TOKEN_NUMBER, ""};
    char *start = *input;
    int has_dot = 0;
    
    while (**input && (isdigit(**input) || **input == '.')) {
        if (**input == '.') {
            if (has_dot) break;
            has_dot = 1;
        }
        (*input)++;
    }
    
    size_t len = *input - start;
    if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
    strncpy(token.value, start, len);
    token.value[len] = '\0';
    
    return token;
}

Token get_identifier_token(char **input) {
    Token token = {TOKEN_IDENTIFIER, ""};
    char *start = *input;
    
    while (**input && (isalnum(**input) || **input == '_')) {
        (*input)++;
    }
    
    size_t len = *input - start;
    if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
    strncpy(token.value, start, len);
    token.value[len] = '\0';
    
    return token;
}

Token get_operator_token(char **input) {
    Token token = {TOKEN_OPERATOR, ""};
    char *start = *input;
    
    while (**input && is_operator_char(**input)) {
        (*input)++;
    }
    
    size_t len = *input - start;
    if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
    strncpy(token.value, start, len);
    token.value[len] = '\0';
    
    return token;
}

Token get_string_token(char **input) {
    Token token = {TOKEN_STRING, ""};
    char *start = *input;
    
    if (**input != '"') {
        token.type = TOKEN_ERROR;
        return token;
    }
    
    (*input)++;
    start = *input;
    
    while (**input && **input != '"') {
        (*input)++;
    }
    
    if (**input != '"') {
        token.type = TOKEN_ERROR;
        return token;
    }
    
    size_t len = *input - start;
    if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
    strncpy(token.value, start, len);
    token.value[len] = '\0';
    
    (*input)++;
    return token;
}

int tokenize(char *input, Token *tokens, int max_tokens) {
    int token_count = 0;
    
    while (*input && token_count < max_tokens) {
        skip_whitespace(&input);
        
        if (!*input) break;
        
        if (isdigit(*input)) {
            tokens[token_count] = get_number_token(&input);
        } else if (isalpha(*input) || *input == '_') {
            tokens[token_count] = get_identifier_token(&input);
        } else if (is_operator_char(*input)) {
            tokens[token_count] = get_operator_token(&input);
        } else if (*input == '"') {
            tokens[token_count] = get_string_token(&input);
        } else {
            tokens[token_count].type = TOKEN_ERROR;
            tokens[token_count].value[0] = *input;
            tokens[token_count].value[1] = '\0';
            input++;
        }
        
        if (tokens[token_count].type != TOKEN_ERROR) {
            token_count++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        token_count++;
    }
    
    return token_count;
}

const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input[1024];
    Token tokens[MAX_TOKENS];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *newline = strchr(input, '\n');
    if (newline) *newline = '\0';
    
    if (str