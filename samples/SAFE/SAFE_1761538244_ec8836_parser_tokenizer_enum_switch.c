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
        while (*pos < len && isdigit(input[*pos]) && value_pos < 31) {
            token->value[value_pos++] = input[(*pos)++];
        }
        token->value[value_pos] = '\0';
    } else if (isalpha(current)) {
        token->type = TOKEN_IDENTIFIER;
        while (*pos < len && (isalnum(input[*pos]) || input[*pos] == '_') && value_pos < 31) {
            token->value[value_pos++] = input[(*pos)++];
        }
        token->value[value_pos] = '\0';
    } else if (is_operator_char(current)) {
        token->type = TOKEN_OPERATOR;
        token->value[0] = input[(*pos)++];
        token->value[1] = '\0';
    } else if (current == '(' || current == ')') {
        token->type = TOKEN_PAREN;
        token->value[0] = input[(*pos)++];
        token->value[1] = '\0';
    } else {
        token->type = TOKEN_INVALID;
        token->value[0] = input[(*pos)++];
        token->value[1] = '\0';
        return 0;
    }

    return 1;
}

void process_token(struct Token *token) {
    switch (token->type) {
        case TOKEN_NUMBER:
            printf("Number: %s\n", token->value);
            break;
        case TOKEN_IDENTIFIER:
            printf("Identifier: %s\n", token->value);
            break;
        case TOKEN_OPERATOR:
            printf("Operator: %s\n", token->value);
            break;
        case TOKEN_PAREN:
            printf("Parenthesis: %s\n", token->value);
            break;
        case TOKEN_END:
            printf("End of input\n");
            break;
        case TOKEN_INVALID:
            printf("Invalid token: %s\n", token->value);
            break;
    }
}

int main(void) {
    char input[256];
    printf("Enter an expression to tokenize: ");
    
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
    int valid = 1;

    printf("Tokens:\n");
    while (1) {
        if (!get_next_token(input, &pos, &token)) {
            valid = 0;
        }
        
        process_token(&token);
        
        if (token.type == TOKEN_END) {
            break;
        }
    }

    if (!valid) {
        fprintf(stderr, "Input contained invalid tokens\n");
        return 1;
    }

    return 0;
}