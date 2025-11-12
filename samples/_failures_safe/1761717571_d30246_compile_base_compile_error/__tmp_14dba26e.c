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
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int get_token(struct Token *token, const char *input, int *pos, int *line, int *col) {
    if (input[*pos] == '\0') {
        token->type = TOKEN_EOF;
        strcpy(token->value, "");
        token->line = *line;
        token->column = *col;
        return 1;
    }

    while (isspace((unsigned char)input[*pos])) {
        if (input[*pos] == '\n') {
            (*line)++;
            *col = 1;
        } else {
            (*col)++;
        }
        (*pos)++;
        if (input[*pos] == '\0') {
            token->type = TOKEN_EOF;
            strcpy(token->value, "");
            token->line = *line;
            token->column = *col;
            return 1;
        }
    }

    token->line = *line;
    token->column = *col;

    if (isdigit((unsigned char)input[*pos])) {
        token->type = TOKEN_NUMBER;
        int i = 0;
        while (isdigit((unsigned char)input[*pos]) && i < 31) {
            token->value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token->value[i] = '\0';
        return 1;
    }

    if (isalpha((unsigned char)input[*pos]) || input[*pos] == '_') {
        token->type = TOKEN_IDENTIFIER;
        int i = 0;
        while ((isalnum((unsigned char)input[*pos]) || input[*pos] == '_') && i < 31) {
            token->value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token->value[i] = '\0';
        return 1;
    }

    if (input[*pos] == '"') {
        token->type = TOKEN_STRING;
        int i = 0;
        (*pos)++;
        (*col)++;
        while (input[*pos] != '"' && input[*pos] != '\0' && i < 31) {
            token->value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        if (input[*pos] == '"') {
            (*pos)++;
            (*col)++;
        }
        token->value[i] = '\0';
        return 1;
    }

    if (is_operator_char(input[*pos])) {
        token->type = TOKEN_OPERATOR;
        int i = 0;
        while (is_operator_char(input[*pos]) && i < 31) {
            token->value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token->value[i] = '\0';
        return 1;
    }

    token->type = TOKEN_INVALID;
    token->value[0] = input[*pos];
    token->value[1] = '\0';
    (*pos)++;
    (*col)++;
    return 1;
}

void print_token(const struct Token *token) {
    switch (token->type) {
        case TOKEN_NUMBER:
            printf("NUMBER: %s", token->value);
            break;
        case TOKEN_IDENTIFIER:
            printf("IDENTIFIER: %s", token->value);
            break;
        case TOKEN_OPERATOR:
            printf("OPERATOR: %s", token->value);
            break;
        case TOKEN_STRING:
            printf("STRING: \"%s\"", token->value);
            break;
        case TOKEN_EOF:
            printf("EOF");
            break;
        case TOKEN_INVALID:
            printf("INVALID: %s", token->value);
            break;
    }
    printf(" (line %d, column %d)\n", token->line, token->column);
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }

    if (len == 1 && input[0] == '\n') {
        printf("Empty input\n");
        return 0;
    }

    int pos = 0;
    int line = 1;
    int col = 1;
    struct Token token;

    printf("Tokens:\n");
    while (1) {
        if (!get_token(&token, input, &pos, &line, &col)) {
            fprintf(stderr, "Error getting token\n");
            return 1;