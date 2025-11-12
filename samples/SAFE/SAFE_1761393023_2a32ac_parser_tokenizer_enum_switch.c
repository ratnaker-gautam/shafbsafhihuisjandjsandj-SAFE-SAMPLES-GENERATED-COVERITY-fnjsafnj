//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
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
    
    if (isalpha(current) || current == '_') {
        int i = 0;
        while (*pos < len && (isalnum(input[*pos]) || input[*pos] == '_') && i < 31) {
            token->value[i++] = input[*pos];
            (*pos)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_IDENTIFIER;
    } else if (isdigit(current)) {
        int i = 0;
        while (*pos < len && isdigit(input[*pos]) && i < 31) {
            token->value[i++] = input[*pos];
            (*pos)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_NUMBER;
    } else if (current == '"') {
        int i = 0;
        (*pos)++;
        while (*pos < len && input[*pos] != '"' && i < 31) {
            token->value[i++] = input[*pos];
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
        token->value[0] = current;
        token->value[1] = '\0';
        token->type = TOKEN_INVALID;
        (*pos)++;
    }
    
    return 0;
}

void print_token_info(struct Token *token) {
    if (token == NULL) {
        return;
    }
    
    switch (token->type) {
        case TOKEN_IDENTIFIER:
            printf("IDENTIFIER: %s\n", token->value);
            break;
        case TOKEN_NUMBER:
            printf("NUMBER: %s\n", token->value);
            break;
        case TOKEN_OPERATOR:
            printf("OPERATOR: %s\n", token->value);
            break;
        case TOKEN_STRING:
            printf("STRING: \"%s\"\n", token->value);
            break;
        case TOKEN_EOF:
            printf("END OF INPUT\n");
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
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    int pos = 0;
    struct Token token;
    
    printf("Tokens:\n");
    while (1) {
        if (get_next_token(input, &pos, &token) != 0) {
            fprintf(stderr, "Error tokenizing input\n");
            return 1;
        }
        
        print_token_info(&token);
        
        if (token.type == TOKEN_EOF) {
            break;
        }
    }
    
    return 0;
}