//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

typedef void (*TokenHandler)(Token*);

void handle_number(Token* token) {
    if (token == NULL) return;
    printf("Number: %s\n", token->value);
}

void handle_identifier(Token* token) {
    if (token == NULL) return;
    printf("Identifier: %s\n", token->value);
}

void handle_operator(Token* token) {
    if (token == NULL) return;
    printf("Operator: %s\n", token->value);
}

void handle_string(Token* token) {
    if (token == NULL) return;
    printf("String: %s\n", token->value);
}

void skip_whitespace(const char** input) {
    if (input == NULL || *input == NULL) return;
    while (**input && isspace(**input)) {
        (*input)++;
    }
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int parse_number(const char** input, Token* token) {
    if (input == NULL || *input == NULL || token == NULL) return 0;
    
    const char* start = *input;
    int len = 0;
    
    while (**input && (isdigit(**input) || **input == '.')) {
        (*input)++;
        len++;
    }
    
    if (len > 0 && len < MAX_TOKEN_LEN) {
        strncpy(token->value, start, len);
        token->value[len] = '\0';
        token->type = TOKEN_NUMBER;
        return 1;
    }
    
    return 0;
}

int parse_identifier(const char** input, Token* token) {
    if (input == NULL || *input == NULL || token == NULL) return 0;
    
    const char* start = *input;
    int len = 0;
    
    if (isalpha(**input) || **input == '_') {
        (*input)++;
        len++;
        
        while (**input && (isalnum(**input) || **input == '_')) {
            (*input)++;
            len++;
        }
    }
    
    if (len > 0 && len < MAX_TOKEN_LEN) {
        strncpy(token->value, start, len);
        token->value[len] = '\0';
        token->type = TOKEN_IDENTIFIER;
        return 1;
    }
    
    return 0;
}

int parse_operator(const char** input, Token* token) {
    if (input == NULL || *input == NULL || token == NULL) return 0;
    
    if (is_operator_char(**input)) {
        token->value[0] = **input;
        token->value[1] = '\0';
        token->type = TOKEN_OPERATOR;
        (*input)++;
        return 1;
    }
    
    return 0;
}

int parse_string(const char** input, Token* token) {
    if (input == NULL || *input == NULL || token == NULL) return 0;
    
    if (**input == '"') {
        (*input)++;
        const char* start = *input;
        int len = 0;
        
        while (**input && **input != '"') {
            (*input)++;
            len++;
        }
        
        if (**input == '"') {
            if (len > 0 && len < MAX_TOKEN_LEN) {
                strncpy(token->value, start, len);
                token->value[len] = '\0';
                token->type = TOKEN_STRING;
                (*input)++;
                return 1;
            }
        }
    }
    
    return 0;
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    if (input == NULL || tokens == NULL || max_tokens <= 0) return 0;
    
    int token_count = 0;
    const char* current = input;
    
    while (*current && token_count < max_tokens) {
        skip_whitespace(&current);
        if (!*current) break;
        
        Token token;
        int parsed = 0;
        
        if (parse_string(&current, &token)) parsed = 1;
        else if (parse_number(&current, &token)) parsed = 1;
        else if (parse_identifier(&current, &token)) parsed = 1;
        else if (parse_operator(&current, &token)) parsed = 1;
        
        if (parsed) {
            tokens[token_count] = token;
            token_count++;
        } else {
            current++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "");
        token_count++;
    }
    
    return token_count;
}

void process_tokens(Token* tokens, int count, TokenHandler* handlers) {
    if (tokens == NULL || handlers == NULL || count <= 0) return;
    
    for (int i = 0; i < count