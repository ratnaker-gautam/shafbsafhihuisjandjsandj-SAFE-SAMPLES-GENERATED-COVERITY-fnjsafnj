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
    TOKEN_UNKNOWN
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
    char *ptr = token->value;
    
    if (**input == '\0') return 0;
    
    *ptr++ = **input;
    (*input)++;
    
    if ((*start == '<' && **input == '=') ||
        (*start == '>' && **input == '=') ||
        (*start == '!' && **input == '=') ||
        (*start == '=' && **input == '=') ||
        (*start == '&' && **input == '&') ||
        (*start == '|' && **input == '|')) {
        if (ptr - token->value < MAX_TOKEN_LEN - 1) {
            *ptr++ = **input;
            (*input)++;
        }
    }
    
    *ptr = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

int parse_number(const char **input, Token *token) {
    const char *start = *input;
    char *ptr = token->value;
    int has_dot = 0;
    
    if (**input == '\0' || !isdigit(**input)) return 0;
    
    while (**input != '\0' && (isdigit(**input) || **input == '.')) {
        if (**input == '.') {
            if (has_dot) break;
            has_dot = 1;
        }
        if (ptr - token->value < MAX_TOKEN_LEN - 1) {
            *ptr++ = **input;
        }
        (*input)++;
    }
    
    *ptr = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_string(const char **input, Token *token) {
    char *ptr = token->value;
    
    if (**input != '"') return 0;
    
    (*input)++;
    
    while (**input != '\0' && **input != '"') {
        if (ptr - token->value < MAX_TOKEN_LEN - 1) {
            *ptr++ = **input;
        }
        (*input)++;
    }
    
    if (**input == '"') {
        (*input)++;
    }
    
    *ptr = '\0';
    token->type = TOKEN_STRING;
    return 1;
}

int parse_identifier(const char **input, Token *token) {
    const char *start = *input;
    char *ptr = token->value;
    
    if (**input == '\0' || !isalpha(**input)) return 0;
    
    while (**input != '\0' && (isalnum(**input) || **input == '_')) {
        if (ptr - token->value < MAX_TOKEN_LEN - 1) {
            *ptr++ = **input;
        }
        (*input)++;
    }
    
    *ptr = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

void skip_whitespace(const char **input) {
    while (**input != '\0' && isspace(**input)) {
        (*input)++;
    }
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    int count = 0;
    const char *ptr = input;
    
    while (*ptr != '\0' && count < max_tokens) {
        skip_whitespace(&ptr);
        
        if (*ptr == '\0') break;
        
        Token *current = tokens + count;
        
        if (parse_string(&ptr, current) ||
            parse_number(&ptr, current) ||
            parse_operator(&ptr, current) ||
            parse_identifier(&ptr, current)) {
            count++;
            continue;
        }
        
        current->type = TOKEN_UNKNOWN;
        current->value[0] = *ptr;
        current->value[1] = '\0';
        ptr++;
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
    const char *names[] = {
        "IDENTIFIER", "NUMBER", "OPERATOR", "STRING", "EOF", "UNKNOWN"
    };
    if (type >= TOKEN_IDENTIFIER && type <= TOKEN_UNKNOWN) {
        printf("%s", names[type]);
    }
}

int main(void) {
    char input[1024];
    Token tokens[MAX_TOKENS];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;