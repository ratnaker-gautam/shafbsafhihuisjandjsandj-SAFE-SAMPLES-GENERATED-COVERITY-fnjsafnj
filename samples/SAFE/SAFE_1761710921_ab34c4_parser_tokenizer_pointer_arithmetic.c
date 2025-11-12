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
    TOKEN_ERROR
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenType classify_token(const char *str) {
    if (isdigit(*str)) return TOKEN_NUMBER;
    if (*str == '"') return TOKEN_STRING;
    if (is_operator_char(*str)) return TOKEN_OPERATOR;
    if (isalpha(*str) || *str == '_') return TOKEN_IDENTIFIER;
    return TOKEN_ERROR;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    const char *p = input;
    int token_count = 0;
    
    while (*p && token_count < max_tokens) {
        while (isspace(*p)) p++;
        
        if (!*p) break;
        
        Token *current = tokens + token_count;
        TokenType type = classify_token(p);
        current->type = type;
        
        char *dest = current->value;
        char *end = dest + MAX_TOKEN_LEN - 1;
        
        if (type == TOKEN_NUMBER) {
            while (isdigit(*p) && dest < end) *dest++ = *p++;
        } else if (type == TOKEN_IDENTIFIER) {
            while ((isalnum(*p) || *p == '_') && dest < end) *dest++ = *p++;
        } else if (type == TOKEN_OPERATOR) {
            if (dest < end) *dest++ = *p++;
        } else if (type == TOKEN_STRING) {
            *dest++ = *p++;
            while (*p && *p != '"' && dest < end) *dest++ = *p++;
            if (*p == '"' && dest < end) *dest++ = *p++;
        } else {
            current->type = TOKEN_ERROR;
            if (dest < end) *dest++ = *p++;
        }
        
        *dest = '\0';
        token_count++;
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        token_count++;
    }
    
    return token_count;
}

void print_token_type(TokenType type) {
    const char *names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "ERROR"
    };
    if (type >= TOKEN_NUMBER && type <= TOKEN_ERROR) {
        printf("%s", names[type]);
    } else {
        printf("UNKNOWN");
    }
}

int main(void) {
    char input[1024];
    
    printf("Enter expression to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || len == 1) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];
    int count = tokenize(input, tokens, MAX_TOKENS);
    
    if (count <= 0) {
        fprintf(stderr, "No tokens found\n");
        return 1;
    }
    
    printf("Tokens found: %d\n", count);
    for (int i = 0; i < count; i++) {
        printf("Token %d: Type=", i + 1);
        print_token_type(tokens[i].type);
        printf(", Value='%s'\n", tokens[i].value);
    }
    
    return 0;
}