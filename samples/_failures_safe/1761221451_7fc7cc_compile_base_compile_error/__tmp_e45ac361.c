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
    if (value[0] == '\0') return TOKEN_INVALID;
    if (isdigit(value[0])) return TOKEN_NUMBER;
    if (isalpha(value[0]) || value[0] == '_') return TOKEN_IDENTIFIER;
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
    
    size_t len = ptr - start;
    if (len >= MAX_TOKEN_LEN) return 0;
    
    strncpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_NUMBER;
    *input = ptr;
    return 1;
}

int parse_identifier(char **input, Token *token) {
    char *start = *input;
    char *ptr = *input;
    
    if (!isalpha(*ptr) && *ptr != '_') return 0;
    ptr++;
    
    while (*ptr && (isalnum(*ptr) || *ptr == '_')) {
        ptr++;
    }
    
    size_t len = ptr - start;
    if (len >= MAX_TOKEN_LEN) return 0;
    
    strncpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_IDENTIFIER;
    *input = ptr;
    return 1;
}

int parse_operator(char **input, Token *token) {
    char *start = *input;
    char *ptr = *input;
    
    if (!is_operator_char(*ptr)) return 0;
    ptr++;
    
    size_t len = ptr - start;
    if (len >= MAX_TOKEN_LEN) return 0;
    
    strncpy(token->value, start, len);
    token->value[len] = '\0';
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
    
    size_t len = ptr - start;
    if (len >= MAX_TOKEN_LEN) return 0;
    
    strncpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_STRING;
    *input = ptr;
    return 1;
}

int tokenize(char *input, Token *tokens, int max_tokens) {
    int count = 0;
    
    while (*input && count < max_tokens) {
        skip_whitespace(&input);
        if (!*input) break;
        
        Token token = {TOKEN_INVALID, ""};
        int parsed = 0;
        
        if (parse_number(&input, &token)) parsed = 1;
        else if (parse_identifier(&input, &token)) parsed = 1;
        else if (parse_operator(&input, &token)) parsed = 1;
        else if (parse_string(&input, &token)) parsed = 1;
        
        if (!parsed) {
            token.value[0] = *input;
            token.value[1] = '\0';
            token.type = TOKEN_INVALID;
            input++;
        }
        
        tokens[count] = token;
        count++;
    }
    
    if (count < max_tokens) {
        tokens[count].type = TOKEN_EOF;
        tokens[count].value[0] = '\0';
        count++;
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
        case TOKEN_INVALID: printf("INVALID"); break;
    }
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error