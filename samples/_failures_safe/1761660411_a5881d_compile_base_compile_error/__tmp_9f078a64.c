//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
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
    TOKEN_UNKNOWN
};

struct Token {
    enum TokenType type;
    char value[64];
    int line;
    int column;
};

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
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
        token->value[0] = '\0';
        token->line = line;
        token->column = *col;
        return 1;
    }
    
    start_pos = *pos;
    start_col = *col;
    
    if (isdigit(input[*pos])) {
        token->type = TOKEN_NUMBER;
        int i = 0;
        while (input[*pos] && (isdigit(input[*pos]) || input[*pos] == '.')) {
            if (i < 63) {
                token->value[i++] = input[*pos];
            }
            (*pos)++;
            (*col)++;
        }
        token->value[i] = '\0';
        token->line = line;
        token->column = start_col;
        return 1;
    }
    
    if (isalpha(input[*pos]) || input[*pos] == '_') {
        token->type = TOKEN_IDENTIFIER;
        int i = 0;
        while (input[*pos] && (isalnum(input[*pos]) || input[*pos] == '_')) {
            if (i < 63) {
                token->value[i++] = input[*pos];
            }
            (*pos)++;
            (*col)++;
        }
        token->value[i] = '\0';
        token->line = line;
        token->column = start_col;
        return 1;
    }
    
    if (input[*pos] == '"') {
        token->type = TOKEN_STRING;
        int i = 0;
        (*pos)++;
        (*col)++;
        while (input[*pos] && input[*pos] != '"') {
            if (i < 63) {
                token->value[i++] = input[*pos];
            }
            (*pos)++;
            (*col)++;
            if (input[*pos] == '\n') {
                break;
            }
        }
        if (input[*pos] == '"') {
            (*pos)++;
            (*col)++;
        }
        token->value[i] = '\0';
        token->line = line;
        token->column = start_col;
        return 1;
    }
    
    if (is_operator_char(input[*pos])) {
        token->type = TOKEN_OPERATOR;
        int i = 0;
        while (input[*pos] && is_operator_char(input[*pos]) && i < 63) {
            token->value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token->value[i] = '\0';
        token->line = line;
        token->column = start_col;
        return 1;
    }
    
    token->type = TOKEN_UNKNOWN;
    token->value[0] = input[*pos];
    token->value[1] = '\0';
    (*pos)++;
    (*col)++;
    token->line = line;
    token->column = start_col;
    return 1;
}

const char* token_type_name(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_UNKNOWN: return "UNKNOWN";
        default: return "INVALID";
    }
}

int main(void) {
    char input[1024];
    printf("Enter text to tokenize (max 1023 chars): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        printf("No input provided\n");
        return 1;
    }
    
    printf("\nTokens:\n");
    printf("%-12s %-15s %-6s %-6s\n", "TYPE", "VALUE", "LINE", "COL");
    printf("------------ --------------- ------ ------\n");
    
    int pos = 0;
    int line = 1;
    int col = 1;
    struct Token token;
    
    while (get_token(&token, input,