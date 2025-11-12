//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 1000
#define MAX_TOKEN_LEN 256

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

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

TokenType classify_token(const char* str) {
    if (isdigit(str[0])) return TOKEN_NUMBER;
    if (str[0] == '"') return TOKEN_STRING;
    if (is_operator_char(str[0])) return TOKEN_OPERATOR;
    if (isalpha(str[0]) || str[0] == '_') return TOKEN_IDENTIFIER;
    return TOKEN_UNKNOWN;
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    int token_count = 0;
    const char* p = input;
    
    while (*p && token_count < max_tokens) {
        while (isspace(*p)) p++;
        if (!*p) break;
        
        int len = 0;
        Token token;
        
        if (is_operator_char(*p)) {
            while (is_operator_char(*p) && len < MAX_TOKEN_LEN - 1) {
                token.value[len++] = *p++;
            }
            token.type = TOKEN_OPERATOR;
        } else if (isdigit(*p)) {
            while (isdigit(*p) && len < MAX_TOKEN_LEN - 1) {
                token.value[len++] = *p++;
            }
            token.type = TOKEN_NUMBER;
        } else if (isalpha(*p) || *p == '_') {
            while ((isalnum(*p) || *p == '_') && len < MAX_TOKEN_LEN - 1) {
                token.value[len++] = *p++;
            }
            token.type = TOKEN_IDENTIFIER;
        } else if (*p == '"') {
            token.value[len++] = *p++;
            while (*p != '"' && *p != '\0' && len < MAX_TOKEN_LEN - 1) {
                token.value[len++] = *p++;
            }
            if (*p == '"') {
                token.value[len++] = *p++;
            }
            token.type = TOKEN_STRING;
        } else {
            token.value[len++] = *p++;
            token.type = TOKEN_UNKNOWN;
        }
        
        token.value[len] = '\0';
        tokens[token_count++] = token;
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "EOF");
        token_count++;
    }
    
    return token_count;
}

void print_tokens(const Token* tokens, int count) {
    const char* type_names[] = {
        "IDENTIFIER", "NUMBER", "OPERATOR", "STRING", "EOF", "UNKNOWN"
    };
    
    for (int i = 0; i < count; i++) {
        printf("Token %d: %s -> '%s'\n", i, type_names[tokens[i].type], tokens[i].value);
    }
}

int main(void) {
    char input[4096];
    Token tokens[MAX_TOKENS];
    
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
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count >= MAX_TOKENS) {
        fprintf(stderr, "Too many tokens\n");
        return 1;
    }
    
    printf("\nTokenized output:\n");
    print_tokens(tokens, token_count);
    
    return 0;
}