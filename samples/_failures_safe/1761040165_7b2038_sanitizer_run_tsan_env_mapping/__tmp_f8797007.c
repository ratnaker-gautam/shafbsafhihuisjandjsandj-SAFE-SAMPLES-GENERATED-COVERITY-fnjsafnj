//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
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
    if (*pos >= len) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }

    while (*pos < len && isspace(input[*pos])) {
        (*pos)++;
    }

    if (*pos >= len) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }

    char current = input[*pos];
    int value_pos = 0;

    if (isdigit(current)) {
        token->type = TOKEN_NUMBER;
        while (*pos < len && isdigit(input[*pos]) && value_pos < 31) {
            token->value[value_pos++] = input[(*pos)++];
        }
        token->value[value_pos] = '\0';
    } else if (isalpha(current) || current == '_') {
        token->type = TOKEN_IDENTIFIER;
        while (*pos < len && (isalnum(input[*pos]) || input[*pos] == '_') && value_pos < 31) {
            token->value[value_pos++] = input[(*pos)++];
        }
        token->value[value_pos] = '\0';
    } else if (is_operator_char(current)) {
        token->type = TOKEN_OPERATOR;
        token->value[value_pos++] = input[(*pos)++];
        if (*pos < len && is_operator_char(input[*pos]) && value_pos < 31) {
            token->value[value_pos++] = input[(*pos)++];
        }
        token->value[value_pos] = '\0';
    } else if (current == '"') {
        token->type = TOKEN_STRING;
        (*pos)++;
        while (*pos < len && input[*pos] != '"' && value_pos < 31) {
            token->value[value_pos++] = input[(*pos)++];
        }
        token->value[value_pos] = '\0';
        if (*pos < len && input[*pos] == '"') {
            (*pos)++;
        } else {
            token->type = TOKEN_INVALID;
        }
    } else {
        token->type = TOKEN_INVALID;
        token->value[0] = input[(*pos)++];
        token->value[1] = '\0';
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
        case TOKEN_STRING:
            printf("STRING: %s\n", token->value);
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
            break;
        }
        print_token(&token);
        if (token.type == TOKEN_EOF) {
            break;
        }
    }

    return 0;
}