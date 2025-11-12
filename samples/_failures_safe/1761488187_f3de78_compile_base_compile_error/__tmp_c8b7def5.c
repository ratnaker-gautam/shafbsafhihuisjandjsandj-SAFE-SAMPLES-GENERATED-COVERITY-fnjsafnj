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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int parse_number(const char **input, Token *token) {
    const char *p = *input;
    char *dest = token->value;
    int has_dot = 0;
    
    while (*p && (isdigit(*p) || (*p == '.' && !has_dot))) {
        if (*p == '.') has_dot = 1;
        if ((size_t)(dest - token->value) >= MAX_TOKEN_LEN - 1) return 0;
        *dest++ = *p++;
    }
    *dest = '\0';
    *input = p;
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_string(const char **input, Token *token) {
    const char *p = *input;
    char *dest = token->value;
    
    if (*p != '"') return 0;
    p++;
    
    while (*p && *p != '"') {
        if ((size_t)(dest - token->value) >= MAX_TOKEN_LEN - 1) return 0;
        *dest++ = *p++;
    }
    
    if (*p != '"') return 0;
    p++;
    *dest = '\0';
    *input = p;
    token->type = TOKEN_STRING;
    return 1;
}

int parse_identifier(const char **input, Token *token) {
    const char *p = *input;
    char *dest = token->value;
    
    if (!isalpha(*p) && *p != '_') return 0;
    
    while (*p && (isalnum(*p) || *p == '_')) {
        if ((size_t)(dest - token->value) >= MAX_TOKEN_LEN - 1) return 0;
        *dest++ = *p++;
    }
    *dest = '\0';
    *input = p;
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_operator(const char **input, Token *token) {
    const char *p = *input;
    char *dest = token->value;
    
    if (!is_operator_char(*p)) return 0;
    
    *dest++ = *p++;
    if (is_operator_char(*p) && 
        ((*(p-1) == '&' && *p == '&') || 
         (*(p-1) == '|' && *p == '|') ||
         (*(p-1) == '=' && *p == '=') ||
         (*(p-1) == '!' && *p == '=') ||
         (*(p-1) == '<' && *p == '=') ||
         (*(p-1) == '>' && *p == '='))) {
        if ((size_t)(dest - token->value) >= MAX_TOKEN_LEN - 1) return 0;
        *dest++ = *p++;
    }
    *dest = '\0';
    *input = p;
    token->type = TOKEN_OPERATOR;
    return 1;
}

int tokenize(const char *input, Token *tokens, size_t max_tokens) {
    const char *p = input;
    size_t token_count = 0;
    
    while (*p && token_count < max_tokens) {
        while (*p && isspace(*p)) p++;
        if (!*p) break;
        
        Token *current = tokens + token_count;
        
        if (parse_number(&p, current) ||
            parse_string(&p, current) ||
            parse_identifier(&p, current) ||
            parse_operator(&p, current)) {
            token_count++;
        } else {
            current->type = TOKEN_INVALID;
            char *dest = current->value;
            if ((size_t)(dest - current->value) >= MAX_TOKEN_LEN - 1) return -1;
            *dest++ = *p;
            *dest = '\0';
            p++;
            token_count++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        token_count++;
    }
    
    return (int)token_count;
}

void print_token_type(TokenType type) {
    const char *names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "INVALID"
    };
    if (type >= TOKEN_NUMBER && type <= TOKEN_INVALID) {
        printf("%s", names[type]);
    }
}

int main(void) {
    char input[1024];
    
    printf("Enter expression to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");