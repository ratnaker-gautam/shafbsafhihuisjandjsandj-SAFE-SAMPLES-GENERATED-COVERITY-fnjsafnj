//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

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

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenType classify_token(const char* str) {
    if (str[0] == '\0') return TOKEN_EOF;
    
    if (isdigit(str[0])) {
        for (size_t i = 0; str[i] != '\0'; i++) {
            if (!isdigit(str[i])) return TOKEN_UNKNOWN;
        }
        return TOKEN_NUMBER;
    }
    
    if (isalpha(str[0])) {
        for (size_t i = 0; str[i] != '\0'; i++) {
            if (!isalnum(str[i])) return TOKEN_UNKNOWN;
        }
        return TOKEN_IDENTIFIER;
    }
    
    if (is_operator_char(str[0])) {
        return TOKEN_OPERATOR;
    }
    
    if (str[0] == '"') {
        return TOKEN_STRING;
    }
    
    return TOKEN_UNKNOWN;
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    if (input == NULL || tokens == NULL || max_tokens <= 0) {
        return -1;
    }
    
    int token_count = 0;
    const char* pos = input;
    
    while (*pos != '\0' && token_count < max_tokens) {
        while (isspace(*pos)) pos++;
        
        if (*pos == '\0') break;
        
        const char* start = pos;
        size_t len = 0;
        
        if (is_operator_char(*pos)) {
            len = 1;
        } else if (*pos == '"') {
            len = 1;
            pos++;
            while (*pos != '\0' && *pos != '"') {
                len++;
                pos++;
            }
            if (*pos == '"') {
                len++;
                pos++;
            }
        } else if (isalnum(*pos)) {
            while (isalnum(*pos)) {
                len++;
                pos++;
            }
        } else {
            len = 1;
            pos++;
        }
        
        if (len >= MAX_TOKEN_LEN) {
            len = MAX_TOKEN_LEN - 1;
        }
        
        strncpy(tokens[token_count].value, start, len);
        tokens[token_count].value[len] = '\0';
        tokens[token_count].type = classify_token(tokens[token_count].value);
        
        token_count++;
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        token_count++;
    }
    
    return token_count;
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

void print_tokens(const Token* tokens, int count) {
    if (tokens == NULL || count <= 0) {
        return;
    }
    
    printf("Tokenized output:\n");
    for (int i = 0; i < count; i++) {
        printf("Token %d: '%-10s' Type: ", i, tokens[i].value);
        print_token_type(tokens[i].type);
        printf("\n");
    }
}

int main(void) {
    char input[1024];
    Token tokens[MAX_TOKENS];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    if (token_count < 0) {
        printf("Tokenization failed\n");
        return 1;
    }
    
    print_tokens(tokens, token_count);
    
    return 0;
}