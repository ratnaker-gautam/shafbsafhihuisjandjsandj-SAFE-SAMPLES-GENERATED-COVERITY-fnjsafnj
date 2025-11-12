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

int get_next_token(const char *input, int *pos, int *line, int *column, struct Token *token) {
    if (input == NULL || pos == NULL || line == NULL || column == NULL || token == NULL) {
        return -1;
    }
    
    int start_pos = *pos;
    int start_line = *line;
    int start_column = *column;
    
    while (input[*pos] != '\0' && isspace(input[*pos])) {
        if (input[*pos] == '\n') {
            (*line)++;
            *column = 1;
        } else {
            (*column)++;
        }
        (*pos)++;
    }
    
    if (input[*pos] == '\0') {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        token->line = *line;
        token->column = *column;
        return 0;
    }
    
    start_pos = *pos;
    start_line = *line;
    start_column = *column;
    
    if (isdigit(input[*pos])) {
        int i = 0;
        while (input[*pos] != '\0' && (isdigit(input[*pos]) || input[*pos] == '.')) {
            if (i < 31) {
                token->value[i++] = input[*pos];
            }
            (*pos)++;
            (*column)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_NUMBER;
        token->line = start_line;
        token->column = start_column;
        return 0;
    }
    
    if (isalpha(input[*pos]) || input[*pos] == '_') {
        int i = 0;
        while (input[*pos] != '\0' && (isalnum(input[*pos]) || input[*pos] == '_')) {
            if (i < 31) {
                token->value[i++] = input[*pos];
            }
            (*pos)++;
            (*column)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_IDENTIFIER;
        token->line = start_line;
        token->column = start_column;
        return 0;
    }
    
    if (input[*pos] == '"') {
        int i = 0;
        (*pos)++;
        (*column)++;
        while (input[*pos] != '\0' && input[*pos] != '"') {
            if (i < 31) {
                token->value[i++] = input[*pos];
            }
            (*pos)++;
            (*column)++;
        }
        if (input[*pos] == '"') {
            (*pos)++;
            (*column)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_STRING;
        token->line = start_line;
        token->column = start_column;
        return 0;
    }
    
    if (is_operator_char(input[*pos])) {
        int i = 0;
        while (input[*pos] != '\0' && is_operator_char(input[*pos]) && i < 31) {
            token->value[i++] = input[*pos];
            (*pos)++;
            (*column)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_OPERATOR;
        token->line = start_line;
        token->column = start_column;
        return 0;
    }
    
    token->type = TOKEN_INVALID;
    token->value[0] = input[*pos];
    token->value[1] = '\0';
    token->line = start_line;
    token->column = start_column;
    (*pos)++;
    (*column)++;
    return 0;
}

void print_token(const struct Token *token) {
    if (token == NULL) return;
    
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
            printf("INVALID: '%s'", token->value);
            break;
    }
    printf(" (line %d, column %d)\n", token->line, token->column);
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return