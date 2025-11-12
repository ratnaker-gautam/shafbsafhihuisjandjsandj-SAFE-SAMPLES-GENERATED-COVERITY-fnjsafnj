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

int parse_number(const char *input, int *pos, int len, char *buffer, int buffer_size) {
    int i = 0;
    int has_dot = 0;
    
    while (*pos < len && i < buffer_size - 1) {
        char c = input[*pos];
        if (isdigit(c)) {
            buffer[i++] = c;
            (*pos)++;
        } else if (c == '.' && !has_dot) {
            buffer[i++] = c;
            (*pos)++;
            has_dot = 1;
        } else {
            break;
        }
    }
    
    buffer[i] = '\0';
    return i > 0;
}

int parse_string(const char *input, int *pos, int len, char *buffer, int buffer_size) {
    if (*pos >= len || input[*pos] != '"') return 0;
    
    int i = 0;
    (*pos)++;
    
    while (*pos < len && i < buffer_size - 1) {
        char c = input[*pos];
        if (c == '"') {
            (*pos)++;
            buffer[i] = '\0';
            return 1;
        } else if (c == '\\') {
            (*pos)++;
            if (*pos < len) {
                char escape_char = input[*pos];
                if (escape_char == 'n') buffer[i++] = '\n';
                else if (escape_char == 't') buffer[i++] = '\t';
                else if (escape_char == '"') buffer[i++] = '"';
                else if (escape_char == '\\') buffer[i++] = '\\';
                else {
                    buffer[i++] = '\\';
                    buffer[i++] = escape_char;
                }
                (*pos)++;
            } else {
                buffer[i++] = '\\';
            }
        } else {
            buffer[i++] = c;
            (*pos)++;
        }
    }
    
    buffer[i] = '\0';
    return 0;
}

int parse_identifier(const char *input, int *pos, int len, char *buffer, int buffer_size) {
    if (*pos >= len || !is_valid_identifier_start(input[*pos])) return 0;
    
    int i = 0;
    while (*pos < len && i < buffer_size - 1 && is_valid_identifier_char(input[*pos])) {
        buffer[i++] = input[*pos];
        (*pos)++;
    }
    
    buffer[i] = '\0';
    return i > 0;
}

int parse_operator(const char *input, int *pos, int len, char *buffer, int buffer_size) {
    if (*pos >= len || !is_operator_char(input[*pos])) return 0;
    
    int i = 0;
    char first_char = input[*pos];
    buffer[i++] = first_char;
    (*pos)++;
    
    if (*pos < len && i < buffer_size - 1) {
        char second_char = input[*pos];
        if ((first_char == '=' && second_char == '=') ||
            (first_char == '!' && second_char == '=') ||
            (first_char == '<' && second_char == '=') ||
            (first_char == '>' && second_char == '=') ||
            (first_char == '&' && second_char == '&') ||
            (first_char == '|' && second_char == '|')) {
            buffer[i++] = second_char;
            (*pos)++;
        }
    }
    
    buffer[i] = '\0';
    return i > 0;
}

void skip_whitespace_and_comments(const char *input, int *pos, int len, int *line, int *column) {
    while (*pos < len) {
        char c = input[*pos];
        if (isspace(c)) {
            if (c == '\n') {
                (*line)++;
                *column = 1;
            } else {
                (*column)++;
            }
            (*pos)++;
        } else if (c == '/' && *pos + 1 < len && input[*pos + 1] == '/') {
            while (*pos < len && input[*pos] != '\n') {
                (*pos)++;
            }
        } else if (c == '/' && *pos + 1 < len && input[*pos + 1] == '*') {
            (*pos) += 2;
            *column += 2;
            while (*pos < len - 1 && !(input[*pos] == '*' && input[*pos + 1] == '/')) {
                if (input[*pos] == '\n') {
                    (*line)++;
                    *column = 1;
                } else {
                    (*column)++;