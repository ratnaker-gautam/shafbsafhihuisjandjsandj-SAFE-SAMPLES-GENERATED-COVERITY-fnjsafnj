//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int parse_operator(const char *input, char *output) {
    const char *p = input;
    char *out = output;
    
    if (*p == '\0') return 0;
    
    if ((*p == '&' && *(p + 1) == '&') ||
        (*p == '|' && *(p + 1) == '|') ||
        (*p == '=' && *(p + 1) == '=') ||
        (*p == '!' && *(p + 1) == '=') ||
        (*p == '<' && *(p + 1) == '=') ||
        (*p == '>' && *(p + 1) == '=')) {
        *out++ = *p++;
        *out++ = *p++;
    } else if (is_operator_char(*p)) {
        *out++ = *p++;
    } else {
        return 0;
    }
    
    *out = '\0';
    return (int)(p - input);
}

int parse_number(const char *input, char *output) {
    const char *p = input;
    char *out = output;
    int has_digit = 0;
    
    while (*p && (*p == '-' || *p == '+' || *p == '.' || isdigit(*p))) {
        if (isdigit(*p)) has_digit = 1;
        *out++ = *p++;
        if ((size_t)(out - output) >= MAX_TOKEN_LEN - 1) break;
    }
    
    if (!has_digit) return 0;
    
    *out = '\0';
    return (int)(p - input);
}

int parse_string(const char *input, char *output) {
    const char *p = input;
    char *out = output;
    
    if (*p != '"') return 0;
    
    *out++ = *p++;
    
    while (*p && *p != '"') {
        if (*p == '\\' && *(p + 1)) {
            *out++ = *p++;
            *out++ = *p++;
        } else {
            *out++ = *p++;
        }
        if ((size_t)(out - output) >= MAX_TOKEN_LEN - 1) break;
    }
    
    if (*p == '"') {
        *out++ = *p++;
    }
    
    *out = '\0';
    return (int)(p - input);
}

int parse_identifier(const char *input, char *output) {
    const char *p = input;
    char *out = output;
    
    if (!isalpha(*p) && *p != '_') return 0;
    
    *out++ = *p++;
    
    while (*p && (isalnum(*p) || *p == '_')) {
        *out++ = *p++;
        if ((size_t)(out - output) >= MAX_TOKEN_LEN - 1) break;
    }
    
    *out = '\0';
    return (int)(p - input);
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    const char *p = input;
    Token *token = tokens;
    int token_count = 0;
    
    while (*p && token_count < max_tokens) {
        while (*p && isspace(*p)) p++;
        
        if (*p == '\0') break;
        
        int consumed = 0;
        
        if ((consumed = parse_string(p, token->value)) > 0) {
            token->type = TOKEN_STRING;
        } else if ((consumed = parse_number(p, token->value)) > 0) {
            token->type = TOKEN_NUMBER;
        } else if ((consumed = parse_operator(p, token->value)) > 0) {
            token->type = TOKEN_OPERATOR;
        } else if ((consumed = parse_identifier(p, token->value)) > 0) {
            token->type = TOKEN_IDENTIFIER;
        } else {
            token->type = TOKEN_UNKNOWN;
            token->value[0] = *p;
            token->value[1] = '\0';
            consumed = 1;
        }
        
        p += consumed;
        token++;
        token_count++;
    }
    
    if (token_count < max_tokens) {
        token->type = TOKEN_EOF;
        strcpy(token->value, "EOF");
        token_count++;
    }
    
    return token_count;
}

void print_token_type(TokenType type) {
    const char *names[] = {
        "IDENTIFIER", "NUMBER", "OPERATOR", "STRING", "EOF", "UNKNOWN"
    };
    printf("%s", names[type]);
}

int main(void) {
    char input[1024