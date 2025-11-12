//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 1000
#define MAX_TOKEN_LEN 256

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_KEYWORD,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

const char *keywords[] = {
    "if", "else", "while", "for", "return", "int", "float", "char", "void"
};
const int num_keywords = 9;

int is_keyword(const char *str) {
    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    int token_count = 0;
    const char *p = input;
    
    while (*p && token_count < max_tokens - 1) {
        while (isspace((unsigned char)*p)) p++;
        
        if (!*p) break;
        
        if (isdigit((unsigned char)*p)) {
            int i = 0;
            while (isdigit((unsigned char)*p) && i < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[i++] = *p++;
            }
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
        }
        else if (isalpha((unsigned char)*p) || *p == '_') {
            int i = 0;
            while ((isalnum((unsigned char)*p) || *p == '_') && i < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[i++] = *p++;
            }
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = is_keyword(tokens[token_count].value) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
            token_count++;
        }
        else if (*p == '"') {
            int i = 0;
            p++;
            while (*p && *p != '"' && i < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[i++] = *p++;
            }
            if (*p == '"') p++;
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = TOKEN_STRING;
            token_count++;
        }
        else if (is_operator_char(*p)) {
            int i = 0;
            while (is_operator_char(*p) && i < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[i++] = *p++;
            }
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
            token_count++;
        }
        else {
            p++;
        }
    }
    
    tokens[token_count].type = TOKEN_EOF;
    tokens[token_count].value[0] = '\0';
    
    return token_count;
}

void print_tokens(const Token *tokens, int count) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "KEYWORD", "EOF"
    };
    
    for (int i = 0; i < count; i++) {
        printf("Token %d: %s -> '%s'\n", i, type_names[tokens[i].type], tokens[i].value);
    }
}

int main() {
    char input[4096];
    
    printf("Enter code to tokenize (max 4095 chars):\n");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count >= MAX_TOKENS - 1) {
        fprintf(stderr, "Warning: Input too large, some tokens may be truncated\n");
    }
    
    printf("\nTokenized output:\n");
    print_tokens(tokens, token_count);
    
    return 0;
}