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
    while (**input && isspace((unsigned char)**input)) {
        (*input)++;
    }
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int parse_number(const char** input, Token* token) {
    int i = 0;
    while (**input && (isdigit((unsigned char)**input) || **input == '.')) {
        if (i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = **input;
            (*input)++;
        } else {
            return 0;
        }
    }
    token->value[i] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_identifier(const char** input, Token* token) {
    int i = 0;
    while (**input && (isalnum((unsigned char)**input) || **input == '_')) {
        if (i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = **input;
            (*input)++;
        } else {
            return 0;
        }
    }
    token->value[i] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_operator(const char** input, Token* token) {
    int i = 0;
    while (**input && is_operator_char(**input)) {
        if (i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = **input;
            (*input)++;
        } else {
            return 0;
        }
    }
    token->value[i] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

int parse_string(const char** input, Token* token) {
    if (**input != '"') return 0;
    (*input)++;
    
    int i = 0;
    while (**input && **input != '"') {
        if (i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = **input;
            (*input)++;
        } else {
            return 0;
        }
    }
    
    if (**input != '"') return 0;
    (*input)++;
    
    token->value[i] = '\0';
    token->type = TOKEN_STRING;
    return 1;
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    int token_count = 0;
    const char* ptr = input;
    
    while (*ptr && token_count < max_tokens) {
        skip_whitespace(&ptr);
        if (!*ptr) break;
        
        Token token;
        int success = 0;
        
        if (isdigit((unsigned char)*ptr)) {
            success = parse_number(&ptr, &token);
        } else if (isalpha((unsigned char)*ptr) || *ptr == '_') {
            success = parse_identifier(&ptr, &token);
        } else if (is_operator_char(*ptr)) {
            success = parse_operator(&ptr, &token);
        } else if (*ptr == '"') {
            success = parse_string(&ptr, &token);
        } else {
            break;
        }
        
        if (success) {
            if (token_count < max_tokens) {
                tokens[token_count++] = token;
            }
        } else {
            break;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strncpy(tokens[token_count].value, "", MAX_TOKEN_LEN - 1);
        tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
        token_count++;
    }
    
    return token_count;
}

void process_tokens(Token* tokens, int count, TokenHandler* handlers) {
    for (int i = 0; i < count; i++) {
        if (tokens[i].type == TOKEN_EOF) break;
        
        if (handlers[tokens[i].type]) {
            handlers[tokens[i].type](&tokens[i]);
        }
    }
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1]