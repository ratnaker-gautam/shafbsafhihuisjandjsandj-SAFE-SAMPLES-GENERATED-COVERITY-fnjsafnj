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
    return strchr("+-*/=<>!&|", c) != NULL;
}

int parse_number(const char** input, Token* token) {
    const char* start = *input;
    int len = 0;
    
    while (**input && (isdigit(**input) || **input == '.')) {
        if (len < MAX_TOKEN_LEN - 1) {
            token->value[len++] = **input;
        }
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
    const char* start = *input;
    int len = 0;
    
    if (**input && (isalpha(**input) || **input == '_')) {
        token->value[len++] = **input;
        (*input)++;
        
        while (**input && (isalnum(**input) || **input == '_')) {
            if (len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = **input;
            }
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
    const char* start = *input;
    int len = 0;
    
    if (**input && is_operator_char(**input)) {
        token->value[len++] = **input;
        (*input)++;
        
        if (**input && is_operator_char(**input)) {
            if (len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = **input;
            }
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
    const char* start = *input;
    int len = 0;
    
    if (**input == '"') {
        (*input)++;
        
        while (**input && **input != '"') {
            if (len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = **input;
            }
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
    int count = 0;
    
    while (*input && count < max_tokens) {
        skip_whitespace(&input);
        
        if (!*input) break;
        
        Token token;
        int parsed = 0;
        
        if (parse_string(&input, &token)) {
            parsed = 1;
        } else if (parse_number(&input, &token)) {
            parsed = 1;
        } else if (parse_identifier(&input, &token)) {
            parsed = 1;
        } else if (parse_operator(&input, &token)) {
            parsed = 1;
        }
        
        if (parsed) {
            if (count < max_tokens) {
                tokens[count++] = token;
            }
        } else {
            input++;
        }
    }
    
    if (count < max_tokens) {
        tokens[count].type = TOKEN_EOF;
        tokens[count].value[0] = '\0';
        count++;
    }
    
    return count;
}

void process_tokens(Token* tokens, int count, TokenHandler* handlers) {
    for (int i = 0; i < count; i++) {
        if (tokens[i].type == TOKEN_EOF) break;
        
        if (handlers[tokens[i].type]) {
            handlers[tokens[i].type](&tokens[i]);
        }
    }
}

int main() {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr