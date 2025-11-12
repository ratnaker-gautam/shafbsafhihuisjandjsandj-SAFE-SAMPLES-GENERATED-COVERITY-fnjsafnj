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

int get_token(struct Token *token, const char *input, int *pos, int line, int *col) {
    int start_pos = *pos;
    int start_col = *col;
    
    while (input[*pos] && isspace(input[*pos])) {
        if (input[*pos] == '\n') {
            line++;
            *col = 1;
        } else {
            (*col)++;
        }
        (*pos)++;
    }
    
    if (!input[*pos]) {
        token->type = TOKEN_EOF;
        strcpy(token->value, "");
        token->line = line;
        token->column = *col;
        return 1;
    }
    
    start_pos = *pos;
    start_col = *col;
    
    if (isdigit(input[*pos])) {
        int len = 0;
        while (input[*pos] && isdigit(input[*pos]) && len < 31) {
            token->value[len++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token->value[len] = '\0';
        token->type = TOKEN_NUMBER;
        token->line = line;
        token->column = start_col;
        return 1;
    }
    
    if (isalpha(input[*pos]) || input[*pos] == '_') {
        int len = 0;
        while (input[*pos] && (isalnum(input[*pos]) || input[*pos] == '_') && len < 31) {
            token->value[len++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token->value[len] = '\0';
        token->type = TOKEN_IDENTIFIER;
        token->line = line;
        token->column = start_col;
        return 1;
    }
    
    if (input[*pos] == '"') {
        int len = 0;
        (*pos)++;
        (*col)++;
        while (input[*pos] && input[*pos] != '"' && len < 31) {
            token->value[len++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        if (input[*pos] == '"') {
            (*pos)++;
            (*col)++;
        } else {
            token->type = TOKEN_INVALID;
            strcpy(token->value, "Unterminated string");
            token->line = line;
            token->column = start_col;
            return 0;
        }
        token->value[len] = '\0';
        token->type = TOKEN_STRING;
        token->line = line;
        token->column = start_col;
        return 1;
    }
    
    if (is_operator_char(input[*pos])) {
        int len = 0;
        while (input[*pos] && is_operator_char(input[*pos]) && len < 31) {
            token->value[len++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token->value[len] = '\0';
        token->type = TOKEN_OPERATOR;
        token->line = line;
        token->column = start_col;
        return 1;
    }
    
    token->type = TOKEN_INVALID;
    token->value[0] = input[*pos];
    token->value[1] = '\0';
    token->line = line;
    token->column = *col;
    (*pos)++;
    (*col)++;
    return 0;
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
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int pos = 0;
    int line = 1;
    int col = 1;