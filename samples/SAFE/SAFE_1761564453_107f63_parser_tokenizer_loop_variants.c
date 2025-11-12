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

int parse_number(const char *input, int *pos, char *buffer) {
    int start = *pos;
    int i = 0;
    while (isdigit(input[*pos])) {
        if (i >= 31) return 0;
        buffer[i++] = input[(*pos)++];
    }
    buffer[i] = '\0';
    return i > 0;
}

int parse_identifier(const char *input, int *pos, char *buffer) {
    int start = *pos;
    int i = 0;
    if (isalpha(input[*pos]) || input[*pos] == '_') {
        buffer[i++] = input[(*pos)++];
        while (isalnum(input[*pos]) || input[*pos] == '_') {
            if (i >= 31) return 0;
            buffer[i++] = input[(*pos)++];
        }
        buffer[i] = '\0';
        return 1;
    }
    return 0;
}

int get_next_token(const char *input, int *pos, struct Token *token) {
    int len = strlen(input);
    while (*pos < len && isspace(input[*pos])) (*pos)++;
    if (*pos >= len) {
        token->type = TOKEN_END;
        return 1;
    }
    char current = input[*pos];
    if (isdigit(current)) {
        if (parse_number(input, pos, token->value)) {
            token->type = TOKEN_NUMBER;
            return 1;
        }
        return 0;
    }
    if (is_operator(current)) {
        token->value[0] = input[(*pos)++];
        token->value[1] = '\0';
        token->type = TOKEN_OPERATOR;
        return 1;
    }
    if (parse_identifier(input, pos, token->value)) {
        token->type = TOKEN_IDENTIFIER;
        return 1;
    }
    return 0;
}

void print_token(struct Token *token) {
    const char *type_names[] = {"NUMBER", "OPERATOR", "IDENTIFIER", "END"};
    printf("Token: %-12s Value: %s\n", type_names[token->type], token->value);
}

int main(void) {
    char input[1024];
    printf("Enter expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    input[strcspn(input, "\n")] = '\0';
    int pos = 0;
    struct Token token;
    int token_count = 0;
    for (;;) {
        if (!get_next_token(input, &pos, &token)) {
            fprintf(stderr, "Invalid token at position %d\n", pos);
            return 1;
        }
        print_token(&token);
        token_count++;
        if (token.type == TOKEN_END) break;
        if (token_count > 100) {
            fprintf(stderr, "Too many tokens\n");
            return 1;
        }
    }
    printf("Total tokens: %d\n", token_count);
    return 0;
}