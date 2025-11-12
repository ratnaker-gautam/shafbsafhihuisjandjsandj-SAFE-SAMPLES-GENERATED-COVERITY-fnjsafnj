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

int get_next_token(const char *input, int *pos, struct Token *token) {
    int len = strlen(input);
    if (*pos >= len) {
        token->type = TOKEN_END;
        token->value[0] = '\0';
        return 1;
    }

    while (*pos < len && isspace(input[*pos])) {
        (*pos)++;
    }

    if (*pos >= len) {
        token->type = TOKEN_END;
        token->value[0] = '\0';
        return 1;
    }

    char current = input[*pos];
    int value_pos = 0;

    if (isdigit(current)) {
        token->type = TOKEN_NUMBER;
        while (*pos < len && isdigit(input[*pos])) {
            if (value_pos < 31) {
                token->value[value_pos++] = input[*pos];
            }
            (*pos)++;
        }
        token->value[value_pos] = '\0';
    } else if (isalpha(current)) {
        token->type = TOKEN_IDENTIFIER;
        while (*pos < len && (isalnum(input[*pos]) || input[*pos] == '_')) {
            if (value_pos < 31) {
                token->value[value_pos++] = input[*pos];
            }
            (*pos)++;
        }
        token->value[value_pos] = '\0';
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

    if (len == 1) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }

    int pos = 0;
    struct Token token;
    int error_count = 0;

    printf("Tokens:\n");
    while (1) {
        if (get_next_token(input, &pos, &token)) {
            break;
        }

        print_token(&token);
        
        if (token.type == TOKEN_INVALID) {
            error_count++;
        }

        if (token.type == TOKEN_END) {
            break;
        }
    }

    if (error_count > 0) {
        printf("Found %d invalid tokens\n", error_count);
        return 1;
    }

    return 0;
}