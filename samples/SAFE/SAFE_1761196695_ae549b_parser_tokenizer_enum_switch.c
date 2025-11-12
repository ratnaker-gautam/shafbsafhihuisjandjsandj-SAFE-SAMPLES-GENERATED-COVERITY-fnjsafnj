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
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int get_token(struct Token *token, const char *input, int *pos, int line, int column) {
    int start = *pos;
    int len = strlen(input);
    
    if (start >= len) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }
    
    char c = input[start];
    
    if (isspace(c)) {
        while (start < len && isspace(input[start])) {
            start++;
        }
        *pos = start;
        return get_token(token, input, pos, line, column + (start - *pos));
    }
    
    if (isdigit(c)) {
        int i = 0;
        while (start < len && i < 31 && (isdigit(input[start]) || input[start] == '.')) {
            token->value[i++] = input[start++];
        }
        token->value[i] = '\0';
        token->type = TOKEN_NUMBER;
        *pos = start;
        return 1;
    }
    
    if (isalpha(c) || c == '_') {
        int i = 0;
        while (start < len && i < 31 && (isalnum(input[start]) || input[start] == '_')) {
            token->value[i++] = input[start++];
        }
        token->value[i] = '\0';
        token->type = TOKEN_IDENTIFIER;
        *pos = start;
        return 1;
    }
    
    if (c == '"') {
        int i = 0;
        start++;
        while (start < len && i < 31 && input[start] != '"') {
            token->value[i++] = input[start++];
        }
        if (start >= len || input[start] != '"') {
            token->type = TOKEN_INVALID;
            token->value[0] = '\0';
            *pos = start;
            return 0;
        }
        token->value[i] = '\0';
        token->type = TOKEN_STRING;
        *pos = start + 1;
        return 1;
    }
    
    if (is_operator_char(c)) {
        int i = 0;
        while (start < len && i < 31 && is_operator_char(input[start])) {
            token->value[i++] = input[start++];
        }
        token->value[i] = '\0';
        token->type = TOKEN_OPERATOR;
        *pos = start;
        return 1;
    }
    
    token->type = TOKEN_INVALID;
    token->value[0] = c;
    token->value[1] = '\0';
    *pos = start + 1;
    return 0;
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
    
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int pos = 0;
    int line = 1;
    int column = 1;
    
    printf("Tokens:\n");
    
    while (1) {
        struct Token token;
        token.line = line;
        token.column = column;
        
        if (!get_token(&token, input, &pos, line, column)) {
            if (token.type == TOKEN_INVALID) {
                printf("Error: Invalid token at position %d\n", pos);
                break;
            }
        }
        
        print_token(&token);
        
        if (token.type == TOKEN_EOF) {
            break;
        }
        
        column += (int)strlen(token.value);
    }
    
    return 0;
}