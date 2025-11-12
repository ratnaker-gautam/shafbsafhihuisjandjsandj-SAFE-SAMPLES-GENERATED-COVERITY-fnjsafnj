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
    printf("Found number: %s\n", token->value);
}

void handle_identifier(Token* token) {
    printf("Found identifier: %s\n", token->value);
}

void handle_operator(Token* token) {
    printf("Found operator: %s\n", token->value);
}

void handle_string(Token* token) {
    printf("Found string: %s\n", token->value);
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
    
    while (**input && (isdigit(**input) || **input == '.')) {
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
        memset(&token, 0, sizeof(Token));
        
        if (parse_number(&current, token.value)) {
            token.type = TOKEN_NUMBER;
        } else if (parse_identifier(&current, token.value)) {
            token.type = TOKEN_IDENTIFIER;
        } else if (parse_operator(&current, token.value)) {
            token.type = TOKEN_OPERATOR;
        } else if (parse_string(&current, token.value)) {
            token.type = TOKEN_STRING;
        } else {
            break;
        }
        
        if (token_count < max_tokens) {
            tokens[token_count++] = token;
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
        if (tokens[i].type == TOKEN_EOF) {
            break;
        }
        
        if (handlers[tokens[i].type]) {
            handlers[tokens[i].type](&tokens[i]);
        }
    }
}

int main() {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];