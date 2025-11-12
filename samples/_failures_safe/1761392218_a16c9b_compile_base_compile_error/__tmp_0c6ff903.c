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
    int line;
    int column;
} Token;

int is_operator_char(char c) {
    return strchr("+-*/%=&|<>!", c) != NULL;
}

int is_punctuation_char(char c) {
    return strchr("(){}[];,.:", c) != NULL;
}

TokenType classify_token(const char* value) {
    if (value[0] == '\0') return TOKEN_UNKNOWN;
    if (isdigit((unsigned char)value[0])) return TOKEN_NUMBER;
    if (isalpha((unsigned char)value[0]) || value[0] == '_') return TOKEN_IDENTIFIER;
    if (value[0] == '"') return TOKEN_STRING;
    if (is_operator_char(value[0])) return TOKEN_OPERATOR;
    if (is_punctuation_char(value[0])) return TOKEN_PUNCTUATION;
    return TOKEN_UNKNOWN;
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    if (!input || !tokens || max_tokens <= 0) return 0;
    
    int token_count = 0;
    int line = 1;
    int column = 1;
    const char* p = input;
    
    while (*p && token_count < max_tokens) {
        while (isspace((unsigned char)*p)) {
            if (*p == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            p++;
        }
        
        if (!*p) break;
        
        Token* token = &tokens[token_count];
        token->line = line;
        token->column = column;
        
        if (isalpha((unsigned char)*p) || *p == '_') {
            int len = 0;
            while ((isalnum((unsigned char)*p) || *p == '_') && len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = *p++;
                column++;
            }
            token->value[len] = '\0';
            token->type = TOKEN_IDENTIFIER;
        }
        else if (isdigit((unsigned char)*p)) {
            int len = 0;
            while (isdigit((unsigned char)*p) && len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = *p++;
                column++;
            }
            token->value[len] = '\0';
            token->type = TOKEN_NUMBER;
        }
        else if (*p == '"') {
            int len = 0;
            token->value[len++] = *p++;
            column++;
            while (*p && *p != '"' && len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = *p++;
                column++;
            }
            if (*p == '"') {
                token->value[len++] = *p++;
                column++;
            }
            token->value[len] = '\0';
            token->type = TOKEN_STRING;
        }
        else if (is_operator_char(*p)) {
            int len = 0;
            while (is_operator_char(*p) && len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = *p++;
                column++;
            }
            token->value[len] = '\0';
            token->type = TOKEN_OPERATOR;
        }
        else if (is_punctuation_char(*p)) {
            token->value[0] = *p++;
            token->value[1] = '\0';
            token->type = TOKEN_PUNCTUATION;
            column++;
        }
        else {
            token->value[0] = *p++;
            token->value[1] = '\0';
            token->type = TOKEN_UNKNOWN;
            column++;
        }
        
        token_count++;
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        tokens[token_count].line = line;
        tokens[token_count].column = column;
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
    printf("Enter text to tokenize (max 4095 chars): ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len -