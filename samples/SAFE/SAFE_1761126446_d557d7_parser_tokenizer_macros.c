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

TokenType classify_token(const char* token) {
    if (isdigit(token[0])) {
        for (size_t i = 0; token[i] != '\0'; i++) {
            if (!isdigit(token[i])) {
                return TOKEN_UNKNOWN;
            }
        }
        return TOKEN_NUMBER;
    }
    
    if (isalpha(token[0]) || token[0] == '_') {
        for (size_t i = 0; token[i] != '\0'; i++) {
            if (!is_valid_identifier_char(token[i])) {
                return TOKEN_UNKNOWN;
            }
        }
        return TOKEN_IDENTIFIER;
    }
    
    if (token[0] == '"' && token[strlen(token)-1] == '"') {
        return TOKEN_STRING;
    }
    
    if (strlen(token) == 1 && is_operator(token[0])) {
        return TOKEN_OPERATOR;
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
    
    while (*ptr && token_count < max_tokens) {
        while (*ptr && strchr(DELIMITERS, *ptr)) {
            ptr++;
        }
        
        if (!*ptr) break;
        
        size_t len = 0;
        
        if (*ptr == '"') {
            buffer[len++] = *ptr++;
            while (*ptr && *ptr != '"' && len < MAX_TOKEN_LEN - 1) {
                buffer[len++] = *ptr++;
            }
            if (*ptr == '"') {
                buffer[len++] = *ptr++;
            }
        } else if (is_operator(*ptr)) {
            buffer[len++] = *ptr++;
        } else {
            while (*ptr && !strchr(DELIMITERS, *ptr) && !is_operator(*ptr) && len < MAX_TOKEN_LEN - 1) {
                buffer[len++] = *ptr++;
            }
        }
        
        if (len > 0) {
            buffer[len] = '\0';
            if (token_count < max_tokens) {
                strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
                tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
                tokens[token_count].type = classify_token(buffer);
                token_count++;
            }
        }
    }
    
    return token_count;
}

void print_tokens(const Token* tokens, int count) {
    if (!tokens || count <= 0) {
        return;
    }
    
    for (int i = 0; i < count; i++) {
        const char* type_str;
        switch (tokens[i].type) {
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case TOKEN_IDENTIFIER: type_str = "IDENTIFIER"; break;
            case TOKEN_OPERATOR: type_str = "OPERATOR"; break;
            case TOKEN_STRING: type_str = "STRING"; break;
            default: type_str = "UNKNOWN"; break;
        }
        printf("Token %d: %s ('%s')\n", i + 1, type_str, tokens[i].value);
    }
}

int main(void) {
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
    print_tokens(tokens, token_count);
    
    return 0;
}