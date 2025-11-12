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

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    if (!input || !tokens || max_tokens <= 0) return -1;
    
    int token_count = 0;
    const char *ptr = input;
    
    while (*ptr && token_count < max_tokens) {
        while (*ptr && strchr(DELIMITERS, *ptr)) ptr++;
        if (!*ptr) break;
        
        Token *current = &tokens[token_count];
        memset(current->value, 0, MAX_TOKEN_LEN);
        
        if (isdigit(*ptr)) {
            current->type = TOKEN_NUMBER;
            int i = 0;
            while (*ptr && isdigit(*ptr) && i < MAX_TOKEN_LEN - 1) {
                current->value[i++] = *ptr++;
            }
        } else if (isalpha(*ptr) || *ptr == '_') {
            current->type = TOKEN_IDENTIFIER;
            int i = 0;
            while (*ptr && (isalnum(*ptr) || *ptr == '_') && i < MAX_TOKEN_LEN - 1) {
                current->value[i++] = *ptr++;
            }
        } else if (*ptr == '"') {
            current->type = TOKEN_STRING;
            int i = 0;
            ptr++;
            while (*ptr && *ptr != '"' && i < MAX_TOKEN_LEN - 1) {
                current->value[i++] = *ptr++;
            }
            if (*ptr == '"') ptr++;
        } else if (is_operator_char(*ptr)) {
            current->type = TOKEN_OPERATOR;
            int i = 0;
            while (*ptr && is_operator_char(*ptr) && i < MAX_TOKEN_LEN - 1) {
                current->value[i++] = *ptr++;
            }
        } else {
            current->type = TOKEN_UNKNOWN;
            current->value[0] = *ptr++;
            current->value[1] = '\0';
        }
        
        token_count++;
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "");
        token_count++;
    }
    
    return token_count;
}

const char* token_type_name(TokenType type) {
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
    
    Token tokens[MAX_TOKENS];
    int count = tokenize(input, tokens, MAX_TOKENS);
    
    if (count < 0) {
        fprintf(stderr, "Tokenization failed\n");
        return 1;
    }
    
    printf("Tokens found: %d\n", count);
    printf("%-15s %-15s %s\n", "Index", "Type", "Value");
    printf("--------------- --------------- ---------------\n");
    
    for (int i = 0; i < count; i++) {
        if (i >= MAX_TOKENS) break;
        printf("%-15d %-15s %-15s\n", 
               i, 
               token_type_name(tokens[i].type),
               tokens[i].value);
    }
    
    return 0;
}