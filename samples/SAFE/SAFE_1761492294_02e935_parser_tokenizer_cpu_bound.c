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
    return strchr("(){}[],;:", c) != NULL;
}

TokenType classify_token(const char* str) {
    if (isdigit(str[0])) return TOKEN_NUMBER;
    if (str[0] == '"') return TOKEN_STRING;
    if (is_operator_char(str[0])) return TOKEN_OPERATOR;
    if (is_punctuation_char(str[0])) return TOKEN_PUNCTUATION;
    if (isalpha(str[0]) || str[0] == '_') return TOKEN_IDENTIFIER;
    return TOKEN_UNKNOWN;
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    int token_count = 0;
    const char* p = input;
    
    while (*p && token_count < max_tokens) {
        while (isspace(*p)) p++;
        if (!*p) break;
        
        Token* token = &tokens[token_count];
        int len = 0;
        
        if (is_operator_char(*p)) {
            while (is_operator_char(*p) && len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = *p++;
            }
            token->type = TOKEN_OPERATOR;
        } else if (is_punctuation_char(*p)) {
            token->value[len++] = *p++;
            token->type = TOKEN_PUNCTUATION;
        } else if (*p == '"') {
            token->value[len++] = *p++;
            while (*p && *p != '"' && len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = *p++;
            }
            if (*p == '"') {
                token->value[len++] = *p++;
            }
            token->type = TOKEN_STRING;
        } else if (isdigit(*p)) {
            while (isdigit(*p) && len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = *p++;
            }
            if (*p == '.' && len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = *p++;
                while (isdigit(*p) && len < MAX_TOKEN_LEN - 1) {
                    token->value[len++] = *p++;
                }
            }
            token->type = TOKEN_NUMBER;
        } else if (isalpha(*p) || *p == '_') {
            while ((isalnum(*p) || *p == '_') && len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = *p++;
            }
            token->type = TOKEN_IDENTIFIER;
        } else {
            token->value[len++] = *p++;
            token->type = TOKEN_UNKNOWN;
        }
        
        token->value[len] = '\0';
        token_count++;
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "EOF");
        token_count++;
    }
    
    return token_count;
}

const char* token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_PUNCTUATION: return "PUNCTUATION";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input[4096];
    printf("Enter code to tokenize: ");
    
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
    
    if (token_count >= MAX_TOKENS) {
        fprintf(stderr, "Too many tokens\n");
        return 1;
    }
    
    printf("Tokens found: %d\n", token_count);
    printf("Type\t\tValue\n");
    printf("----\t\t-----\n");
    
    for (int i = 0; i < token_count; i++) {
        printf("%-12s\t%s\n", token_type_name(tokens[i].type), tokens[i].value);
    }
    
    return 0;
}