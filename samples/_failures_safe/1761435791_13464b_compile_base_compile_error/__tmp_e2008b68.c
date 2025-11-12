//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_IDENTIFIER, TOKEN_NUMBER, TOKEN_OPERATOR, TOKEN_STRING, TOKEN_EOF, TOKEN_UNKNOWN };

struct Token {
    enum TokenType type;
    char value[64];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int is_valid_identifier_start(char c) {
    return isalpha(c) || c == '_';
}

int is_valid_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

int parse_number(const char *input, int *pos, char *buffer, int buffer_size) {
    int i = 0;
    int has_dot = 0;
    
    while (input[*pos] != '\0' && (isdigit(input[*pos]) || input[*pos] == '.')) {
        if (input[*pos] == '.') {
            if (has_dot) break;
            has_dot = 1;
        }
        if (i < buffer_size - 1) {
            buffer[i++] = input[*pos];
        }
        (*pos)++;
    }
    buffer[i] = '\0';
    return i > 0;
}

int parse_string(const char *input, int *pos, char *buffer, int buffer_size) {
    if (input[*pos] != '"') return 0;
    
    int i = 0;
    (*pos)++;
    
    while (input[*pos] != '\0' && input[*pos] != '"') {
        if (input[*pos] == '\\') {
            (*pos)++;
            if (input[*pos] == '\0') break;
        }
        if (i < buffer_size - 1) {
            buffer[i++] = input[*pos];
        }
        (*pos)++;
    }
    
    if (input[*pos] == '"') {
        (*pos)++;
        buffer[i] = '\0';
        return 1;
    }
    return 0;
}

int parse_identifier(const char *input, int *pos, char *buffer, int buffer_size) {
    if (!is_valid_identifier_start(input[*pos])) return 0;
    
    int i = 0;
    while (input[*pos] != '\0' && is_valid_identifier_char(input[*pos])) {
        if (i < buffer_size - 1) {
            buffer[i++] = input[*pos];
        }
        (*pos)++;
    }
    buffer[i] = '\0';
    return i > 0;
}

int parse_operator(const char *input, int *pos, char *buffer, int buffer_size) {
    if (!is_operator_char(input[*pos])) return 0;
    
    int i = 0;
    while (input[*pos] != '\0' && is_operator_char(input[*pos]) && i < buffer_size - 1) {
        buffer[i++] = input[*pos];
        (*pos)++;
    }
    buffer[i] = '\0';
    return i > 0;
}

void skip_whitespace(const char *input, int *pos, int *line, int *column) {
    while (input[*pos] != '\0' && isspace(input[*pos])) {
        if (input[*pos] == '\n') {
            (*line)++;
            *column = 1;
        } else {
            (*column)++;
        }
        (*pos)++;
    }
}

int get_next_token(const char *input, int *pos, int *line, int *column, struct Token *token) {
    if (input == NULL || pos == NULL || line == NULL || column == NULL || token == NULL) return 0;
    
    skip_whitespace(input, pos, line, column);
    
    token->line = *line;
    token->column = *column;
    
    if (input[*pos] == '\0') {
        token->type = TOKEN_EOF;
        strcpy(token->value, "");
        return 1;
    }
    
    if (parse_number(input, pos, token->value, sizeof(token->value))) {
        token->type = TOKEN_NUMBER;
        *column += (int)strlen(token->value);
        return 1;
    }
    
    if (parse_string(input, pos, token->value, sizeof(token->value))) {
        token->type = TOKEN_STRING;
        *column += (int)strlen(token->value) + 2;
        return 1;
    }
    
    if (parse_identifier(input, pos, token->value, sizeof(token->value))) {
        token->type = TOKEN_IDENTIFIER;
        *column += (int)strlen(token->value);
        return 1;
    }
    
    if (parse_operator(input, pos, token->value, sizeof(token->value))) {
        token->type = TOKEN_OPERATOR;
        *column += (int)strlen(token->value);
        return 1;
    }
    
    token->type = TOKEN_UNKNOWN;
    if (sizeof(token->value) > 1) {
        token->value[0] = input[*pos];
        token->value[1] = '\0';
    } else {
        token->value[0] = '\0';
    }