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
    TOKEN_WORD,
    TOKEN_OPERATOR,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int tokenize_string(const char *input, Token *tokens, int max_tokens) {
    if (!input || !tokens || max_tokens <= 0) return 0;
    
    int token_count = 0;
    const char *ptr = input;
    
    while (*ptr && token_count < max_tokens) {
        while (*ptr && strchr(DELIMITERS, *ptr)) ptr++;
        if (!*ptr) break;
        
        const char *start = ptr;
        TokenType type = TOKEN_UNKNOWN;
        
        if (isdigit(*ptr)) {
            type = TOKEN_NUMBER;
            while (*ptr && (isdigit(*ptr) || *ptr == '.')) ptr++;
        } else if (is_operator(*ptr)) {
            type = TOKEN_OPERATOR;
            ptr++;
        } else if (isalpha(*ptr) || *ptr == '_') {
            type = TOKEN_WORD;
            while (*ptr && (isalnum(*ptr) || *ptr == '_')) ptr++;
        } else {
            ptr++;
        }
        
        size_t len = ptr - start;
        if (len > 0 && len < MAX_TOKEN_LEN) {
            strncpy(tokens[token_count].value, start, len);
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = type;
            token_count++;
        }
    }
    
    return token_count;
}

void print_token_type(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_WORD: printf("WORD"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        default: printf("UNKNOWN"); break;
    }
}

int main() {
    char input[1024];
    Token tokens[MAX_TOKENS];
    
    printf("Enter text to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
        printf("Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    int token_count = tokenize_string(input, tokens, MAX_TOKENS);
    
    if (token_count == 0) {
        printf("No tokens found\n");
        return 1;
    }
    
    printf("Found %d tokens:\n", token_count);
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: '%-15s' Type: ", i + 1, tokens[i].value);
        print_token_type(tokens[i].type);
        printf("\n");
    }
    
    return 0;
}