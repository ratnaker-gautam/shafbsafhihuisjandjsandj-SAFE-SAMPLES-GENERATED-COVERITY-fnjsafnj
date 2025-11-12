//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_number(const char **input, struct Token *token) {
    const char *p = *input;
    char *dest = token->value;
    int len = 0;
    
    while (*p && (isdigit(*p) || *p == '.')) {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        *dest++ = *p++;
        len++;
    }
    *dest = '\0';
    token->type = TOKEN_NUMBER;
    *input = p;
    return 1;
}

int parse_identifier(const char **input, struct Token *token) {
    const char *p = *input;
    char *dest = token->value;
    int len = 0;
    
    if (!isalpha(*p) && *p != '_') return 0;
    
    while (*p && (isalnum(*p) || *p == '_')) {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        *dest++ = *p++;
        len++;
    }
    *dest = '\0';
    token->type = TOKEN_IDENTIFIER;
    *input = p;
    return 1;
}

int parse_operator(const char **input, struct Token *token) {
    const char *p = *input;
    char *dest = token->value;
    int len = 0;
    
    if (!is_operator_char(*p)) return 0;
    
    *dest++ = *p++;
    len++;
    
    if (*p && is_operator_char(*p) && len < MAX_TOKEN_LEN - 1) {
        *dest++ = *p++;
        len++;
    }
    *dest = '\0';
    token->type = TOKEN_OPERATOR;
    *input = p;
    return 1;
}

int parse_string(const char **input, struct Token *token) {
    const char *p = *input;
    char *dest = token->value;
    int len = 0;
    
    if (*p != '"') return 0;
    p++;
    
    while (*p && *p != '"') {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        *dest++ = *p++;
        len++;
    }
    
    if (*p != '"') return 0;
    p++;
    *dest = '\0';
    token->type = TOKEN_STRING;
    *input = p;
    return 1;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    const char *p = input;
    int token_count = 0;
    
    while (*p && token_count < max_tokens) {
        while (*p && isspace(*p)) p++;
        if (!*p) break;
        
        if (parse_number(&p, &tokens[token_count]) ||
            parse_identifier(&p, &tokens[token_count]) ||
            parse_operator(&p, &tokens[token_count]) ||
            parse_string(&p, &tokens[token_count])) {
            token_count++;
        } else {
            return -1;
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
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_EOF: printf("EOF"); break;
    }
}

int main(void) {
    char input[1024];
    struct Token tokens[MAX_TOKENS];
    
    printf("Enter expression to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
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
    if (token_count == -1) {
        fprintf(stderr, "Tokenization failed\n");
        return 1;
    }
    
    printf("Tokens found: %d\n", token_count);
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: Type=", i);
        print_token_type(tokens[i].type);
        printf(", Value='%