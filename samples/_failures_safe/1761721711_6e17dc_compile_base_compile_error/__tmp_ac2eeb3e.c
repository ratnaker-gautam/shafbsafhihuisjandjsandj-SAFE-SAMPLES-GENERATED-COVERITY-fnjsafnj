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
        while (**input && isspace(**input)) {
            (*input)++;
        }
    }
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int parse_number(const char** input, char* buffer, size_t buf_size) {
    if (!input || !*input || !buffer || buf_size == 0) return 0;
    
    size_t i = 0;
    while (**input && isdigit(**input) && i < buf_size - 1) {
        buffer[i++] = **input;
        (*input)++;
    }
    buffer[i] = '\0';
    return i > 0;
}

int parse_identifier(const char** input, char* buffer, size_t buf_size) {
    if (!input || !*input || !buffer || buf_size == 0) return 0;
    
    size_t i = 0;
    if (**input && (isalpha(**input) || **input == '_')) {
        buffer[i++] = **input;
        (*input)++;
        while (**input && (isalnum(**input) || **input == '_') && i < buf_size - 1) {
            buffer[i++] = **input;
            (*input)++;
        }
        buffer[i] = '\0';
        return 1;
    }
    return 0;
}

int parse_operator(const char** input, char* buffer, size_t buf_size) {
    if (!input || !*input || !buffer || buf_size == 0) return 0;
    
    size_t i = 0;
    if (**input && is_operator_char(**input) && i < buf_size - 1) {
        buffer[i++] = **input;
        (*input)++;
        if (**input && is_operator_char(**input) && i < buf_size - 1) {
            buffer[i++] = **input;
            (*input)++;
        }
        buffer[i] = '\0';
        return 1;
    }
    return 0;
}

int parse_string(const char** input, char* buffer, size_t buf_size) {
    if (!input || !*input || !buffer || buf_size == 0) return 0;
    
    size_t i = 0;
    if (**input == '"') {
        (*input)++;
        while (**input && **input != '"' && i < buf_size - 1) {
            buffer[i++] = **input;
            (*input)++;
        }
        if (**input == '"') {
            (*input)++;
            buffer[i] = '\0';
            return 1;
        }
    }
    return 0;
}

int tokenize(const char* input, Token* tokens, size_t max_tokens) {
    if (!input || !tokens || max_tokens == 0) return 0;
    
    size_t token_count = 0;
    
    while (*input && token_count < max_tokens) {
        skip_whitespace(&input);
        if (!*input) break;
        
        Token token;
        memset(&token, 0, sizeof(token));
        
        if (parse_number(&input, token.value, sizeof(token.value))) {
            token.type = TOKEN_NUMBER;
        } else if (parse_string(&input, token.value, sizeof(token.value))) {
            token.type = TOKEN_STRING;
        } else if (parse_identifier(&input, token.value, sizeof(token.value))) {
            token.type = TOKEN_IDENTIFIER;
        } else if (parse_operator(&input, token.value, sizeof(token.value))) {
            token.type = TOKEN_OPERATOR;
        } else {
            break;
        }
        
        tokens[token_count++] = token;
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strncpy(tokens[token_count].value, "", sizeof(tokens[token_count].value) - 1);
        tokens[token_count].value[sizeof(tokens[token_count].value) - 1] = '\0';
        token_count++;
    }
    
    return token_count;
}

void process_tokens(Token* tokens, size_t count, TokenHandler* handlers) {
    if (!tokens || !handlers || count == 0) return;
    
    for (size_t i = 0;