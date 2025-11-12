//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
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

int get_next_token(const char *input, int *pos, struct Token *token) {
    int len = strlen(input);
    if (input == NULL || pos == NULL || token == NULL || *pos < 0) {
        return -1;
    }
    
    while (*pos < len && isspace(input[*pos])) {
        (*pos)++;
    }
    
    if (*pos >= len) {
        token->type = TOKEN_END;
        token->value[0] = '\0';
        return 0;
    }
    
    char current = input[*pos];
    
    if (isdigit(current)) {
        token->type = TOKEN_NUMBER;
        int i = 0;
        while (*pos < len && i < 31 && isdigit(input[*pos])) {
            token->value[i++] = input[*pos];
            (*pos)++;
        }
        token->value[i] = '\0';
    } else if (isalpha(current)) {
        token->type = TOKEN_IDENTIFIER;
        int i = 0;
        while (*pos < len && i < 31 && (isalnum(input[*pos]) || input[*pos] == '_')) {
            token->value[i++] = input[*pos];
            (*pos)++;
        }
        token->value[i] = '\0';
    } else if (is_operator_char(current)) {
        token->type = TOKEN_OPERATOR;
        token->value[0] = current;
        token->value[1] = '\0';
        (*pos)++;
    } else if (current == '(' || current == ')') {
        token->type = TOKEN_PAREN;
        token->value[0] = current;
        token->value[1] = '\0';
        (*pos)++;
    } else {
        token->type = TOKEN_INVALID;
        token->value[0] = current;
        token->value[1] = '\0';
        (*pos)++;
    }
    
    return 0;
}

void print_token(struct Token *token) {
    if (token == NULL) {
        return;
    }
    
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
        case TOKEN_PAREN:
            printf("PAREN: %s\n", token->value);
            break;
        case TOKEN_END:
            printf("END\n");
            break;
        case TOKEN_INVALID:
            printf("INVALID: %s\n", token->value);
            break;
    }
}

int main(void) {
    char input[256];
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 1 && input[0] == '\n') {
        printf("Empty input\n");
        return 0;
    }
    
    int pos = 0;
    struct Token token;
    
    printf("Tokens:\n");
    while (1) {
        if (get_next_token(input, &pos, &token) != 0) {
            fprintf(stderr, "Error tokenizing\n");
            return 1;
        }
        
        print_token(&token);
        
        if (token.type == TOKEN_END || token.type == TOKEN_INVALID) {
            break;
        }
    }
    
    return 0;
}