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

int get_token(struct Token *token, const char *input, int *pos, int line) {
    if (input[*pos] == '\0') {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        token->line = line;
        return 1;
    }
    
    while (isspace(input[*pos])) {
        if (input[*pos] == '\n') line++;
        (*pos)++;
        if (input[*pos] == '\0') {
            token->type = TOKEN_EOF;
            token->value[0] = '\0';
            token->line = line;
            return 1;
        }
    }
    
    token->line = line;
    
    if (isdigit(input[*pos])) {
        token->type = TOKEN_NUMBER;
        int i = 0;
        while (isdigit(input[*pos]) && i < 31) {
            token->value[i++] = input[*pos];
            (*pos)++;
        }
        token->value[i] = '\0';
        return 1;
    }
    
    if (isalpha(input[*pos]) || input[*pos] == '_') {
        token->type = TOKEN_IDENTIFIER;
        int i = 0;
        while ((isalnum(input[*pos]) || input[*pos] == '_') && i < 31) {
            token->value[i++] = input[*pos];
            (*pos)++;
        }
        token->value[i] = '\0';
        return 1;
    }
    
    if (input[*pos] == '"') {
        token->type = TOKEN_STRING;
        int i = 0;
        (*pos)++;
        while (input[*pos] != '"' && input[*pos] != '\0' && i < 31) {
            token->value[i++] = input[*pos];
            (*pos)++;
        }
        if (input[*pos] == '"') (*pos)++;
        token->value[i] = '\0';
        return 1;
    }
    
    if (is_operator_char(input[*pos])) {
        token->type = TOKEN_OPERATOR;
        int i = 0;
        while (is_operator_char(input[*pos]) && i < 31) {
            token->value[i++] = input[*pos];
            (*pos)++;
        }
        token->value[i] = '\0';
        return 1;
    }
    
    token->type = TOKEN_UNKNOWN;
    token->value[0] = input[*pos];
    token->value[1] = '\0';
    (*pos)++;
    return 1;
}

void print_token(const struct Token *token) {
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
    printf("Enter text to tokenize (max 1023 characters):\n");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        printf("No input provided\n");
        return 1;
    }
    
    printf("\nTokens:\n");
    int pos = 0;
    int line = 1;
    
    while (1) {
        struct Token token;
        if (!get_token(&token, input, &pos, line)) {
            fprintf(stderr, "Error getting token\n");
            return 1;
        }
        
        print_token(&token);
        line = token.line;
        
        if (token.type == TOKEN_EOF) {
            break;
        }
    }
    
    return 0;
}