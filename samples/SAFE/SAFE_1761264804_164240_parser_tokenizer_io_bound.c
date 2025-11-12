//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_SYMBOL,
    TOKEN_EOF
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
};

int is_symbol_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int parse_tokens(const char *input, struct Token *tokens, int max_tokens) {
    int token_count = 0;
    const char *p = input;
    
    while (*p && token_count < max_tokens) {
        while (isspace((unsigned char)*p)) p++;
        
        if (!*p) break;
        
        if (isdigit((unsigned char)*p)) {
            int len = 0;
            while (isdigit((unsigned char)*p) && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *p++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
        }
        else if (*p == '"') {
            p++;
            int len = 0;
            while (*p && *p != '"' && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *p++;
            }
            if (*p == '"') p++;
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_STRING;
            token_count++;
        }
        else if (is_symbol_char(*p)) {
            int len = 0;
            while (is_symbol_char(*p) && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *p++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_SYMBOL;
            token_count++;
        }
        else {
            break;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "");
        token_count++;
    }
    
    return token_count;
}

void print_token_type(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_SYMBOL: printf("SYMBOL"); break;
        case TOKEN_EOF: printf("EOF"); break;
    }
}

int main() {
    char input[1024];
    struct Token tokens[MAX_TOKENS];
    
    printf("Enter text to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    int token_count = parse_tokens(input, tokens, MAX_TOKENS);
    
    printf("\nTokens found:\n");
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: Type=", i + 1);
        print_token_type(tokens[i].type);
        printf(", Value='%s'\n", tokens[i].value);
    }
    
    return 0;
}