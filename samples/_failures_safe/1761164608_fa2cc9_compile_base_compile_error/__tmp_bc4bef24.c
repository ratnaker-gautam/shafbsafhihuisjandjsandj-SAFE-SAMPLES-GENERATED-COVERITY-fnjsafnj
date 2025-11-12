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

typedef void (*TokenHandler)(const Token*);

void handle_number(const Token* token) {
    printf("Number: %s\n", token->value);
}

void handle_identifier(const Token* token) {
    printf("Identifier: %s\n", token->value);
}

void handle_operator(const Token* token) {
    printf("Operator: %s\n", token->value);
}

void handle_string(const Token* token) {
    printf("String: %s\n", token->value);
}

void skip_whitespace(const char** input) {
    while (**input && isspace(**input)) {
        (*input)++;
    }
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int parse_number(const char** input, char* buffer) {
    int len = 0;
    
    while (**input && isdigit(**input)) {
        if (len < MAX_TOKEN_LEN - 1) {
            buffer[len++] = **input;
        }
        (*input)++;
    }
    
    buffer[len] = '\0';
    return len > 0;
}

int parse_identifier(const char** input, char* buffer) {
    int len = 0;
    
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
    return len > 0;
}

int parse_operator(const char** input, char* buffer) {
    int len = 0;
    
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
    return len > 0;
}

int parse_string(const char** input, char* buffer) {
    int len = 0;
    
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
    return len > 0;
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    int token_count = 0;
    const char* current = input;
    
    while (*current && token_count < max_tokens) {
        skip_whitespace(&current);
        
        if (!*current) {
            break;
        }
        
        Token token;
        char buffer[MAX_TOKEN_LEN];
        
        if (parse_string(&current, buffer)) {
            token.type = TOKEN_STRING;
            strncpy(token.value, buffer, MAX_TOKEN_LEN - 1);
            token.value[MAX_TOKEN_LEN - 1] = '\0';
        } else if (parse_number(&current, buffer)) {
            token.type = TOKEN_NUMBER;
            strncpy(token.value, buffer, MAX_TOKEN_LEN - 1);
            token.value[MAX_TOKEN_LEN - 1] = '\0';
        } else if (parse_operator(&current, buffer)) {
            token.type = TOKEN_OPERATOR;
            strncpy(token.value, buffer, MAX_TOKEN_LEN - 1);
            token.value[MAX_TOKEN_LEN - 1] = '\0';
        } else if (parse_identifier(&current, buffer)) {
            token.type = TOKEN_IDENTIFIER;
            strncpy(token.value, buffer, MAX_TOKEN_LEN - 1);
            token.value[MAX_TOKEN_LEN - 1] = '\0';
        } else {
            break;
        }
        
        tokens[token_count++] = token;
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        token_count++;
    }
    
    return token_count;
}

void process_tokens(const Token* tokens, int count, TokenHandler* handlers) {
    for (int i = 0; i < count && tokens[i].type != TOKEN_EOF; i++) {
        if (tokens[i].type >= 0 && tokens[i].type <= TOKEN_STRING) {
            handlers[tokens[i].type](&tokens[i]);
        }
    }
}

int main() {
    char input[