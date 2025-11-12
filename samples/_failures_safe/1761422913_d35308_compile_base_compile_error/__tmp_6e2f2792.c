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
    TOKEN_UNKNOWN
};

struct Token {
    enum TokenType type;
    char value[32];
    int line;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int get_next_token(const char *input, int *pos, int line, struct Token *token) {
    if (input == NULL || pos == NULL || token == NULL) return -1;
    
    int start = *pos;
    int len = strlen(input);
    
    while (start < len && isspace(input[start])) {
        if (input[start] == '\n') line++;
        start++;
    }
    
    if (start >= len) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        token->line = line;
        *pos = start;
        return 0;
    }
    
    char current = input[start];
    token->line = line;
    
    if (isdigit(current)) {
        int i = 0;
        while (start < len && (isdigit(input[start]) || input[start] == '.')) {
            if (i < 31) {
                token->value[i++] = input[start];
            }
            start++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_NUMBER;
        *pos = start;
        return 0;
    }
    
    if (isalpha(current) || current == '_') {
        int i = 0;
        while (start < len && (isalnum(input[start]) || input[start] == '_')) {
            if (i < 31) {
                token->value[i++] = input[start];
            }
            start++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_IDENTIFIER;
        *pos = start;
        return 0;
    }
    
    if (current == '"') {
        int i = 0;
        start++;
        while (start < len && input[start] != '"') {
            if (i < 31) {
                token->value[i++] = input[start];
            }
            start++;
        }
        if (start < len && input[start] == '"') {
            start++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_STRING;
        *pos = start;
        return 0;
    }
    
    if (is_operator_char(current)) {
        int i = 0;
        while (start < len && is_operator_char(input[start]) && i < 31) {
            token->value[i++] = input[start];
            start++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_OPERATOR;
        *pos = start;
        return 0;
    }
    
    token->value[0] = current;
    token->value[1] = '\0';
    token->type = TOKEN_UNKNOWN;
    *pos = start + 1;
    return 0;
}

void print_token(const struct Token *token) {
    if (token == NULL) return;
    
    switch (token->type) {
        case TOKEN_NUMBER:
            printf("NUMBER: %s (line %d)\n", token->value, token->line);
            break;
        case TOKEN_IDENTIFIER:
            printf("IDENTIFIER: %s (line %d)\n", token->value, token->line);
            break;
        case TOKEN_OPERATOR:
            printf("OPERATOR: %s (line %d)\n", token->value, token->line);
            break;
        case TOKEN_STRING:
            printf("STRING: \"%s\" (line %d)\n", token->value, token->line);
            break;
        case TOKEN_EOF:
            printf("EOF (line %d)\n", token->line);
            break;
        case TOKEN_UNKNOWN:
            printf("UNKNOWN: %s (line %d)\n", token->value, token->line);
            break;
    }
}

int main(void) {
    char input[1024];
    printf("Enter text to tokenize (max 1023 characters): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    int pos = 0;
    int line = 1;
    struct Token token;
    
    printf("\nTokens:\n");
    printf("--------\n");
    
    while (1) {
        if (get_next_token(input, &pos, line, &token) != 0) {
            fprintf(stderr, "Error tokenizing input\n");
            return 1;
        }
        
        print_token(&token);
        
        if (token.type == TOKEN_EOF) {
            break;
        }
        
        line = token.line;
    }
    
    return 0;