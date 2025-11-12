//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_INVALID
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(const char **input) {
    while (**input && isspace((unsigned char)**input)) {
        (*input)++;
    }
}

TokenType classify_token(const char *value) {
    if (value[0] == '\0') return TOKEN_INVALID;
    
    if (isdigit((unsigned char)value[0])) {
        for (const char *p = value; *p; p++) {
            if (!isdigit((unsigned char)*p)) return TOKEN_INVALID;
        }
        return TOKEN_NUMBER;
    }
    
    if (is_operator_char(value[0])) {
        return TOKEN_OPERATOR;
    }
    
    if (isalpha((unsigned char)value[0]) || value[0] == '_') {
        for (const char *p = value; *p; p++) {
            if (!isalnum((unsigned char)*p) && *p != '_') return TOKEN_INVALID;
        }
        return TOKEN_IDENTIFIER;
    }
    
    return TOKEN_INVALID;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    int token_count = 0;
    const char *current = input;
    
    while (*current && token_count < max_tokens) {
        skip_whitespace(&current);
        if (!*current) break;
        
        Token *token = tokens + token_count;
        char *value_ptr = token->value;
        char *value_end = token->value + MAX_TOKEN_LEN - 1;
        
        if (isdigit((unsigned char)*current)) {
            while (*current && isdigit((unsigned char)*current) && value_ptr < value_end) {
                *value_ptr++ = *current++;
            }
        } else if (isalpha((unsigned char)*current) || *current == '_') {
            while (*current && (isalnum((unsigned char)*current) || *current == '_') && value_ptr < value_end) {
                *value_ptr++ = *current++;
            }
        } else if (is_operator_char(*current)) {
            while (*current && is_operator_char(*current) && value_ptr < value_end) {
                *value_ptr++ = *current++;
            }
        } else if (*current == '"') {
            *value_ptr++ = *current++;
            while (*current && *current != '"' && value_ptr < value_end) {
                *value_ptr++ = *current++;
            }
            if (*current == '"') {
                *value_ptr++ = *current++;
            }
        } else {
            token->type = TOKEN_INVALID;
            token->value[0] = *current++;
            token->value[1] = '\0';
            token_count++;
            continue;
        }
        
        *value_ptr = '\0';
        token->type = classify_token(token->value);
        token_count++;
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        token_count++;
    }
    
    return token_count;
}

const char *token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_INVALID: return "INVALID";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input_buffer[1024];
    
    printf("Enter expression to tokenize: ");
    if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input_buffer[0] == '\n')) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];
    int token_count = tokenize(input_buffer, tokens, MAX_TOKENS);
    
    if (token_count >= MAX_TOKENS) {
        fprintf(stderr, "Too many tokens\n");
        return 1;
    }
    
    printf("Tokens found:\n");
    for (int i = 0; i < token_count; i++) {
        printf("%d: %s -> '%s'\n", i, token_type_name(tokens[i].type), tokens[i].value);
    }
    
    return 0;
}