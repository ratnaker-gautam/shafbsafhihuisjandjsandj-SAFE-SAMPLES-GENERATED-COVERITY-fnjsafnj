//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_NUMBER, TOKEN_STRING, TOKEN_IDENTIFIER, TOKEN_OPERATOR, TOKEN_EOF, TOKEN_INVALID };

struct Token {
    enum TokenType type;
    char value[64];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_number(const char *input, int *pos, int line, int col, struct Token *token) {
    int start = *pos;
    int has_dot = 0;
    
    while (input[*pos] != '\0') {
        if (isdigit(input[*pos])) {
            (*pos)++;
        } else if (input[*pos] == '.' && !has_dot) {
            has_dot = 1;
            (*pos)++;
        } else {
            break;
        }
    }
    
    if (*pos - start > 63) {
        return 0;
    }
    
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_NUMBER;
    token->line = line;
    token->column = col;
    return 1;
}

int parse_string(const char *input, int *pos, int line, int col, struct Token *token) {
    if (input[*pos] != '"') return 0;
    
    int start = *pos;
    (*pos)++;
    
    while (input[*pos] != '\0' && input[*pos] != '"') {
        if (input[*pos] == '\\' && input[*pos + 1] != '\0') {
            (*pos) += 2;
        } else {
            (*pos)++;
        }
    }
    
    if (input[*pos] != '"') {
        return 0;
    }
    
    (*pos)++;
    
    int len = *pos - start;
    if (len > 63) {
        return 0;
    }
    
    strncpy(token->value, &input[start], len);
    token->value[len] = '\0';
    token->type = TOKEN_STRING;
    token->line = line;
    token->column = col;
    return 1;
}

int parse_identifier(const char *input, int *pos, int line, int col, struct Token *token) {
    if (!isalpha(input[*pos]) && input[*pos] != '_') return 0;
    
    int start = *pos;
    (*pos)++;
    
    while (isalnum(input[*pos]) || input[*pos] == '_') {
        (*pos)++;
    }
    
    int len = *pos - start;
    if (len > 63) {
        return 0;
    }
    
    strncpy(token->value, &input[start], len);
    token->value[len] = '\0';
    token->type = TOKEN_IDENTIFIER;
    token->line = line;
    token->column = col;
    return 1;
}

int parse_operator(const char *input, int *pos, int line, int col, struct Token *token) {
    if (!is_operator_char(input[*pos])) return 0;
    
    int start = *pos;
    (*pos)++;
    
    if (is_operator_char(input[*pos])) {
        (*pos)++;
    }
    
    int len = *pos - start;
    if (len > 63) {
        return 0;
    }
    
    strncpy(token->value, &input[start], len);
    token->value[len] = '\0';
    token->type = TOKEN_OPERATOR;
    token->line = line;
    token->column = col;
    return 1;
}

void skip_whitespace(const char *input, int *pos, int *line, int *col) {
    while (input[*pos] != '\0') {
        if (isspace(input[*pos])) {
            if (input[*pos] == '\n') {
                (*line)++;
                *col = 1;
            } else {
                (*col)++;
            }
            (*pos)++;
        } else {
            break;
        }
    }
}

int get_next_token(const char *input, int *pos, int *line, int *col, struct Token *token) {
    skip_whitespace(input, pos, line, col);
    
    if (input[*pos] == '\0') {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        token->line = *line;
        token->column = *col;
        return 1;
    }
    
    int current_col = *col;
    
    if (parse_number(input, pos, *line, current_col, token)) return 1;
    if (parse_string(input, pos, *line, current_col, token)) return 1;
    if (parse_identifier(input, pos, *line, current_col, token)) return 1;
    if (parse_operator(input, pos, *line, current_col, token)) return 1;
    
    token->type = TOKEN_INVALID;
    token->value[0] = input[*pos];
    token->value[1] = '\0';
    token->line = *line;