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
    int len = *pos - start;
    if (len > 0 && len < 32) {
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
        token->type = TOKEN_NUMBER;
        return 1;
    }
    return 0;
}

int parse_identifier(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    while (isalpha(input[*pos]) || input[*pos] == '_') {
        (*pos)++;
    }
    int len = *pos - start;
    if (len > 0 && len < 32) {
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
        token->type = TOKEN_IDENTIFIER;
        return 1;
    }
    return 0;
}

int parse_operator(const char *input, int *pos, struct Token *token) {
    if (is_operator(input[*pos])) {
        token->value[0] = input[*pos];
        token->value[1] = '\0';
        token->type = TOKEN_OPERATOR;
        (*pos)++;
        return 1;
    }
    return 0;
}

int get_next_token(const char *input, int *pos, struct Token *token) {
    int i = *pos;
    while (input[i] && isspace(input[i])) {
        i++;
    }
    if (!input[i]) {
        token->type = TOKEN_END;
        return 1;
    }
    *pos = i;
    if (parse_number(input, pos, token)) return 1;
    if (parse_identifier(input, pos, token)) return 1;
    if (parse_operator(input, pos, token)) return 1;
    return 0;
}

void print_token(struct Token *token) {
    const char *type_names[] = {"NUMBER", "OPERATOR", "IDENTIFIER", "END"};
    printf("Token: %-12s Value: %s\n", type_names[token->type], token->value);
}

int main(void) {
    char input[256];
    printf("Enter expression: ");
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    input[strcspn(input, "\n")] = '\0';
    
    int pos = 0;
    struct Token token;
    
    printf("Parsing tokens:\n");
    do {
        if (!get_next_token(input, &pos, &token)) {
            fprintf(stderr, "Invalid token at position %d\n", pos);
            return 1;
        }
        print_token(&token);
    } while (token.type != TOKEN_END);
    
    int count = 0;
    pos = 0;
    printf("\nToken count: ");
    while (1) {
        if (!get_next_token(input, &pos, &token)) break;
        if (token.type == TOKEN_END) break;
        count++;
    }
    printf("%d\n", count);
    
    pos = 0;
    printf("\nToken values: ");
    for (;;) {
        if (!get_next_token(input, &pos, &token)) break;
        if (token.type == TOKEN_END) break;
        printf("[%s] ", token.value);
    }
    printf("\n");
    
    return 0;
}