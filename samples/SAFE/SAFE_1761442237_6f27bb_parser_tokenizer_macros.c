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
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int tokenize(const char* input, Token* tokens, int max_tokens) {
    if (!input || !tokens || max_tokens <= 0) return -1;
    
    int token_count = 0;
    const char* pos = input;
    
    while (*pos && token_count < max_tokens) {
        while (*pos && strchr(DELIMITERS, *pos)) pos++;
        if (!*pos) break;
        
        const char* start = pos;
        size_t len = 0;
        
        if (isdigit(*pos)) {
            while (*pos && isdigit(*pos) && len < MAX_TOKEN_LEN - 1) {
                pos++;
                len++;
            }
            tokens[token_count].type = TOKEN_NUMBER;
        } else if (isalpha(*pos)) {
            while (*pos && isalnum(*pos) && len < MAX_TOKEN_LEN - 1) {
                pos++;
                len++;
            }
            tokens[token_count].type = TOKEN_WORD;
        } else {
            if (len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *pos++;
                tokens[token_count].type = TOKEN_OPERATOR;
            }
        }
        
        if (len > 0 && len < MAX_TOKEN_LEN) {
            strncpy(tokens[token_count].value, start, len);
            tokens[token_count].value[len] = '\0';
            token_count++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
    }
    
    return token_count;
}

void print_tokens(const Token* tokens, int count) {
    if (!tokens || count <= 0) return;
    
    for (int i = 0; i < count; i++) {
        const char* type_str = "UNKNOWN";
        switch (tokens[i].type) {
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case TOKEN_WORD: type_str = "WORD"; break;
            case TOKEN_OPERATOR: type_str = "OPERATOR"; break;
            case TOKEN_EOF: type_str = "EOF"; break;
        }
        printf("Token %d: %s ('%s')\n", i, type_str, tokens[i].value);
    }
}

int main() {
    char input[1024];
    Token tokens[MAX_TOKENS];
    
    printf("Enter text to tokenize: ");
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
    
    printf("\nTokens found: %d\n", token_count);
    print_tokens(tokens, token_count);
    
    return 0;
}