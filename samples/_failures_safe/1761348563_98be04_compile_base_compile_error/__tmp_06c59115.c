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
    
    op[0] = **input;
    (*input)++;
    
    if (**input && is_operator_char(**input)) {
        op[1] = **input;
        (*input)++;
    }
    
    if (strlen(op) > sizeof(token->value) - 1) {
        return 0;
    }
    
    strcpy(token->value, op);
    token->type = TOKEN_OPERATOR;
    return 1;
}

int parse_number(const char **input, Token *token) {
    const char *start = *input;
    char *end = token->value;
    char *max_end = token->value + sizeof(token->value) - 1;
    
    while (**input && isdigit(**input)) {
        if (end >= max_end) {
            return 0;
        }
        *end++ = **input;
        (*input)++;
    }
    *end = '\0';
    
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_string(const char **input, Token *token) {
    if (**input != '"') return 0;
    
    (*input)++;
    char *end = token->value;
    char *max_end = token->value + sizeof(token->value) - 1;
    
    while (**input && **input != '"') {
        if (end >= max_end) {
            return 0;
        }
        *end++ = **input;
        (*input)++;
    }
    
    if (**input != '"') {
        return 0;
    }
    (*input)++;
    *end = '\0';
    
    token->type = TOKEN_STRING;
    return 1;
}

int parse_identifier(const char **input, Token *token) {
    const char *start = *input;
    char *end = token->value;
    char *max_end = token->value + sizeof(token->value) - 1;
    
    if (!isalpha(**input) && **input != '_') {
        return 0;
    }
    
    *end++ = **input;
    (*input)++;
    
    while (**input && (isalnum(**input) || **input == '_')) {
        if (end >= max_end) {
            return 0;
        }
        *end++ = **input;
        (*input)++;
    }
    *end = '\0';
    
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int get_next_token(const char **input, Token *token) {
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

int tokenize(const char *input, Token *tokens, int max_tokens) {
    int count = 0;
    const char *current = input;
    
    while (*current && count < max_tokens) {
        if (!get_next_token(&current, &tokens[count])) {
            return -1;
        }
        count++;
        if (tokens[count-1].type == TOKEN_EOF) {
            break;
        }
    }
    
    return count;
}

int main(void) {
    char input[1024];
    Token tokens[MAX_TOKENS];
    
    printf("Enter expression to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    int token