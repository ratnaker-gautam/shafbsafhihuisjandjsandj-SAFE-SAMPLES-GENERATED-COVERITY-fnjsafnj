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

TokenType classify_token(const char* token) {
    if (isdigit(token[0])) {
        return TOKEN_NUMBER;
    } else if (isalpha(token[0]) || token[0] == '_') {
        return TOKEN_IDENTIFIER;
    } else if (is_operator(token[0])) {
        return TOKEN_OPERATOR;
    } else if (token[0] == '"') {
        return TOKEN_STRING;
    }
    return TOKEN_UNKNOWN;
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    char buffer[MAX_TOKEN_LEN];
    int token_count = 0;
    const char* ptr = input;
    
    while (*ptr && token_count < max_tokens) {
        while (*ptr && strchr(DELIMITERS, *ptr)) {
            ptr++;
        }
        
        if (!*ptr) break;
        
        int buf_index = 0;
        memset(buffer, 0, sizeof(buffer));
        
        if (is_operator(*ptr)) {
            buffer[buf_index++] = *ptr++;
        } else if (*ptr == '"') {
            buffer[buf_index++] = *ptr++;
            while (*ptr && *ptr != '"' && buf_index < MAX_TOKEN_LEN - 1) {
                buffer[buf_index++] = *ptr++;
            }
            if (*ptr == '"') {
                buffer[buf_index++] = *ptr++;
            }
        } else {
            while (*ptr && !strchr(DELIMITERS, *ptr) && !is_operator(*ptr) && 
                   buf_index < MAX_TOKEN_LEN - 1) {
                buffer[buf_index++] = *ptr++;
            }
        }
        
        if (buf_index > 0) {
            strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            tokens[token_count].type = classify_token(buffer);
            token_count++;
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
    
    printf("Enter expression to tokenize: ");
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