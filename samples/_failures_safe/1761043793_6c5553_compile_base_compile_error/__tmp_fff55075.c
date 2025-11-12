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
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int get_token(struct Token *token, const char *input, int *pos, int *line, int *col) {
    if (input == NULL || pos == NULL || line == NULL || col == NULL) {
        return -1;
    }
    
    int start_pos = *pos;
    int start_line = *line;
    int start_col = *col;
    
    while (input[*pos] != '\0' && isspace(input[*pos])) {
        if (input[*pos] == '\n') {
            (*line)++;
            *col = 1;
        } else {
            (*col)++;
        }
        (*pos)++;
    }
    
    if (input[*pos] == '\0') {
        token->type = TOKEN_EOF;
        strcpy(token->value, "");
        token->line = *line;
        token->column = *col;
        return 0;
    }
    
    start_pos = *pos;
    start_line = *line;
    start_col = *col;
    
    if (isdigit(input[*pos])) {
        token->type = TOKEN_NUMBER;
        int i = 0;
        while (input[*pos] != '\0' && i < 31 && isdigit(input[*pos])) {
            token->value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token->value[i] = '\0';
    } else if (isalpha(input[*pos]) || input[*pos] == '_') {
        token->type = TOKEN_IDENTIFIER;
        int i = 0;
        while (input[*pos] != '\0' && i < 31 && 
               (isalnum(input[*pos]) || input[*pos] == '_')) {
            token->value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token->value[i] = '\0';
    } else if (input[*pos] == '"') {
        token->type = TOKEN_STRING;
        int i = 0;
        (*pos)++;
        (*col)++;
        while (input[*pos] != '\0' && i < 31 && input[*pos] != '"') {
            token->value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        if (input[*pos] == '"') {
            (*pos)++;
            (*col)++;
        }
        token->value[i] = '\0';
    } else if (is_operator_char(input[*pos])) {
        token->type = TOKEN_OPERATOR;
        int i = 0;
        while (input[*pos] != '\0' && i < 31 && is_operator_char(input[*pos])) {
            token->value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token->value[i] = '\0';
    } else {
        token->type = TOKEN_INVALID;
        token->value[0] = input[*pos];
        token->value[1] = '\0';
        (*pos)++;
        (*col)++;
    }
    
    token->line = start_line;
    token->column = start_col;
    return 0;
}

void print_token(const struct Token *token) {
    if (token == NULL) {
        return;
    }
    
    switch (token->type) {
        case TOKEN_NUMBER:
            printf("NUMBER: %s at line %d, column %d\n", token->value, token->line, token->column);
            break;
        case TOKEN_IDENTIFIER:
            printf("IDENTIFIER: %s at line %d, column %d\n", token->value, token->line, token->column);
            break;
        case TOKEN_OPERATOR:
            printf("OPERATOR: %s at line %d, column %d\n", token->value, token->line, token->column);
            break;
        case TOKEN_STRING:
            printf("STRING: \"%s\" at line %d, column %d\n", token->value, token->line, token->column);
            break;
        case TOKEN_EOF:
            printf("EOF at line %d, column %d\n", token->line, token->column);
            break;
        case TOKEN_INVALID:
            printf("INVALID: %s at line %d, column %d\n", token->value, token->line, token->column);
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
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {