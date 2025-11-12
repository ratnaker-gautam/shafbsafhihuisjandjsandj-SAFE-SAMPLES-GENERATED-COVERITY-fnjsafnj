//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: parser_tokenizer
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

typedef struct {
    Token* tokens;
    size_t capacity;
    size_t count;
} TokenArray;

TokenArray* create_token_array(size_t capacity) {
    if (capacity == 0 || capacity > MAX_TOKENS) {
        return NULL;
    }
    
    TokenArray* array = malloc(sizeof(TokenArray));
    if (!array) {
        return NULL;
    }
    
    array->tokens = malloc(sizeof(Token) * capacity);
    if (!array->tokens) {
        free(array);
        return NULL;
    }
    
    array->capacity = capacity;
    array->count = 0;
    return array;
}

void destroy_token_array(TokenArray* array) {
    if (array) {
        free(array->tokens);
        free(array);
    }
}

int add_token(TokenArray* array, TokenType type, const char* value) {
    if (!array || !value || array->count >= array->capacity) {
        return 0;
    }
    
    size_t len = strlen(value);
    if (len >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    Token* token = &array->tokens[array->count];
    token->type = type;
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
    array->count++;
    return 1;
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenArray* tokenize(const char* input) {
    if (!input) {
        return NULL;
    }
    
    TokenArray* tokens = create_token_array(MAX_TOKENS);
    if (!tokens) {
        return NULL;
    }
    
    size_t i = 0;
    while (input[i] != '\0' && tokens->count < tokens->capacity) {
        if (isspace(input[i])) {
            i++;
            continue;
        }
        
        if (isdigit(input[i])) {
            char number[MAX_TOKEN_LEN] = {0};
            size_t j = 0;
            
            while (isdigit(input[i]) && j < MAX_TOKEN_LEN - 1) {
                number[j++] = input[i++];
            }
            number[j] = '\0';
            
            if (!add_token(tokens, TOKEN_NUMBER, number)) {
                destroy_token_array(tokens);
                return NULL;
            }
            continue;
        }
        
        if (isalpha(input[i]) || input[i] == '_') {
            char identifier[MAX_TOKEN_LEN] = {0};
            size_t j = 0;
            
            while ((isalnum(input[i]) || input[i] == '_') && j < MAX_TOKEN_LEN - 1) {
                identifier[j++] = input[i++];
            }
            identifier[j] = '\0';
            
            if (!add_token(tokens, TOKEN_IDENTIFIER, identifier)) {
                destroy_token_array(tokens);
                return NULL;
            }
            continue;
        }
        
        if (input[i] == '"') {
            char string[MAX_TOKEN_LEN] = {0};
            size_t j = 0;
            i++;
            
            while (input[i] != '"' && input[i] != '\0' && j < MAX_TOKEN_LEN - 1) {
                string[j++] = input[i++];
            }
            
            if (input[i] == '"') {
                i++;
                string[j] = '\0';
                if (!add_token(tokens, TOKEN_STRING, string)) {
                    destroy_token_array(tokens);
                    return NULL;
                }
            }
            continue;
        }
        
        if (is_operator(input[i])) {
            char op[2] = {input[i++], '\0'};
            if (!add_token(tokens, TOKEN_OPERATOR, op)) {
                destroy_token_array(tokens);
                return NULL;
            }
            continue;
        }
        
        i++;
    }
    
    if (!add_token(tokens, TOKEN_EOF, "")) {
        destroy_token_array(tokens);
        return NULL;
    }
    
    return tokens;
}

const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
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
        len--;
    }
    
    if (len == 0