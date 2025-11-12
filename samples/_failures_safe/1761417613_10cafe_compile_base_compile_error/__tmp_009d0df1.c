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
    if (isdigit(value[0])) return TOKEN_NUMBER;
    if (isalpha(value[0])) return TOKEN_IDENTIFIER;
    if (is_operator_char(value[0])) return TOKEN_OPERATOR;
    if (value[0] == '"') return TOKEN_STRING;
    return TOKEN_INVALID;
}

int parse_number(char **input, Token *token) {
    char *start = *input;
    char *ptr = *input;
    
    while (*ptr && isdigit(*ptr)) {
        ptr++;
    }
    
    if (ptr - start >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    strncpy(token->value, start, ptr - start);
    token->value[ptr - start] = '\0';
    token->type = TOKEN_NUMBER;
    *input = ptr;
    return 1;
}

int parse_identifier(char **input, Token *token) {
    char *start = *input;
    char *ptr = *input;
    
    while (*ptr && (isalnum(*ptr) || *ptr == '_')) {
        ptr++;
    }
    
    if (ptr - start >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    strncpy(token->value, start, ptr - start);
    token->value[ptr - start] = '\0';
    token->type = TOKEN_IDENTIFIER;
    *input = ptr;
    return 1;
}

int parse_operator(char **input, Token *token) {
    char *start = *input;
    char *ptr = *input;
    
    while (*ptr && is_operator_char(*ptr)) {
        ptr++;
    }
    
    if (ptr - start >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    strncpy(token->value, start, ptr - start);
    token->value[ptr - start] = '\0';
    token->type = TOKEN_OPERATOR;
    *input = ptr;
    return 1;
}

int parse_string(char **input, Token *token) {
    char *start = *input;
    char *ptr = *input;
    
    if (*ptr != '"') return 0;
    ptr++;
    
    while (*ptr && *ptr != '"') {
        ptr++;
    }
    
    if (*ptr != '"') return 0;
    ptr++;
    
    if (ptr - start >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    strncpy(token->value, start, ptr - start);
    token->value[ptr - start] = '\0';
    token->type = TOKEN_STRING;
    *input = ptr;
    return 1;
}

int get_next_token(char **input, Token *token) {
    skip_whitespace(input);
    
    if (!**input) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }
    
    char current = **input;
    
    if (isdigit(current)) {
        return parse_number(input, token);
    } else if (isalpha(current)) {
        return parse_identifier(input, token);
    } else if (is_operator_char(current)) {
        return parse_operator(input, token);
    } else if (current == '"') {
        return parse_string(input, token);
    } else {
        token->type = TOKEN_INVALID;
        token->value[0] = current;
        token->value[1] = '\0';
        (*input)++;
        return 1;
    }
}

int tokenize(char *input, Token *tokens, int max_tokens) {
    int count = 0;
    char *ptr = input;
    
    while (*ptr && count < max_tokens) {
        if (!get_next_token(&ptr, &tokens[count])) {
            return -1;
        }
        if (tokens[count].type == TOKEN_EOF) {
            break;
        }
        count++;
    }
    
    if (count < max_tokens) {
        tokens[count].type = TOKEN_EOF;
        tokens[count].value[0] = '\0';
    }
    
    return count;
}

const char *token_type_name(TokenType type) {
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