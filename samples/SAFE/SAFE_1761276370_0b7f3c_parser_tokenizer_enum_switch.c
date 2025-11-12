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

int get_next_token(const char *input, int *pos, struct Token *token) {
    int len = strlen(input);
    if (input == NULL || pos == NULL || token == NULL || *pos < 0) {
        return -1;
    }
    
    while (*pos < len && isspace(input[*pos])) {
        (*pos)++;
    }
    
    if (*pos >= len) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 0;
    }
    
    char current = input[*pos];
    
    if (isdigit(current)) {
        int i = 0;
        while (*pos < len && (isdigit(input[*pos]) || input[*pos] == '.')) {
            if (i < 31) {
                token->value[i++] = input[*pos];
            }
            (*pos)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_NUMBER;
    } else if (isalpha(current) || current == '_') {
        int i = 0;
        while (*pos < len && (isalnum(input[*pos]) || input[*pos] == '_')) {
            if (i < 31) {
                token->value[i++] = input[*pos];
            }
            (*pos)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_IDENTIFIER;
    } else if (current == '"') {
        int i = 0;
        (*pos)++;
        while (*pos < len && input[*pos] != '"') {
            if (i < 31) {
                token->value[i++] = input[*pos];
            }
            (*pos)++;
        }
        if (*pos < len && input[*pos] == '"') {
            (*pos)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_STRING;
    } else if (is_operator_char(current)) {
        int i = 0;
        while (*pos < len && is_operator_char(input[*pos]) && i < 31) {
            token->value[i++] = input[*pos];
            (*pos)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_OPERATOR;
    } else {
        token->type = TOKEN_INVALID;
        token->value[0] = input[*pos];
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
    char input[256];
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
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
        
        if (token.type == TOKEN_EOF || token.type == TOKEN_INVALID) {
            break;
        }
    }
    
    return 0;
}