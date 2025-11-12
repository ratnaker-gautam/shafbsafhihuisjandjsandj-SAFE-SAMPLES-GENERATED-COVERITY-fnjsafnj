//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_NUMBER, TOKEN_STRING, TOKEN_IDENTIFIER, TOKEN_OPERATOR, TOKEN_EOF };

struct Token {
    enum TokenType type;
    char value[32];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_number(const char *input, int *pos, int len, struct Token *token) {
    if (*pos >= len || !isdigit(input[*pos])) return 0;
    int start = *pos;
    int has_dot = 0;
    while (*pos < len && (isdigit(input[*pos]) || input[*pos] == '.')) {
        if (input[*pos] == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        }
        (*pos)++;
    }
    if (*pos - start > 31) return 0;
    strncpy(token->value, input + start, *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_string(const char *input, int *pos, int len, struct Token *token) {
    if (*pos >= len || input[*pos] != '"') return 0;
    (*pos)++;
    int start = *pos;
    while (*pos < len && input[*pos] != '"') {
        if (input[*pos] == '\\' && *pos + 1 < len) (*pos)++;
        (*pos)++;
    }
    if (*pos >= len || input[*pos] != '"') return 0;
    int value_len = *pos - start;
    if (value_len > 31) return 0;
    strncpy(token->value, input + start, value_len);
    token->value[value_len] = '\0';
    (*pos)++;
    token->type = TOKEN_STRING;
    return 1;
}

int parse_identifier(const char *input, int *pos, int len, struct Token *token) {
    if (*pos >= len || (!isalpha(input[*pos]) && input[*pos] != '_')) return 0;
    int start = *pos;
    while (*pos < len && (isalnum(input[*pos]) || input[*pos] == '_')) {
        (*pos)++;
    }
    if (*pos - start > 31) return 0;
    strncpy(token->value, input + start, *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_operator(const char *input, int *pos, int len, struct Token *token) {
    if (*pos >= len || !is_operator_char(input[*pos])) return 0;
    int start = *pos;
    (*pos)++;
    if (*pos < len && is_operator_char(input[*pos]) && 
        ((input[start] == '<' && input[*pos] == '=') ||
         (input[start] == '>' && input[*pos] == '=') ||
         (input[start] == '!' && input[*pos] == '=') ||
         (input[start] == '=' && input[*pos] == '='))) {
        (*pos)++;
    }
    if (*pos - start > 31) return 0;
    strncpy(token->value, input + start, *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

void skip_whitespace(const char *input, int *pos, int len, int *line, int *col) {
    while (*pos < len && isspace(input[*pos])) {
        if (input[*pos] == '\n') {
            (*line)++;
            *col = 1;
        } else {
            (*col)++;
        }
        (*pos)++;
    }
}

int get_next_token(const char *input, int *pos, int len, struct Token *token, int *line, int *col) {
    skip_whitespace(input, pos, len, line, col);
    if (*pos >= len) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        token->line = *line;
        token->column = *col;
        return 1;
    }
    token->line = *line;
    token->column = *col;
    if (parse_number(input, pos, len, token)) {
        *col += strlen(token->value);
        return 1;
    }
    if (parse_string(input, pos, len, token)) {
        *col += strlen(token->value) + 2;
        return 1;
    }
    if (parse_operator(input, pos, len, token)) {
        *col += strlen(token->value);
        return 1;
    }
    if (parse_identifier(input, pos, len, token)) {
        *col += strlen(token->value);
        return 1;
    }
    return 0;
}

const char* token_type_name(enum TokenType type) {
    switch (type) {
        case TOKEN_N