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

int get_token(struct Token *token, const char *input, int *pos, int line, int column) {
    int start = *pos;
    int len = strlen(input);
    
    if (start >= len) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        token->line = line;
        token->column = column;
        return 1;
    }
    
    char c = input[start];
    
    if (isspace(c)) {
        while (start < len && isspace(input[start])) {
            if (input[start] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            start++;
        }
        *pos = start;
        return get_token(token, input, pos, line, column);
    }
    
    if (isdigit(c)) {
        int i = 0;
        while (start < len && (isdigit(input[start]) || input[start] == '.')) {
            if (i < 31) {
                token->value[i++] = input[start];
            }
            start++;
            column++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_NUMBER;
        token->line = line;
        token->column = column - i;
        *pos = start;
        return 1;
    }
    
    if (isalpha(c) || c == '_') {
        int i = 0;
        while (start < len && (isalnum(input[start]) || input[start] == '_')) {
            if (i < 31) {
                token->value[i++] = input[start];
            }
            start++;
            column++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_IDENTIFIER;
        token->line = line;
        token->column = column - i;
        *pos = start;
        return 1;
    }
    
    if (c == '"') {
        int i = 0;
        start++;
        column++;
        while (start < len && input[start] != '"') {
            if (i < 31) {
                token->value[i++] = input[start];
            }
            start++;
            column++;
        }
        if (start < len && input[start] == '"') {
            start++;
            column++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_STRING;
        token->line = line;
        token->column = column - i - 1;
        *pos = start;
        return 1;
    }
    
    if (is_operator_char(c)) {
        int i = 0;
        while (start < len && is_operator_char(input[start]) && i < 31) {
            token->value[i++] = input[start];
            start++;
            column++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_OPERATOR;
        token->line = line;
        token->column = column - i;
        *pos = start;
        return 1;
    }
    
    token->type = TOKEN_INVALID;
    token->value[0] = c;
    token->value[1] = '\0';
    token->line = line;
    token->column = column;
    (*pos)++;
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
            printf("INVALID: '%s'", token->value);
            break;
    }
    printf(" at line %d, column %d\n", token->line, token->column);
}

int main(void) {
    char input[1024];
    
    printf("Enter text to tokenize (max 1023 characters):\n");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    int pos = 0;
    int line = 1;
    int column = 1;