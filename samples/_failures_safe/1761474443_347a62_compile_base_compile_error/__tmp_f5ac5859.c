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

int parse_operator(const char *input, int pos, char *output) {
    int len = 0;
    char c = input[pos];
    
    if (is_operator_char(c)) {
        output[len++] = c;
        char next = input[pos + 1];
        
        if ((c == '=' && next == '=') || (c == '!' && next == '=') ||
            (c == '<' && next == '=') || (c == '>' && next == '=') ||
            (c == '&' && next == '&') || (c == '|' && next == '|')) {
            output[len++] = next;
        }
    }
    
    output[len] = '\0';
    return len > 0 ? len : 0;
}

int parse_number(const char *input, int pos, char *output) {
    int len = 0;
    int has_dot = 0;
    
    while (input[pos] != '\0') {
        char c = input[pos];
        if (isdigit(c)) {
            if (len < 63) output[len++] = c;
        } else if (c == '.' && !has_dot) {
            if (len < 63) output[len++] = c;
            has_dot = 1;
        } else {
            break;
        }
        pos++;
    }
    
    output[len] = '\0';
    return len > 0 ? len : 0;
}

int parse_identifier(const char *input, int pos, char *output) {
    int len = 0;
    
    if (isalpha(input[pos]) || input[pos] == '_') {
        while (input[pos] != '\0' && (isalnum(input[pos]) || input[pos] == '_')) {
            if (len < 63) output[len++] = input[pos];
            pos++;
        }
    }
    
    output[len] = '\0';
    return len > 0 ? len : 0;
}

int parse_string(const char *input, int pos, char *output) {
    int len = 0;
    
    if (input[pos] == '"') {
        if (len < 63) output[len++] = input[pos];
        pos++;
        
        while (input[pos] != '\0' && input[pos] != '"') {
            if (input[pos] == '\\' && input[pos + 1] != '\0') {
                if (len < 63) output[len++] = input[pos++];
                if (len < 63) output[len++] = input[pos++];
            } else {
                if (len < 63) output[len++] = input[pos++];
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
    while (input[*pos] != '\0') {
        if (input[*pos] == '\n') {
            (*line)++;
            *col = 1;
            (*pos)++;
        } else if (isspace(input[*pos])) {
            (*col)++;
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
        strcpy(token->value, "");
        token->line = *line;
        token->column = *col;
        return 1;
    }
    
    token->line = *line;
    token->column = *col;
    
    char buffer[64];
    int advance;
    
    if ((advance = parse_number(input, *pos, buffer)) > 0) {
        token->type = TOKEN_NUMBER;
        strncpy(token->value, buffer, 63);
        token->value[63] = '\0';
    } else if ((advance = parse_string(input, *pos, buffer)) > 0) {
        token->type = TOKEN_STRING;
        strncpy(token->value, buffer, 63);
        token->value[63] = '\0';
    } else if ((advance = parse_identifier(input, *pos, buffer)) > 0) {
        token->type = TOKEN_IDENTIFIER;
        strncpy(token->value, buffer, 63);
        token->value[63] = '\0';
    } else if ((advance = parse_operator(input, *pos, buffer)) > 0) {
        token->type = TOKEN_OPERATOR;
        strncpy(token->value, buffer, 63);
        token->value[63] = '\0';
    } else {
        token