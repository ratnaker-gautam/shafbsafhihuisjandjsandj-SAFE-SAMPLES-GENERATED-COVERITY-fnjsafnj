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

int parse_operator(const char *input, int pos, char *buffer) {
    int len = 0;
    char c = input[pos];
    
    if (is_operator_char(c)) {
        buffer[len++] = c;
        if (is_operator_char(input[pos + 1])) {
            buffer[len++] = input[pos + 1];
        }
    }
    buffer[len] = '\0';
    return len;
}

int parse_number(const char *input, int pos, char *buffer) {
    int len = 0;
    int has_dot = 0;
    
    while (input[pos] && (isdigit(input[pos]) || input[pos] == '.')) {
        if (input[pos] == '.') {
            if (has_dot) break;
            has_dot = 1;
        }
        if (len < 63) buffer[len++] = input[pos++];
        else break;
    }
    buffer[len] = '\0';
    return len;
}

int parse_identifier(const char *input, int pos, char *buffer) {
    int len = 0;
    
    if (input[pos] && (isalpha(input[pos]) || input[pos] == '_')) {
        while (input[pos] && (isalnum(input[pos]) || input[pos] == '_')) {
            if (len < 63) buffer[len++] = input[pos++];
            else break;
        }
    }
    buffer[len] = '\0';
    return len;
}

int parse_string(const char *input, int pos, char *buffer) {
    int len = 0;
    
    if (input[pos] == '"') {
        pos++;
        while (input[pos] && input[pos] != '"') {
            if (input[pos] == '\\' && input[pos + 1]) {
                if (len < 62) {
                    buffer[len++] = input[pos++];
                    buffer[len++] = input[pos++];
                } else break;
            } else {
                if (len < 63) buffer[len++] = input[pos++];
                else break;
            }
        }
        if (input[pos] == '"') {
            if (len < 63) buffer[len++] = input[pos++];
        }
    }
    buffer[len] = '\0';
    return len;
}

void tokenize(const char *input, struct Token *tokens, int max_tokens, int *token_count) {
    int pos = 0;
    int line = 1;
    int column = 1;
    *token_count = 0;
    
    while (input[pos] && *token_count < max_tokens - 1) {
        while (isspace(input[pos])) {
            if (input[pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
        }
        
        if (!input[pos]) break;
        
        int start_pos = pos;
        int start_column = column;
        char buffer[64];
        int advance = 0;
        enum TokenType type = TOKEN_UNKNOWN;
        
        if (isdigit(input[pos])) {
            advance = parse_number(input, pos, buffer);
            if (advance > 0) type = TOKEN_NUMBER;
        } else if (isalpha(input[pos]) || input[pos] == '_') {
            advance = parse_identifier(input, pos, buffer);
            if (advance > 0) type = TOKEN_IDENTIFIER;
        } else if (input[pos] == '"') {
            advance = parse_string(input, pos, buffer);
            if (advance > 0) type = TOKEN_STRING;
        } else if (is_operator_char(input[pos])) {
            advance = parse_operator(input, pos, buffer);
            if (advance > 0) type = TOKEN_OPERATOR;
        }
        
        if (advance > 0) {
            tokens[*token_count].type = type;
            strncpy(tokens[*token_count].value, buffer, 63);
            tokens[*token_count].value[63] = '\0';
            tokens[*token_count].line = line;
            tokens[*token_count].column = start_column;
            (*token_count)++;
            pos += advance;
            column += advance;
        } else {
            tokens[*token_count].type = TOKEN_UNKNOWN;
            tokens[*token_count].value[0] = input[pos];
            tokens[*token_count].value[1] = '\0';
            tokens[*token_count].line = line;
            tokens[*token_count].column = column;
            (*token_count)++;
            pos++;
            column++;
        }
    }
    
    tokens[*token_count].type = TOKEN_EOF;
    tokens[*token_count].value[0] = '\0';
    tokens[*token_count].line = line;
    tokens[*token_count].column = column;
    (*token_count)++;
}

const char *token_type_name(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: