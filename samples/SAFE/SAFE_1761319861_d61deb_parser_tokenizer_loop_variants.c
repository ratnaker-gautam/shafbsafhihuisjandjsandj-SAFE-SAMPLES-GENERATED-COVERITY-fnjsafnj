//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_NUMBER, TOKEN_OPERATOR, TOKEN_IDENTIFIER, TOKEN_END };

struct Token {
    enum TokenType type;
    char value[32];
};

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

int parse_number(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    while (isdigit(input[*pos])) {
        (*pos)++;
    }
    if (*pos - start > 31) return 0;
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_identifier(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    while (isalnum(input[*pos]) || input[*pos] == '_') {
        (*pos)++;
    }
    if (*pos - start > 31) return 0;
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int get_next_token(const char *input, int *pos, struct Token *token) {
    int len = strlen(input);
    while (*pos < len && isspace(input[*pos])) {
        (*pos)++;
    }
    if (*pos >= len) {
        token->type = TOKEN_END;
        return 1;
    }
    char c = input[*pos];
    if (isdigit(c)) {
        return parse_number(input, pos, token);
    } else if (is_operator(c)) {
        token->value[0] = c;
        token->value[1] = '\0';
        token->type = TOKEN_OPERATOR;
        (*pos)++;
        return 1;
    } else if (isalpha(c) || c == '_') {
        return parse_identifier(input, pos, token);
    }
    return 0;
}

void process_tokens(struct Token *tokens, int count) {
    int i = 0;
    while (i < count) {
        switch (tokens[i].type) {
            case TOKEN_NUMBER:
                printf("Number: %s\n", tokens[i].value);
                break;
            case TOKEN_OPERATOR:
                printf("Operator: %s\n", tokens[i].value);
                break;
            case TOKEN_IDENTIFIER:
                printf("Identifier: %s\n", tokens[i].value);
                break;
            default:
                break;
        }
        i++;
    }
}

int main(void) {
    char input[1024];
    printf("Enter expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    input[strcspn(input, "\n")] = '\0';
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    struct Token tokens[256];
    int pos = 0;
    int token_count = 0;
    int max_tokens = sizeof(tokens) / sizeof(tokens[0]);
    for (token_count = 0; token_count < max_tokens; token_count++) {
        if (!get_next_token(input, &pos, &tokens[token_count])) {
            fprintf(stderr, "Invalid token at position %d\n", pos);
            return 1;
        }
        if (tokens[token_count].type == TOKEN_END) {
            break;
        }
    }
    if (token_count >= max_tokens) {
        fprintf(stderr, "Too many tokens\n");
        return 1;
    }
    process_tokens(tokens, token_count);
    return 0;
}