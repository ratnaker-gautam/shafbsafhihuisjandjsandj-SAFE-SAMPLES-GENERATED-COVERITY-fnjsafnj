//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
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
        if ((*start == '<' && **input == '=') ||
            (*start == '>' && **input == '=') ||
            (*start == '!' && **input == '=') ||
            (*start == '=' && **input == '=') ||
            (*start == '&' && **input == '&') ||
            (*start == '|' && **input == '|')) {
            op[1] = **input;
            (*input)++;
        }
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
    char *end;
    long num = strtol(*input, &end, 10);
    
    if (end == *input) {
        return 0;
    }
    
    size_t len = end - start;
    if (len >= sizeof(token->value)) {
        return 0;
    }
    
    memcpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_NUMBER;
    *input = end;
    return 1;
}

int parse_string(const char **input, Token *token) {
    if (**input != '"') {
        return 0;
    }
    
    (*input)++;
    const char *start = *input;
    size_t len = 0;
    
    while (**input && **input != '"' && len < sizeof(token->value) - 1) {
        if (**input == '\\' && *(*input + 1)) {
            (*input)++;
        }
        len++;
        (*input)++;
    }
    
    if (**input != '"') {
        return 0;
    }
    
    if (len >= sizeof(token->value)) {
        return 0;
    }
    
    memcpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_STRING;
    (*input)++;
    return 1;
}

int parse_identifier(const char **input, Token *token) {
    const char *start = *input;
    size_t len = 0;
    
    if (!isalpha(**input) && **input != '_') {
        return 0;
    }
    
    while (**input && (isalnum(**input) || **input == '_') && len < sizeof(token->value) - 1) {
        len++;
        (*input)++;
    }
    
    if (len >= sizeof(token->value)) {
        return 0;
    }
    
    memcpy(token->value, start, len);
    token->value[len] = '\0';
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
    
    if (parse_number(input, token)) return 1;
    if (parse_string(input, token)) return 1;
    if (parse_operator(input, token)) return 1;
    if (parse_identifier(input, token)) return 1;
    
    token->type = TOKEN_INVALID;
    token->value[0] = **input;
    token->value[1] = '\0';
    (*input)++;
    return 1;
}

void tokenize(const char *input, Token *tokens, int *token_count) {
    *token_count = 0;
    const char *ptr = input;
    
    while (*ptr && *token_count < MAX_TOKENS - 1) {
        Token token;
        if (!get_next_token(&ptr, &token)) {
            break;
        }
        
        if (token.type == TOKEN_EOF) {
            tokens[*token_count] = token;
            (*token_count)++;
            break;
        }
        
        tokens[*token_count] = token;
        (*token_count)++;
    }
    
    if (*token_count < MAX_TOKENS) {
        tokens[*token_count].type = TOKEN_EOF;
        tokens[*token_count].value[0] = '\0';
    }
}

const char* token_type_name(Token