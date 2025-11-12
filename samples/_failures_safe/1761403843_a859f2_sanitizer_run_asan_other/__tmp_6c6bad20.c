//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50
#define DELIMITERS " \t\n\r"

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

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenType classify_token(const char* str) {
    if (str[0] == '\0') return TOKEN_EOF;
    
    if (isdigit(str[0])) {
        for (size_t i = 1; str[i] != '\0'; i++) {
            if (!isdigit(str[i])) return TOKEN_UNKNOWN;
        }
        return TOKEN_NUMBER;
    }
    
    if (isalpha(str[0]) || str[0] == '_') {
        for (size_t i = 1; str[i] != '\0'; i++) {
            if (!isalnum(str[i]) && str[i] != '_') return TOKEN_UNKNOWN;
        }
        return TOKEN_IDENTIFIER;
    }
    
    if (is_operator(str[0])) {
        if (str[1] == '\0') return TOKEN_OPERATOR;
        if ((str[0] == '<' || str[0] == '>' || str[0] == '=') && str[1] == '=') {
            if (str[2] == '\0') return TOKEN_OPERATOR;
        }
        return TOKEN_UNKNOWN;
    }
    
    if (str[0] == '"') {
        size_t len = strlen(str);
        if (len < 2 || str[len-1] != '"') return TOKEN_UNKNOWN;
        for (size_t i = 1; i < len-1; i++) {
            if (str[i] == '"') return TOKEN_UNKNOWN;
        }
        return TOKEN_STRING;
    }
    
    return TOKEN_UNKNOWN;
}

int tokenize(const char* input, Token* tokens) {
    char buffer[MAX_TOKEN_LEN];
    char* copy = strdup(input);
    if (!copy) return -1;
    
    char* saveptr = NULL;
    char* token_str = strtok_r(copy, DELIMITERS, &saveptr);
    int count = 0;
    
    while (token_str && count < MAX_TOKENS) {
        size_t len = strlen(token_str);
        if (len >= MAX_TOKEN_LEN) {
            free(copy);
            return -1;
        }
        
        strncpy(buffer, token_str, MAX_TOKEN_LEN-1);
        buffer[MAX_TOKEN_LEN-1] = '\0';
        
        TokenType type = classify_token(buffer);
        tokens[count].type = type;
        strncpy(tokens[count].value, buffer, MAX_TOKEN_LEN-1);
        tokens[count].value[MAX_TOKEN_LEN-1] = '\0';
        
        count++;
        token_str = strtok_r(NULL, DELIMITERS, &saveptr);
    }
    
    free(copy);
    return count;
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

int main() {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens);
    
    if (token_count < 0) {
        fprintf(stderr, "Tokenization failed\n");
        return 1;
    }
    
    printf("Tokens found: %d\n", token_count);
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: '%-15s' Type: ", i+1, tokens[i].value);
        print_token_type(tokens[i].type);
        printf("\n");
    }
    
    return 0;
}