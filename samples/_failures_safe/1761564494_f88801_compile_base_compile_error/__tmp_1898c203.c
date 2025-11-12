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
    return strchr("+-*/=<>!&|", c) != NULL;
}

int parse_operator(const char *input, int pos, char *output) {
    int len = 0;
    if (input == NULL || output == NULL || pos < 0) return 0;
    
    char c = input[pos];
    if (c == '\0') return 0;
    
    if (is_operator_char(c)) {
        if (len < 63) output[len++] = c;
        char next = input[pos + 1];
        
        if ((c == '=' && next == '=') || (c == '!' && next == '=') ||
            (c == '<' && next == '=') || (c == '>' && next == '=') ||
            (c == '&' && next == '&') || (c == '|' && next == '|')) {
            if (len < 63) output[len++] = next;
        }
    }
    
    output[len] = '\0';
    return len > 0 ? len : 0;
}

int parse_number(const char *input, int pos, char *output) {
    int len = 0;
    int has_dot = 0;
    if (input == NULL || output == NULL || pos < 0) return 0;
    
    while (input[pos] != '\0' && len < 63) {
        char c = input[pos];
        if (isdigit(c)) {
            output[len++] = c;
            pos++;
        } else if (c == '.' && !has_dot) {
            output[len++] = c;
            has_dot = 1;
            pos++;
        } else {
            break;
        }
    }
    
    output[len] = '\0';
    return len > 0 ? len : 0;
}

int parse_identifier(const char *input, int pos, char *output) {
    int len = 0;
    if (input == NULL || output == NULL || pos < 0) return 0;
    
    if (input[pos] != '\0' && (isalpha(input[pos]) || input[pos] == '_')) {
        if (len < 63) output[len++] = input[pos];
        pos++;
        
        while (input[pos] != '\0' && (isalnum(input[pos]) || input[pos] == '_') && len < 63) {
            output[len++] = input[pos];
            pos++;
        }
    }
    
    output[len] = '\0';
    return len > 0 ? len : 0;
}

int parse_string(const char *input, int pos, char *output) {
    int len = 0;
    if (input == NULL || output == NULL || pos < 0) return 0;
    
    if (input[pos] == '"') {
        if (len < 63) output[len++] = input[pos];
        pos++;
        
        while (input[pos] != '\0' && input[pos] != '"' && len < 63) {
            if (input[pos] == '\\' && input[pos + 1] != '\0') {
                if (len < 63) output[len++] = input[pos];
                pos++;
                if (len < 63) output[len++] = input[pos];
                pos++;
            } else {
                if (len < 63) output[len++] = input[pos];
                pos++;
            }
        }
        
        if (input[pos] == '"' && len < 63) {
            output[len++] = input[pos];
        }
    }
    
    output[len] = '\0';
    return len > 0 ? len : 0;
}

void skip_whitespace(const char *input, int *pos, int *line, int *col) {
    if (input == NULL || pos == NULL || line == NULL || col == NULL) return;
    
    while (input[*pos] != '\0') {
        if (input[*pos] == ' ' || input[*pos] == '\t') {
            (*pos)++;
            (*col)++;
        } else if (input[*pos] == '\n') {
            (*pos)++;
            (*line)++;
            *col = 1;
        } else {
            break;
        }
    }
}

struct Token get_next_token(const char *input, int *pos, int *line, int *col) {
    struct Token token = {TOKEN_ERROR, "", *line, *col};
    
    if (input == NULL || pos == NULL || line == NULL || col == NULL) {
        return token;
    }
    
    skip_whitespace(input, pos, line, col);
    token.line = *line;
    token.column = *col;
    
    if (input[*pos] == '\0') {
        token.type = TOKEN_EOF;
        strncpy(token.value, "EOF", 63);
        token.value[63] = '\0';
        return token;
    }
    
    int advance = 0;
    
    if ((advance = parse_number(input, *pos, token.value)) > 0) {
        token.type = TOKEN_N