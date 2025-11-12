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

TokenType classify_token(const char *value) {
    if (isdigit(value[0])) {
        return TOKEN_NUMBER;
    } else if (isalpha(value[0]) || value[0] == '_') {
        return TOKEN_IDENTIFIER;
    } else if (is_operator_char(value[0])) {
        return TOKEN_OPERATOR;
    } else if (value[0] == '"') {
        return TOKEN_STRING;
    }
    return TOKEN_INVALID;
}

int parse_number(char **input, Token *token) {
    char *start = *input;
    char *ptr = *input;
    
    while (*ptr && isdigit(*ptr)) {
        ptr++;
    }
    
    size_t len = ptr - start;
    if (len >= MAX_TOKEN_LEN || len == 0) {
        return 0;
    }
    
    memcpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_NUMBER;
    *input = ptr;
    return 1;
}

int parse_identifier(char **input, Token *token) {
    char *start = *input;
    char *ptr = *input;
    
    if (!isalpha(*ptr) && *ptr != '_') {
        return 0;
    }
    
    ptr++;
    while (*ptr && (isalnum(*ptr) || *ptr == '_')) {
        ptr++;
    }
    
    size_t len = ptr - start;
    if (len >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    memcpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_IDENTIFIER;
    *input = ptr;
    return 1;
}

int parse_operator(char **input, Token *token) {
    if (!is_operator_char(**input)) {
        return 0;
    }
    
    token->value[0] = **input;
    token->value[1] = '\0';
    token->type = TOKEN_OPERATOR;
    (*input)++;
    return 1;
}

int parse_string(char **input, Token *token) {
    char *start = *input;
    char *ptr = *input;
    
    if (*ptr != '"') {
        return 0;
    }
    
    ptr++;
    while (*ptr && *ptr != '"') {
        ptr++;
    }
    
    if (*ptr != '"') {
        return 0;
    }
    
    size_t len = ptr - start + 1;
    if (len >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    memcpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_STRING;
    *input = ptr + 1;
    return 1;
}

int get_next_token(char **input, Token *token) {
    skip_whitespace(input);
    
    if (!**input) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }
    
    if (parse_number(input, token)) return 1;
    if (parse_identifier(input, token)) return 1;
    if (parse_operator(input, token)) return 1;
    if (parse_string(input, token)) return 1;
    
    token->type = TOKEN_INVALID;
    token->value[0] = **input;
    token->value[1] = '\0';
    (*input)++;
    return 1;
}

const char* token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_INVALID: return "INVALID";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input_buffer[1024];
    
    printf("Enter expression to tokenize: ");
    if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *input_ptr = input_buffer;
    Token tokens[MAX_TOKENS];
    int token_count = 0;
    
    while (token_count < MAX_TOKENS) {
        if (!get_next_token(&input_ptr, &tokens[token_count])) {
            fprintf(stderr, "Error parsing token\n");
            return 1;
        }
        
        if (tokens[token_count].type == TOKEN_EOF) {