//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_PAREN,
    TOKEN_EOF,
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int token_count = 0;
    const char *p = input;
    
    while (*p && token_count < max_tokens) {
        while (isspace(*p)) p++;
        
        if (!*p) break;
        
        if (isdigit(*p)) {
            int len = 0;
            while (isdigit(*p) && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *p++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
        } else if (isalpha(*p)) {
            int len = 0;
            while ((isalnum(*p) || *p == '_') && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *p++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            token_count++;
        } else if (is_operator_char(*p)) {
            tokens[token_count].value[0] = *p++;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
            token_count++;
        } else if (*p == '(' || *p == ')') {
            tokens[token_count].value[0] = *p++;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = TOKEN_PAREN;
            token_count++;
        } else {
            tokens[token_count].value[0] = *p++;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = TOKEN_INVALID;
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

const char *token_type_name(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_PAREN: return "PAREN";
        case TOKEN_EOF: return "EOF";
        case TOKEN_INVALID: return "INVALID";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input[1024];
    struct Token tokens[MAX_TOKENS];
    
    printf("Enter an expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count >= MAX_TOKENS) {
        fprintf(stderr, "Warning: Input too long, some tokens may be truncated\n");
    }
    
    printf("Tokens found: %d\n", token_count);
    printf("Token list:\n");
    
    for (int i = 0; i < token_count; i++) {
        printf("  %s: '%s'\n", token_type_name(tokens[i].type), tokens[i].value);
    }
    
    return 0;
}