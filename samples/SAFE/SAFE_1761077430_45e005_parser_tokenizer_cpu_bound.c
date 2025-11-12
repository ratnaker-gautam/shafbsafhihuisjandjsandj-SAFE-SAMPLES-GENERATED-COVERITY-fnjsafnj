//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: parser_tokenizer
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
    TOKEN_PUNCTUATION,
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

int is_punctuation_char(char c) {
    return strchr("(),;{}[]", c) != NULL;
}

TokenType classify_token(const char* value) {
    if (isdigit(value[0])) return TOKEN_NUMBER;
    if (isalpha(value[0]) || value[0] == '_') return TOKEN_IDENTIFIER;
    if (value[0] == '"') return TOKEN_STRING;
    if (is_operator_char(value[0])) return TOKEN_OPERATOR;
    if (is_punctuation_char(value[0])) return TOKEN_PUNCTUATION;
    return TOKEN_UNKNOWN;
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    int token_count = 0;
    const char* p = input;
    
    while (*p && token_count < max_tokens) {
        while (isspace(*p)) p++;
        if (!*p) break;
        
        Token token = {TOKEN_UNKNOWN, ""};
        int len = 0;
        
        if (isalpha(*p) || *p == '_') {
            while (isalnum(*p) || *p == '_') {
                if (len < MAX_TOKEN_LEN - 1) token.value[len++] = *p;
                p++;
            }
        } else if (isdigit(*p)) {
            while (isdigit(*p)) {
                if (len < MAX_TOKEN_LEN - 1) token.value[len++] = *p;
                p++;
            }
        } else if (*p == '"') {
            token.value[len++] = *p++;
            while (*p && *p != '"') {
                if (len < MAX_TOKEN_LEN - 1) token.value[len++] = *p;
                p++;
            }
            if (*p == '"') {
                if (len < MAX_TOKEN_LEN - 1) token.value[len++] = *p;
                p++;
            }
        } else if (is_operator_char(*p)) {
            while (is_operator_char(*p)) {
                if (len < MAX_TOKEN_LEN - 1) token.value[len++] = *p;
                p++;
            }
        } else if (is_punctuation_char(*p)) {
            token.value[len++] = *p++;
        } else {
            token.value[len++] = *p++;
        }
        
        if (len > 0) {
            token.value[len] = '\0';
            token.type = classify_token(token.value);
            tokens[token_count++] = token;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "EOF");
        token_count++;
    }
    
    return token_count;
}

void print_token_type(TokenType type) {
    const char* names[] = {
        "IDENTIFIER", "NUMBER", "OPERATOR", "STRING", 
        "PUNCTUATION", "EOF", "UNKNOWN"
    };
    if (type >= TOKEN_IDENTIFIER && type <= TOKEN_UNKNOWN) {
        printf("%s", names[type]);
    } else {
        printf("UNKNOWN");
    }
}

int main(void) {
    char input[4096];
    printf("Enter code to tokenize (max 4095 chars): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return 0;
    }
    
    Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count >= MAX_TOKENS) {
        fprintf(stderr, "Too many tokens\n");
        return 1;
    }
    
    printf("Tokens found: %d\n", token_count);
    printf("Type\t\tValue\n");
    printf("----\t\t-----\n");
    
    for (int i = 0; i < token_count; i++) {
        print_token_type(tokens[i].type);
        printf("\t\t%s\n", tokens[i].value);
    }
    
    return 0;
}