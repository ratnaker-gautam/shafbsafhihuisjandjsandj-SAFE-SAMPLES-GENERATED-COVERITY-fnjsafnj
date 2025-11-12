//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char value[32];
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int get_token(struct Token *token, const char **input) {
    const char *p = *input;
    
    while (*p && isspace(*p)) {
        p++;
    }
    
    if (*p == '\0') {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        *input = p;
        return 1;
    }
    
    if (isdigit(*p)) {
        token->type = TOKEN_NUMBER;
        int i = 0;
        while (*p && (isdigit(*p) || *p == '.') && i < 31) {
            token->value[i++] = *p++;
        }
        token->value[i] = '\0';
        *input = p;
        return 1;
    }
    
    if (isalpha(*p) || *p == '_') {
        token->type = TOKEN_IDENTIFIER;
        int i = 0;
        while (*p && (isalnum(*p) || *p == '_') && i < 31) {
            token->value[i++] = *p++;
        }
        token->value[i] = '\0';
        *input = p;
        return 1;
    }
    
    if (*p == '"') {
        token->type = TOKEN_STRING;
        int i = 0;
        p++;
        while (*p && *p != '"' && i < 31) {
            token->value[i++] = *p++;
        }
        if (*p == '"') {
            p++;
        }
        token->value[i] = '\0';
        *input = p;
        return 1;
    }
    
    if (is_operator_char(*p)) {
        token->type = TOKEN_OPERATOR;
        int i = 0;
        while (*p && is_operator_char(*p) && i < 31) {
            token->value[i++] = *p++;
        }
        token->value[i] = '\0';
        *input = p;
        return 1;
    }
    
    token->type = TOKEN_INVALID;
    token->value[0] = *p;
    token->value[1] = '\0';
    p++;
    *input = p;
    return 1;
}

void print_token(const struct Token *token) {
    switch (token->type) {
        case TOKEN_NUMBER:
            printf("NUMBER: %s\n", token->value);
            break;
        case TOKEN_IDENTIFIER:
            printf("IDENTIFIER: %s\n", token->value);
            break;
        case TOKEN_OPERATOR:
            printf("OPERATOR: %s\n", token->value);
            break;
        case TOKEN_STRING:
            printf("STRING: \"%s\"\n", token->value);
            break;
        case TOKEN_EOF:
            printf("EOF\n");
            break;
        case TOKEN_INVALID:
            printf("INVALID: %s\n", token->value);
            break;
    }
}

int main(void) {
    char input[1024];
    
    printf("Enter expression to tokenize: ");
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
    
    const char *cursor = input;
    struct Token token;
    int token_count = 0;
    
    printf("\nTokens:\n");
    while (get_token(&token, &cursor)) {
        print_token(&token);
        token_count++;
        
        if (token.type == TOKEN_EOF) {
            break;
        }
        
        if (token_count > 100) {
            fprintf(stderr, "Too many tokens\n");
            return 1;
        }
    }
    
    printf("Total tokens: %d\n", token_count);
    return 0;
}