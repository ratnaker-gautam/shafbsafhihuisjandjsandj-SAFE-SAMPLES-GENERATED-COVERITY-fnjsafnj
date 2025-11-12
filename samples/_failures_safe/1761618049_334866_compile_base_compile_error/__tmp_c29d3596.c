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
    printf("Number: %s\n", token->value);
}

void handle_identifier(Token* token) {
    printf("Identifier: %s\n", token->value);
}

void handle_operator(Token* token) {
    printf("Operator: %s\n", token->value);
}

void handle_string(Token* token) {
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

int parse_number(const char** input, char* buffer, size_t buf_size) {
    size_t i = 0;
    while (**input && isdigit(**input) && i < buf_size - 1) {
        buffer[i++] = *(*input)++;
    }
    buffer[i] = '\0';
    return i > 0;
}

int parse_identifier(const char** input, char* buffer, size_t buf_size) {
    size_t i = 0;
    if (**input && (isalpha(**input) || **input == '_')) {
        buffer[i++] = *(*input)++;
        while (**input && (isalnum(**input) || **input == '_') && i < buf_size - 1) {
            buffer[i++] = *(*input)++;
        }
        buffer[i] = '\0';
        return 1;
    }
    return 0;
}

int parse_operator(const char** input, char* buffer, size_t buf_size) {
    size_t i = 0;
    if (**input && is_operator_char(**input) && i < buf_size - 1) {
        buffer[i++] = *(*input)++;
        while (**input && is_operator_char(**input) && i < buf_size - 1) {
            buffer[i++] = *(*input)++;
        }
        buffer[i] = '\0';
        return 1;
    }
    return 0;
}

int parse_string(const char** input, char* buffer, size_t buf_size) {
    size_t i = 0;
    if (**input == '"') {
        (*input)++;
        while (**input && **input != '"' && i < buf_size - 1) {
            buffer[i++] = *(*input)++;
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
    size_t token_count = 0;
    const char* ptr = input;
    
    while (*ptr && token_count < max_tokens) {
        skip_whitespace(&ptr);
        if (!*ptr) break;
        
        Token* current = &tokens[token_count];
        
        if (parse_number(&ptr, current->value, sizeof(current->value))) {
            current->type = TOKEN_NUMBER;
            token_count++;
        } else if (parse_identifier(&ptr, current->value, sizeof(current->value))) {
            current->type = TOKEN_IDENTIFIER;
            token_count++;
        } else if (parse_operator(&ptr, current->value, sizeof(current->value))) {
            current->type = TOKEN_OPERATOR;
            token_count++;
        } else if (parse_string(&ptr, current->value, sizeof(current->value))) {
            current->type = TOKEN_STRING;
            token_count++;
        } else {
            break;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strncpy(tokens[token_count].value, "", sizeof(tokens[token_count].value));
        token_count++;
    }
    
    return token_count;
}

void process_tokens(Token* tokens, size_t count, TokenHandler* handlers) {
    for (size_t i = 0; i < count; i++) {
        if (tokens[i].type == TOKEN_EOF) break;
        if (handlers[tokens[i].type]) {
            handlers[tokens[i].type](&tokens[i]);
        }
    }
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || len == 1) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];
    size_t token_count = tokenize(input