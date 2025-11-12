//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64
#define DELIMITERS " \t\n\r"

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

Token tokens[MAX_TOKENS];
size_t token_count = 0;
size_t current_token = 0;

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

void tokenize(const char* input) {
    const char* ptr = input;
    token_count = 0;
    
    while (*ptr && token_count < MAX_TOKENS) {
        while (*ptr && strchr(DELIMITERS, *ptr)) ptr++;
        if (!*ptr) break;
        
        if (isalpha(*ptr) || *ptr == '_') {
            size_t len = 0;
            while ((isalnum(*ptr) || *ptr == '_') && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *ptr++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            token_count++;
        }
        else if (isdigit(*ptr)) {
            size_t len = 0;
            while (isdigit(*ptr) && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *ptr++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
        }
        else if (*ptr == '"') {
            ptr++;
            size_t len = 0;
            while (*ptr != '"' && *ptr && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *ptr++;
            }
            tokens[token_count].value[len] = '\0';
            if (*ptr == '"') ptr++;
            tokens[token_count].type = TOKEN_STRING;
            token_count++;
        }
        else if (is_operator(*ptr)) {
            tokens[token_count].value[0] = *ptr++;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
            token_count++;
        }
        else {
            tokens[token_count].value[0] = *ptr++;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = TOKEN_UNKNOWN;
            token_count++;
        }
    }
    
    tokens[token_count].type = TOKEN_EOF;
    strcpy(tokens[token_count].value, "");
}

void print_token(Token token) {
    const char* type_names[] = {
        "IDENTIFIER", "NUMBER", "OPERATOR", "STRING", "EOF", "UNKNOWN"
    };
    printf("%-12s: %s\n", type_names[token.type], token.value);
}

int main(void) {
    char input[1024];
    
    printf("Enter expression to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (strlen(input) >= sizeof(input) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    tokenize(input);
    
    printf("\nTokens:\n");
    for (size_t i = 0; i < token_count; i++) {
        print_token(tokens[i]);
    }
    
    return 0;
}