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
    TOKEN_END,
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char value[32];
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int token_count = 0;
    const char *ptr = input;
    
    while (*ptr && token_count < max_tokens) {
        while (isspace(*ptr)) ptr++;
        
        if (!*ptr) break;
        
        if (isdigit(*ptr)) {
            int len = 0;
            while (isdigit(*ptr) && len < 31) {
                tokens[token_count].value[len++] = *ptr++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
        } else if (isalpha(*ptr)) {
            int len = 0;
            while ((isalnum(*ptr) || *ptr == '_') && len < 31) {
                tokens[token_count].value[len++] = *ptr++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            token_count++;
        } else if (is_operator_char(*ptr)) {
            tokens[token_count].value[0] = *ptr++;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
            token_count++;
        } else if (*ptr == '(' || *ptr == ')') {
            tokens[token_count].value[0] = *ptr++;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = TOKEN_PAREN;
            token_count++;
        } else {
            tokens[token_count].value[0] = *ptr++;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = TOKEN_INVALID;
            token_count++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_END;
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
        case TOKEN_END:
            printf("END\n");
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
    
    if (token_count >= 64) {
        fprintf(stderr, "Too many tokens\n");
        return 1;
    }
    
    printf("Tokens:\n");
    for (int i = 0; i < token_count; i++) {
        print_token(tokens[i]);
    }
    
    return 0;
}