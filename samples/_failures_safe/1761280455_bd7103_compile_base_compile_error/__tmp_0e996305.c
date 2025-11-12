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
    if (token == NULL) return;
    printf("Number: %s\n", token->value);
}

void handle_identifier(Token* token) {
    if (token == NULL) return;
    printf("Identifier: %s\n", token->value);
}

void handle_operator(Token* token) {
    if (token == NULL) return;
    printf("Operator: %s\n", token->value);
}

void handle_string(Token* token) {
    if (token == NULL) return;
    printf("String: %s\n", token->value);
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenType classify_token(const char* value) {
    if (value == NULL || value[0] == '\0') return TOKEN_EOF;
    
    if (isdigit(value[0])) return TOKEN_NUMBER;
    
    size_t len = strlen(value);
    if (len >= 2 && value[0] == '"' && value[len-1] == '"') return TOKEN_STRING;
    
    if (is_operator_char(value[0])) return TOKEN_OPERATOR;
    
    if (isalpha(value[0]) || value[0] == '_') return TOKEN_IDENTIFIER;
    
    return TOKEN_IDENTIFIER;
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    if (input == NULL || tokens == NULL || max_tokens <= 0) return 0;
    
    int token_count = 0;
    const char* ptr = input;
    
    while (*ptr != '\0' && token_count < max_tokens) {
        while (isspace(*ptr)) ptr++;
        
        if (*ptr == '\0') break;
        
        char token_value[MAX_TOKEN_LEN] = {0};
        int token_len = 0;
        
        if (isdigit(*ptr)) {
            while (isdigit(*ptr) && token_len < MAX_TOKEN_LEN - 1) {
                token_value[token_len++] = *ptr++;
            }
        } else if (is_operator_char(*ptr)) {
            token_value[token_len++] = *ptr++;
        } else if (*ptr == '"') {
            token_value[token_len++] = *ptr++;
            while (*ptr != '"' && *ptr != '\0' && token_len < MAX_TOKEN_LEN - 1) {
                token_value[token_len++] = *ptr++;
            }
            if (*ptr == '"') {
                token_value[token_len++] = *ptr++;
            }
        } else if (isalpha(*ptr) || *ptr == '_') {
            while ((isalnum(*ptr) || *ptr == '_') && token_len < MAX_TOKEN_LEN - 1) {
                token_value[token_len++] = *ptr++;
            }
        } else {
            ptr++;
            continue;
        }
        
        if (token_len > 0) {
            TokenType type = classify_token(token_value);
            tokens[token_count].type = type;
            strncpy(tokens[token_count].value, token_value, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            token_count++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        token_count++;
    }
    
    return token_count;
}

void process_tokens(Token* tokens, int count, TokenHandler* handlers) {
    if (tokens == NULL || handlers == NULL || count <= 0) return;
    
    for (int i = 0; i < count; i++) {
        if (tokens[i].type == TOKEN_EOF) break;
        
        if (handlers[tokens[i].type] != NULL) {
            handlers[tokens[i].type](&tokens[i]);
        }
    }
}

int main() {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count == 0) {
        printf("No tokens found\n");
        return 1;
    }
    
    TokenHandler handlers[5] = {
        handle_number,
        handle_identifier,
        handle_operator,
        handle_string,
        NULL
    };