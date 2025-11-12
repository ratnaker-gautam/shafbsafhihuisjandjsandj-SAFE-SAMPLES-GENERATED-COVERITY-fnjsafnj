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

void handle_eof(Token* token) {
    printf("End of input\n");
}

void skip_whitespace(const char** input) {
    while (**input && isspace(**input)) {
        (*input)++;
    }
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int parse_number(const char** input, Token* token) {
    int len = 0;
    const char* start = *input;
    
    while (**input && isdigit(**input)) {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        token->value[len++] = **input;
        (*input)++;
    }
    
    if (len > 0) {
        token->value[len] = '\0';
        token->type = TOKEN_NUMBER;
        return 1;
    }
    
    *input = start;
    return 0;
}

int parse_identifier(const char** input, Token* token) {
    int len = 0;
    const char* start = *input;
    
    if (**input && (isalpha(**input) || **input == '_')) {
        token->value[len++] = **input;
        (*input)++;
        
        while (**input && (isalnum(**input) || **input == '_')) {
            if (len >= MAX_TOKEN_LEN - 1) break;
            token->value[len++] = **input;
            (*input)++;
        }
        
        token->value[len] = '\0';
        token->type = TOKEN_IDENTIFIER;
        return 1;
    }
    
    *input = start;
    return 0;
}

int parse_operator(const char** input, Token* token) {
    int len = 0;
    const char* start = *input;
    
    if (**input && is_operator_char(**input)) {
        token->value[len++] = **input;
        (*input)++;
        
        if (**input && is_operator_char(**input)) {
            if (len >= MAX_TOKEN_LEN - 1) {
                token->value[len] = '\0';
                token->type = TOKEN_OPERATOR;
                return 1;
            }
            token->value[len++] = **input;
            (*input)++;
        }
        
        token->value[len] = '\0';
        token->type = TOKEN_OPERATOR;
        return 1;
    }
    
    *input = start;
    return 0;
}

int parse_string(const char** input, Token* token) {
    int len = 0;
    const char* start = *input;
    
    if (**input == '"') {
        (*input)++;
        
        while (**input && **input != '"') {
            if (len >= MAX_TOKEN_LEN - 1) break;
            token->value[len++] = **input;
            (*input)++;
        }
        
        if (**input == '"') {
            (*input)++;
            token->value[len] = '\0';
            token->type = TOKEN_STRING;
            return 1;
        }
    }
    
    *input = start;
    return 0;
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    int token_count = 0;
    const char* current = input;
    
    while (*current && token_count < max_tokens) {
        skip_whitespace(&current);
        if (!*current) break;
        
        if (parse_number(&current, &tokens[token_count]) ||
            parse_identifier(&current, &tokens[token_count]) ||
            parse_operator(&current, &tokens[token_count]) ||
            parse_string(&current, &tokens[token_count])) {
            token_count++;
        } else {
            break;
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
    for (int i = 0; i < count; i++) {
        if (tokens[i].type == TOKEN_EOF) break;
        
        if (tokens[i].type >= 0 && tokens[i].type < 5) {
            handlers[tokens[i].type](&tokens[i]);
        }
    }
}

int main() {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (!fgets(input