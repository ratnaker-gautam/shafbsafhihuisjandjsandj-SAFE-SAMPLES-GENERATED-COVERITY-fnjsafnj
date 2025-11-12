//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { NUMBER, IDENTIFIER, OPERATOR, END };

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
    token->type = NUMBER;
    return 1;
}

int parse_identifier(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    if (!isalpha(input[*pos])) return 0;
    (*pos)++;
    while (isalnum(input[*pos])) (*pos)++;
    if (*pos - start > 31) return 0;
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = IDENTIFIER;
    return 1;
}

int parse_operator(const char *input, int *pos, struct Token *token) {
    if (!is_operator(input[*pos])) return 0;
    token->value[0] = input[*pos];
    token->value[1] = '\0';
    token->type = OPERATOR;
    (*pos)++;
    return 1;
}

int get_next_token(const char *input, int *pos, struct Token *token) {
    while (isspace(input[*pos])) (*pos)++;
    if (input[*pos] == '\0') {
        token->type = END;
        return 1;
    }
    if (parse_number(input, pos, token)) return 1;
    if (parse_identifier(input, pos, token)) return 1;
    if (parse_operator(input, pos, token)) return 1;
    return 0;
}

void print_token(struct Token *token) {
    const char *type_names[] = {"NUMBER", "IDENTIFIER", "OPERATOR", "END"};
    printf("Token: %s, Value: %s\n", type_names[token->type], token->value);
}

int main(void) {
    char input[256];
    printf("Enter expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    input[strcspn(input, "\n")] = '\0';
    
    int pos = 0;
    struct Token token;
    
    printf("Parsing tokens:\n");
    for (;;) {
        if (!get_next_token(input, &pos, &token)) {
            fprintf(stderr, "Invalid token at position %d\n", pos);
            return 1;
        }
        print_token(&token);
        if (token.type == END) break;
    }
    
    printf("Parsing complete.\n");
    return 0;
}