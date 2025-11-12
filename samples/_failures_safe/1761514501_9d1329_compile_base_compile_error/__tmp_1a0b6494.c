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
    TOKEN_EOF,
    TOKEN_UNKNOWN
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

void handle_unknown(Token* token) {
    if (token == NULL) return;
    printf("Unknown: %s\n", token->value);
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenType classify_token(const char* value) {
    if (value == NULL || strlen(value) == 0) return TOKEN_UNKNOWN;
    
    if (isdigit(value[0])) {
        for (size_t i = 0; i < strlen(value); i++) {
            if (!isdigit(value[i])) return TOKEN_UNKNOWN;
        }
        return TOKEN_NUMBER;
    }
    
    if (is_operator_char(value[0])) {
        return TOKEN_OPERATOR;
    }
    
    if (isalpha(value[0]) || value[0] == '_') {
        for (size_t i = 0; i < strlen(value); i++) {
            if (!isalnum(value[i]) && value[i] != '_') return TOKEN_UNKNOWN;
        }
        return TOKEN_IDENTIFIER;
    }
    
    if (value[0] == '"') {
        size_t len = strlen(value);
        if (len >= 2 && value[len-1] == '"') return TOKEN_STRING;
    }
    
    return TOKEN_UNKNOWN;
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    if (input == NULL || tokens == NULL || max_tokens <= 0) return 0;
    
    int token_count = 0;
    const char* ptr = input;
    
    while (*ptr != '\0' && token_count < max_tokens) {
        while (isspace(*ptr)) ptr++;
        if (*ptr == '\0') break;
        
        Token token = {TOKEN_UNKNOWN, ""};
        size_t len = 0;
        
        if (isdigit(*ptr)) {
            while (isdigit(*ptr) && len < MAX_TOKEN_LEN - 1) {
                token.value[len++] = *ptr++;
            }
            token.type = TOKEN_NUMBER;
        }
        else if (is_operator_char(*ptr)) {
            token.value[len++] = *ptr++;
            token.type = TOKEN_OPERATOR;
        }
        else if (isalpha(*ptr) || *ptr == '_') {
            while ((isalnum(*ptr) || *ptr == '_') && len < MAX_TOKEN_LEN - 1) {
                token.value[len++] = *ptr++;
            }
            token.type = TOKEN_IDENTIFIER;
        }
        else if (*ptr == '"') {
            token.value[len++] = *ptr++;
            while (*ptr != '"' && *ptr != '\0' && len < MAX_TOKEN_LEN - 1) {
                token.value[len++] = *ptr++;
            }
            if (*ptr == '"') {
                token.value[len++] = *ptr++;
                token.type = TOKEN_STRING;
            }
        }
        else {
            token.value[len++] = *ptr++;
            token.type = TOKEN_UNKNOWN;
        }
        
        token.value[len] = '\0';
        token.type = classify_token(token.value);
        tokens[token_count++] = token;
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "");
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
        } else {
            handle_unknown(&tokens[i]);
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
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input