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
        printf("Number: %s\n", token->value);
    }
}

void handle_identifier(Token* token) {
    if (token) {
        printf("Identifier: %s\n", token->value);
    }
}

void handle_operator(Token* token) {
    if (token) {
        printf("Operator: %s\n", token->value);
    }
}

void handle_string(Token* token) {
    if (token) {
        printf("String: %s\n", token->value);
    }
}

void skip_whitespace(const char** input) {
    if (input && *input) {
        while (**input && isspace((unsigned char)**input)) {
            (*input)++;
        }
    }
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int parse_number(const char** input, char* buffer) {
    if (!input || !*input || !buffer) return 0;
    
    int len = 0;
    const char* start = *input;
    
    while (**input && isdigit((unsigned char)**input)) {
        if (len < MAX_TOKEN_LEN - 1) {
            buffer[len++] = **input;
        }
        (*input)++;
    }
    
    if (len > 0) {
        buffer[len] = '\0';
        return 1;
    }
    
    *input = start;
    return 0;
}

int parse_identifier(const char** input, char* buffer) {
    if (!input || !*input || !buffer) return 0;
    
    int len = 0;
    const char* start = *input;
    
    if (**input && (isalpha((unsigned char)**input) || **input == '_')) {
        if (len < MAX_TOKEN_LEN - 1) {
            buffer[len++] = **input;
        }
        (*input)++;
        
        while (**input && (isalnum((unsigned char)**input) || **input == '_')) {
            if (len < MAX_TOKEN_LEN - 1) {
                buffer[len++] = **input;
            }
            (*input)++;
        }
        
        buffer[len] = '\0';
        return 1;
    }
    
    *input = start;
    return 0;
}

int parse_operator(const char** input, char* buffer) {
    if (!input || !*input || !buffer) return 0;
    
    int len = 0;
    const char* start = *input;
    
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
        
        buffer[len] = '\0';
        return 1;
    }
    
    *input = start;
    return 0;
}

int parse_string(const char** input, char* buffer) {
    if (!input || !*input || !buffer) return 0;
    
    int len = 0;
    const char* start = *input;
    
    if (**input == '"') {
        (*input)++;
        
        while (**input && **input != '"') {
            if (len < MAX_TOKEN_LEN - 1) {
                buffer[len++] = **input;
            }
            (*input)++;
            
            if (**input == '\0') {
                *input = start;
                return 0;
            }
        }
        
        if (**input == '"') {
            (*input)++;
            buffer[len] = '\0';
            return 1;
        }
    }
    
    *input = start;
    return 0;
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    if (!input || !tokens || max_tokens <= 0) return 0;
    
    int token_count = 0;
    const char* current = input;
    
    while (*current && token_count < max_tokens) {
        skip_whitespace(&current);
        
        if (!*current) {
            break;
        }
        
        Token token;
        memset(&token, 0, sizeof(token));
        
        if (parse_number(&current, token.value)) {
            token.type = TOKEN_NUMBER;
        } else if (parse_identifier(&current, token.value)) {
            token.type = TOKEN_IDENTIFIER;
        } else if (parse_operator(&current, token.value)) {
            token.type = TOKEN_OPERATOR;
        } else if (parse_string(&current, token.value)) {
            token.type = TOKEN_STRING;
        } else {
            printf("Error: Invalid character '%c'\n", *current);
            current++;
            continue;
        }
        
        tokens[token_count++] =