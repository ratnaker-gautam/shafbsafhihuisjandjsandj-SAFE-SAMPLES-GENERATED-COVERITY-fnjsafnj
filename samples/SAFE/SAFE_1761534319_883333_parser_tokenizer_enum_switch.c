//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_PAREN,
    TOKEN_EOF,
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char value[32];
};

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        while (isspace(input[pos])) {
            pos++;
        }
        
        if (input[pos] == '\0') {
            break;
        }
        
        if (isdigit(input[pos])) {
            int start = pos;
            while (isdigit(input[pos])) {
                pos++;
            }
            int len = pos - start;
            if (len >= 32) {
                tokens[token_count].type = TOKEN_INVALID;
                strncpy(tokens[token_count].value, "NUMBER_TOO_LONG", 31);
                tokens[token_count].value[31] = '\0';
            } else {
                tokens[token_count].type = TOKEN_NUMBER;
                strncpy(tokens[token_count].value, &input[start], len);
                tokens[token_count].value[len] = '\0';
            }
            token_count++;
        } else if (isalpha(input[pos])) {
            int start = pos;
            while (isalnum(input[pos])) {
                pos++;
            }
            int len = pos - start;
            if (len >= 32) {
                tokens[token_count].type = TOKEN_INVALID;
                strncpy(tokens[token_count].value, "IDENTIFIER_TOO_LONG", 31);
                tokens[token_count].value[31] = '\0';
            } else {
                tokens[token_count].type = TOKEN_IDENTIFIER;
                strncpy(tokens[token_count].value, &input[start], len);
                tokens[token_count].value[len] = '\0';
            }
            token_count++;
        } else if (is_operator(input[pos])) {
            tokens[token_count].type = TOKEN_OPERATOR;
            tokens[token_count].value[0] = input[pos];
            tokens[token_count].value[1] = '\0';
            token_count++;
            pos++;
        } else if (input[pos] == '(' || input[pos] == ')') {
            tokens[token_count].type = TOKEN_PAREN;
            tokens[token_count].value[0] = input[pos];
            tokens[token_count].value[1] = '\0';
            token_count++;
            pos++;
        } else {
            tokens[token_count].type = TOKEN_INVALID;
            tokens[token_count].value[0] = input[pos];
            tokens[token_count].value[1] = '\0';
            token_count++;
            pos++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        token_count++;
    }
    
    return token_count;
}

void print_token(struct Token token) {
    switch (token.type) {
        case TOKEN_NUMBER:
            printf("NUMBER: %s\n", token.value);
            break;
        case TOKEN_IDENTIFIER:
            printf("IDENTIFIER: %s\n", token.value);
            break;
        case TOKEN_OPERATOR:
            printf("OPERATOR: %s\n", token.value);
            break;
        case TOKEN_PAREN:
            printf("PAREN: %s\n", token.value);
            break;
        case TOKEN_EOF:
            printf("EOF\n");
            break;
        case TOKEN_INVALID:
            printf("INVALID: %s\n", token.value);
            break;
    }
}

int main(void) {
    char input[256];
    struct Token tokens[64];
    
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
    
    int token_count = tokenize(input, tokens, 64);
    
    if (token_count <= 0) {
        fprintf(stderr, "No tokens generated\n");
        return 1;
    }
    
    printf("Tokens:\n");
    for (int i = 0; i < token_count; i++) {
        printf("%d: ", i + 1);
        print_token(tokens[i]);
    }
    
    return 0;
}