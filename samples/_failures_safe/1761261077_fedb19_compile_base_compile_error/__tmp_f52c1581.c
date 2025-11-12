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
    int line;
} Token;

typedef struct {
    Token* tokens;
    int capacity;
    int count;
} TokenArray;

TokenArray* create_token_array(int capacity) {
    if (capacity <= 0) return NULL;
    TokenArray* arr = malloc(sizeof(TokenArray));
    if (!arr) return NULL;
    arr->tokens = malloc(sizeof(Token) * capacity);
    if (!arr->tokens) {
        free(arr);
        return NULL;
    }
    arr->capacity = capacity;
    arr->count = 0;
    return arr;
}

void destroy_token_array(TokenArray* arr) {
    if (arr) {
        free(arr->tokens);
        free(arr);
    }
}

int add_token(TokenArray* arr, TokenType type, const char* value, int line) {
    if (!arr || !value || arr->count >= arr->capacity) return 0;
    if (strlen(value) >= MAX_TOKEN_LEN) return 0;
    
    Token* token = &arr->tokens[arr->count];
    token->type = type;
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
    token->line = line;
    arr->count++;
    return 1;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int tokenize(const char* input, TokenArray* tokens) {
    if (!input || !tokens) return 0;
    
    int line = 1;
    int pos = 0;
    int len = strlen(input);
    
    while (pos < len && tokens->count < tokens->capacity) {
        while (pos < len && isspace(input[pos])) {
            if (input[pos] == '\n') line++;
            pos++;
        }
        if (pos >= len) break;
        
        char current = input[pos];
        
        if (isdigit(current)) {
            char number[MAX_TOKEN_LEN] = {0};
            int num_len = 0;
            
            while (pos < len && isdigit(input[pos]) && num_len < MAX_TOKEN_LEN - 1) {
                number[num_len++] = input[pos++];
            }
            number[num_len] = '\0';
            
            if (!add_token(tokens, TOKEN_NUMBER, number, line)) return 0;
        }
        else if (isalpha(current) || current == '_') {
            char identifier[MAX_TOKEN_LEN] = {0};
            int id_len = 0;
            
            while (pos < len && (isalnum(input[pos]) || input[pos] == '_') && id_len < MAX_TOKEN_LEN - 1) {
                identifier[id_len++] = input[pos++];
            }
            identifier[id_len] = '\0';
            
            if (!add_token(tokens, TOKEN_IDENTIFIER, identifier, line)) return 0;
        }
        else if (current == '"') {
            char string[MAX_TOKEN_LEN] = {0};
            int str_len = 0;
            pos++;
            
            while (pos < len && input[pos] != '"' && str_len < MAX_TOKEN_LEN - 1) {
                string[str_len++] = input[pos++];
            }
            string[str_len] = '\0';
            
            if (pos < len && input[pos] == '"') {
                pos++;
                if (!add_token(tokens, TOKEN_STRING, string, line)) return 0;
            } else {
                return 0;
            }
        }
        else if (is_operator_char(current)) {
            char op[2] = {current, '\0'};
            pos++;
            if (!add_token(tokens, TOKEN_OPERATOR, op, line)) return 0;
        }
        else {
            pos++;
        }
    }
    
    if (tokens->count < tokens->capacity) {
        add_token(tokens, TOKEN_EOF, "", line);
    }
    
    return 1;
}

void print_token_type(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_EOF: printf("EOF"); break;
        default: printf("UNKNOWN"); break;
    }
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        printf("Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    TokenArray* tokens = create_token_array(MAX_TOKENS);
    if (!tokens) {