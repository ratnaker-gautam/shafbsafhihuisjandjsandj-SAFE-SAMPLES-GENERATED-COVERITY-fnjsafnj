//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50
#define DELIMITERS " \t\n"

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_valid_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

TokenType classify_token(const char* str) {
    if (isdigit(str[0])) {
        for (int i = 0; str[i] != '\0'; i++) {
            if (!isdigit(str[i])) {
                return TOKEN_UNKNOWN;
            }
        }
        return TOKEN_NUMBER;
    }
    
    if (isalpha(str[0]) || str[0] == '_') {
        for (int i = 0; str[i] != '\0'; i++) {
            if (!is_valid_identifier_char(str[i])) {
                return TOKEN_UNKNOWN;
            }
        }
        return TOKEN_IDENTIFIER;
    }
    
    if (strlen(str) == 1 && is_operator(str[0])) {
        return TOKEN_OPERATOR;
    }
    
    if (str[0] == '"' && str[strlen(str)-1] == '"') {
        return TOKEN_STRING;
    }
    
    return TOKEN_UNKNOWN;
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    if (!input || !tokens || max_tokens <= 0) {
        return -1;
    }
    
    char buffer[MAX_TOKEN_LEN];
    int token_count = 0;
    const char* ptr = input;
    
    while (*ptr != '\0' && token_count < max_tokens) {
        while (*ptr != '\0' && strchr(DELIMITERS, *ptr)) {
            ptr++;
        }
        
        if (*ptr == '\0') {
            break;
        }
        
        int buf_index = 0;
        
        if (*ptr == '"') {
            buffer[buf_index++] = *ptr++;
            while (*ptr != '\0' && *ptr != '"' && buf_index < MAX_TOKEN_LEN - 1) {
                buffer[buf_index++] = *ptr++;
            }
            if (*ptr == '"') {
                buffer[buf_index++] = *ptr++;
            }
        } else if (is_operator(*ptr)) {
            buffer[buf_index++] = *ptr++;
        } else {
            while (*ptr != '\0' && !strchr(DELIMITERS, *ptr) && 
                   !is_operator(*ptr) && buf_index < MAX_TOKEN_LEN - 1) {
                buffer[buf_index++] = *ptr++;
            }
        }
        
        if (buf_index > 0) {
            buffer[buf_index] = '\0';
            if (buf_index < MAX_TOKEN_LEN) {
                tokens[token_count].type = classify_token(buffer);
                strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
                tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
                token_count++;
            }
        }
    }
    
    return token_count;
}

void print_token_type(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        default: printf("UNKNOWN"); break;
    }
}

int main() {
    char input[1024];
    Token tokens[MAX_TOKENS];
    
    printf("Enter input to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count < 0) {
        fprintf(stderr, "Tokenization failed\n");
        return 1;
    }
    
    if (token_count == 0) {
        printf("No tokens found\n");
        return 0;
    }
    
    printf("Found %d tokens:\n", token_count);
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: '%-15s' Type: ", i + 1, tokens[i].value);
        print_token_type(tokens[i].type);
        printf("\n");
    }
    
    return 0;
}