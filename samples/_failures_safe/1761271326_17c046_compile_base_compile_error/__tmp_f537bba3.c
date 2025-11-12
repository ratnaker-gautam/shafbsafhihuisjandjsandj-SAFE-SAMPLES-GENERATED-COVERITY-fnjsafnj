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
    TOKEN_EOF,
    TOKEN_INVALID
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int parse_operator(const char **input, Token *token) {
    const char *start = *input;
    char op[3] = {0};
    
    if (!**input) return 0;
    
    op[0] = **input;
    (*input)++;
    
    if (**input && is_operator_char(**input)) {
        op[1] = **input;
        (*input)++;
    }
    
    if (strlen(op) > 0) {
        strncpy(token->value, op, MAX_TOKEN_LEN - 1);
        token->value[MAX_TOKEN_LEN - 1] = '\0';
        token->type = TOKEN_OPERATOR;
        return 1;
    }
    
    *input = start;
    return 0;
}

int parse_number(const char **input, Token *token) {
    const char *start = *input;
    char *end;
    
    if (!**input || !isdigit(**input)) {
        return 0;
    }
    
    long num = strtol(*input, &end, 10);
    if (end > *input) {
        size_t len = end - *input;
        if (len < MAX_TOKEN_LEN) {
            strncpy(token->value, *input, len);
            token->value[len] = '\0';
            token->type = TOKEN_NUMBER;
            *input = end;
            return 1;
        }
    }
    
    *input = start;
    return 0;
}

int parse_string(const char **input, Token *token) {
    const char *start = *input;
    
    if (!**input || **input != '"') {
        return 0;
    }
    
    (*input)++;
    const char *str_start = *input;
    
    while (**input && **input != '"') {
        (*input)++;
    }
    
    if (**input == '"') {
        size_t len = *input - str_start;
        if (len < MAX_TOKEN_LEN) {
            strncpy(token->value, str_start, len);
            token->value[len] = '\0';
            token->type = TOKEN_STRING;
            (*input)++;
            return 1;
        }
    }
    
    *input = start;
    return 0;
}

int parse_identifier(const char **input, Token *token) {
    const char *start = *input;
    
    if (!**input || (!isalpha(**input) && **input != '_')) {
        return 0;
    }
    
    const char *id_start = *input;
    while (**input && (isalnum(**input) || **input == '_')) {
        (*input)++;
    }
    
    size_t len = *input - id_start;
    if (len > 0 && len < MAX_TOKEN_LEN) {
        strncpy(token->value, id_start, len);
        token->value[len] = '\0';
        token->type = TOKEN_IDENTIFIER;
        return 1;
    }
    
    *input = start;
    return 0;
}

int get_next_token(const char **input, Token *token) {
    if (!input || !token) return 0;
    
    while (**input && isspace(**input)) {
        (*input)++;
    }
    
    if (!**input) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }
    
    if (parse_string(input, token)) return 1;
    if (parse_number(input, token)) return 1;
    if (parse_operator(input, token)) return 1;
    if (parse_identifier(input, token)) return 1;
    
    token->type = TOKEN_INVALID;
    token->value[0] = **input;
    token->value[1] = '\0';
    (*input)++;
    return 1;
}

const char* token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_INVALID: return "INVALID";
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
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len -