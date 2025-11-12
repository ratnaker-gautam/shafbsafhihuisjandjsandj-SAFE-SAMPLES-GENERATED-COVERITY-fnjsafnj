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
    
    if (**input == '"') {
        (*input)++;
        start = *input;
        
        while (**input && **input != '"') {
            (*input)++;
        }
        
        if (**input == '"') {
            size_t len = *input - start;
            if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
            strncpy(token.value, start, len);
            token.value[len] = '\0';
            (*input)++;
        } else {
            token.type = TOKEN_ERROR;
            strcpy(token.value, "Unterminated string");
        }
    }
    
    return token;
}

int tokenize(char *input, Token *tokens, int max_tokens) {
    int token_count = 0;
    
    while (*input && token_count < max_tokens) {
        skip_whitespace(&input);
        if (!*input) break;
        
        Token token;
        
        if (isdigit(*input)) {
            token = get_number_token(&input);
        } else if (isalpha(*input) || *input == '_') {
            token = get_identifier_token(&input);
        } else if (is_operator_char(*input)) {
            token = get_operator_token(&input);
        } else if (*input == '"') {
            token = get_string_token(&input);
        } else {
            token.type = TOKEN_ERROR;
            char error_msg[32];
            snprintf(error_msg, sizeof(error_msg), "Unexpected character: %c", *input);
            strncpy(token.value, error_msg, MAX_TOKEN_LEN - 1);
            token.value[MAX_TOKEN_LEN - 1] = '\0';
            input++;
        }
        
        tokens[token_count++] = token;
        
        if (token.type == TOKEN_ERROR) {
            break;
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
    char input_buffer[1024];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 &&