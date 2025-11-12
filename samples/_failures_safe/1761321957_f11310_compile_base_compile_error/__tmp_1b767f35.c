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
    TOKEN_ERROR
};

struct Token {
    enum TokenType type;
    char value[64];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || 
           c == '=' || c == '<' || c == '>' || c == '!' || 
           c == '&' || c == '|' || c == '%' || c == '^';
}

int get_token(struct Token *token, const char *input, int *pos, int *line, int *col) {
    if (input == NULL || token == NULL || pos == NULL || line == NULL || col == NULL) {
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
    
    start_pos = *pos;
    start_line = *line;
    start_col = *col;
    
    if (input[*pos] == '\0') {
        token->type = TOKEN_EOF;
        strncpy(token->value, "", sizeof(token->value) - 1);
        token->value[sizeof(token->value) - 1] = '\0';
        token->line = start_line;
        token->column = start_col;
        return 0;
    }
    
    if (isdigit(input[*pos])) {
        int i = 0;
        while (input[*pos] != '\0' && (isdigit(input[*pos]) || input[*pos] == '.')) {
            if (i < (int)sizeof(token->value) - 1) {
                token->value[i++] = input[*pos];
            }
            (*pos)++;
            (*col)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_NUMBER;
        token->line = start_line;
        token->column = start_col;
        return 0;
    }
    
    if (isalpha(input[*pos]) || input[*pos] == '_') {
        int i = 0;
        while (input[*pos] != '\0' && (isalnum(input[*pos]) || input[*pos] == '_')) {
            if (i < (int)sizeof(token->value) - 1) {
                token->value[i++] = input[*pos];
            }
            (*pos)++;
            (*col)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_IDENTIFIER;
        token->line = start_line;
        token->column = start_col;
        return 0;
    }
    
    if (input[*pos] == '"') {
        int i = 0;
        (*pos)++;
        (*col)++;
        
        while (input[*pos] != '\0' && input[*pos] != '"') {
            if (i < (int)sizeof(token->value) - 1) {
                token->value[i++] = input[*pos];
            }
            (*pos)++;
            (*col)++;
            
            if (input[*pos - 1] == '\\' && input[*pos] != '\0') {
                if (i < (int)sizeof(token->value) - 1) {
                    token->value[i++] = input[*pos];
                }
                (*pos)++;
                (*col)++;
            }
        }
        
        if (input[*pos] == '"') {
            (*pos)++;
            (*col)++;
        }
        
        token->value[i] = '\0';
        token->type = TOKEN_STRING;
        token->line = start_line;
        token->column = start_col;
        return 0;
    }
    
    if (is_operator_char(input[*pos])) {
        int i = 0;
        while (input[*pos] != '\0' && is_operator_char(input[*pos]) && i < 2) {
            if (i < (int)sizeof(token->value) - 1) {
                token->value[i++] = input[*pos];
            }
            (*pos)++;
            (*col)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_OPERATOR;
        token->line = start_line;
        token->column = start_col;
        return 0;
    }
    
    token->type = TOKEN_ERROR;
    token->value[0] = input[*pos];
    token->value[1] = '\0';
    token->line = start_line;
    token->column = start_col;
    (*pos)++;
    (*col)++;
    return 0;
}

const char* token_type_to_string(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_ERROR: