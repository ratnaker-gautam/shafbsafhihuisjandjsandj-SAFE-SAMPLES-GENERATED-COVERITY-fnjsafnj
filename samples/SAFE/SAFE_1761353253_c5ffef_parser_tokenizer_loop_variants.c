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
    while (isdigit(input[*pos])) (*pos)++;
    if (*pos - start > 31) return 0;
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_identifier(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    while (isalnum(input[*pos]) || input[*pos] == '_') (*pos)++;
    if (*pos - start > 31) return 0;
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int get_next_token(const char *input, int *pos, struct Token *token) {
    int i = *pos;
    while (input[i] && isspace(input[i])) i++;
    if (!input[i]) {
        token->type = TOKEN_END;
        return 1;
    }
    if (isdigit(input[i])) {
        *pos = i;
        return parse_number(input, pos, token);
    }
    if (is_operator(input[i])) {
        token->value[0] = input[i];
        token->value[1] = '\0';
        token->type = TOKEN_OPERATOR;
        *pos = i + 1;
        return 1;
    }
    if (isalpha(input[i]) || input[i] == '_') {
        *pos = i;
        return parse_identifier(input, pos, token);
    }
    return 0;
}

void print_token(struct Token *token) {
    const char *type_names[] = {"NUMBER", "OPERATOR", "IDENTIFIER", "END"};
    printf("Token: %-12s Type: %s\n", token->value, type_names[token->type]);
}

int main(void) {
    char input[1024];
    printf("Enter expression: ");
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    input[strcspn(input, "\n")] = '\0';
    
    int pos = 0;
    struct Token token;
    int token_count = 0;
    
    do {
        if (!get_next_token(input, &pos, &token)) {
            fprintf(stderr, "Invalid token at position %d\n", pos);
            return 1;
        }
        print_token(&token);
        token_count++;
    } while (token.type != TOKEN_END);
    
    printf("Total tokens: %d\n", token_count);
    return 0;
}