//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: parser_tokenizer
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
    if (token) {
        printf("Found number: %s\n", token->value);
    }
}

void handle_identifier(Token* token) {
    if (token) {
        printf("Found identifier: %s\n", token->value);
    }
}

void handle_operator(Token* token) {
    if (token) {
        printf("Found operator: %s\n", token->value);
    }
}

void handle_string(Token* token) {
    if (token) {
        printf("Found string: %s\n", token->value);
    }
}

void skip_whitespace(const char** input) {
    if (input && *input) {
        while (**input && isspace(**input)) {
            (*input)++;
        }
    }
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int parse_number(const char** input, char* buffer) {
    int len = 0;
    const char* start = NULL;
    
    if (!input || !*input || !buffer) {
        return 0;
    }
    
    start = *input;
    
    while (**input && (isdigit(**input) || **input == '.')) {
        if (len < MAX_TOKEN_LEN - 1) {
            buffer[len++] = **input;
        }
        (*input)++;
    }
    
    buffer[len] = '\0';
    return (*input > start) ? 1 : 0;
}

int parse_identifier(const char** input, char* buffer) {
    int len = 0;
    const char* start = NULL;
    
    if (!input || !*input || !buffer) {
        return 0;
    }
    
    start = *input;
    
    if (**input && (isalpha(**input) || **input == '_')) {
        if (len < MAX_TOKEN_LEN - 1) {
            buffer[len++] = **input;
        }
        (*input)++;
        
        while (**input && (isalnum(**input) || **input == '_')) {
            if (len < MAX_TOKEN_LEN - 1) {
                buffer[len++] = **input;
            }
            (*input)++;
        }
    }
    
    buffer[len] = '\0';
    return (*input > start) ? 1 : 0;
}

int parse_operator(const char** input, char* buffer) {
    int len = 0;
    const char* start = NULL;
    
    if (!input || !*input || !buffer) {
        return 0;
    }
    
    start = *input;
    
    if (**input && is_operator_char(**input)) {
        if (len < MAX_TOKEN_LEN - 1) {
            buffer[len++] = **input;
        }
        (*input)++;
        
        if (**input && is_operator_char(**input)) {
            if (len < MAX_TOKEN_LEN - 1) {
                buffer[len++] = **input;
            }
            (*input)++;
        }
    }
    
    buffer[len] = '\0';
    return (*input > start) ? 1 : 0;
}

int parse_string(const char** input, char* buffer) {
    int len = 0;
    
    if (!input || !*input || !buffer) {
        return 0;
    }
    
    if (**input == '"') {
        (*input)++;
        
        while (**input && **input != '"') {
            if (len < MAX_TOKEN_LEN - 1) {
                buffer[len++] = **input;
            }
            (*input)++;
        }
        
        if (**input == '"') {
            (*input)++;
        }
    }
    
    buffer[len] = '\0';
    return (len > 0) ? 1 : 0;
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    int token_count = 0;
    const char* current = NULL;
    
    if (!input || !tokens || max_tokens <= 0) {
        return 0;
    }
    
    current = input;
    
    while (*current && token_count < max_tokens) {
        skip_whitespace(&current);
        
        if (!*current) {
            break;
        }
        
        Token token = {TOKEN_EOF, ""};
        char buffer[MAX_TOKEN_LEN] = {0};
        
        if (parse_number(&current, buffer)) {
            token.type = TOKEN_NUMBER;
            strncpy(token.value, buffer, MAX_TOKEN_LEN - 1);
            token.value[MAX_TOKEN_LEN - 1] = '\0';
        } else if (parse_string(&current, buffer)) {
            token.type = TOKEN_STRING;
            strncpy(token.value, buffer, MAX_TOKEN_LEN - 1);
            token.value[MAX_TOKEN_LEN - 1] = '\0';
        } else if (parse_operator(&current, buffer)) {
            token.type = TOKEN_OPERATOR;