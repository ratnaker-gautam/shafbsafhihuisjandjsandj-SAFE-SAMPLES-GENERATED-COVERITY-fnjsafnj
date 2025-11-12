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

TokenType classify_token(const char* str) {
    if (str == NULL || strlen(str) == 0) return TOKEN_UNKNOWN;
    
    if (isdigit(str[0])) {
        for (size_t i = 0; i < strlen(str); i++) {
            if (!isdigit(str[i])) return TOKEN_UNKNOWN;
        }
        return TOKEN_NUMBER;
    }
    
    if (isalpha(str[0]) || str[0] == '_') {
        for (size_t i = 0; i < strlen(str); i++) {
            if (!isalnum(str[i]) && str[i] != '_') return TOKEN_UNKNOWN;
        }
        return TOKEN_IDENTIFIER;
    }
    
    if (is_operator_char(str[0])) {
        if (strlen(str) == 1) return TOKEN_OPERATOR;
        return TOKEN_UNKNOWN;
    }
    
    if (str[0] == '"') {
        size_t len = strlen(str);
        if (len >= 2 && str[len-1] == '"') return TOKEN_STRING;
        return TOKEN_UNKNOWN;
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
        
        const char* start = ptr;
        size_t token_len = 0;
        
        if (isdigit(*ptr)) {
            while (isdigit(*ptr) && token_len < MAX_TOKEN_LEN - 1) {
                ptr++;
                token_len++;
            }
        } else if (isalpha(*ptr) || *ptr == '_') {
            while ((isalnum(*ptr) || *ptr == '_') && token_len < MAX_TOKEN_LEN - 1) {
                ptr++;
                token_len++;
            }
        } else if (is_operator_char(*ptr)) {
            token_len = 1;
            ptr++;
        } else if (*ptr == '"') {
            token_len = 1;
            ptr++;
            while (*ptr != '"' && *ptr != '\0' && token_len < MAX_TOKEN_LEN - 1) {
                ptr++;
                token_len++;
            }
            if (*ptr == '"') {
                ptr++;
                token_len++;
            }
        } else {
            token_len = 1;
            ptr++;
        }
        
        if (token_len > 0 && token_len < MAX_TOKEN_LEN) {
            strncpy(tokens[token_count].value, start, token_len);
            tokens[token_count].value[token_len] = '\0';
            tokens[token_count].type = classify_token(tokens[token_count].value);
            token_count++;
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
    if (tokens == NULL || handlers == NULL || count <= 0) return;
    
    for (int i = 0; i < count; i++) {
        if (tokens[i].type == TOKEN_EOF) break;
        
        if (tokens[i].type < TOKEN_NUMBER || tokens[i].type > TOKEN_UNKNOWN) {
            tokens[i].type = TOKEN_UNKNOWN;
        }
        
        if (handlers[tokens[i].type] != NULL) {
            handlers[tokens[i].type](&tokens[i]);
        }
    }
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {