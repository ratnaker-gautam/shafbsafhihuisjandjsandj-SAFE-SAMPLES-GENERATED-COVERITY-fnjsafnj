//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

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
    char value[MAX_TOKEN_LEN];
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
        if (len < MAX_TOKEN_LEN - 1) {
            buffer[len++] = input[pos++];
        } else {
            break;
        }
    }
    buffer[len] = '\0';
    return len;
}

int parse_identifier(const char *input, int pos, char *buffer) {
    int len = 0;
    
    if (input[pos] && (isalpha(input[pos]) || input[pos] == '_')) {
        while (input[pos] && (isalnum(input[pos]) || input[pos] == '_')) {
            if (len < MAX_TOKEN_LEN - 1) {
                buffer[len++] = input[pos++];
            } else {
                break;
            }
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
            if (len < MAX_TOKEN_LEN - 1) {
                buffer[len++] = input[pos++];
            } else {
                break;
            }
        }
        if (input[pos] == '"') {
            pos++;
        }
    }
    buffer[len] = '\0';
    return len + (len > 0 ? 2 : 0);
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    int line = 1;
    int column = 1;
    
    while (input[pos] && token_count < max_tokens) {
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
        
        char buffer[MAX_TOKEN_LEN];
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
            if (advance > 2) type = TOKEN_STRING;
        } else if (is_operator_char(input[pos])) {
            advance = parse_operator(input, pos, buffer);
            if (advance > 0) type = TOKEN_OPERATOR;
        }
        
        if (advance > 0) {
            tokens[token_count].type = type;
            strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            tokens[token_count].line = line;
            tokens[token_count].column = column;
            token_count++;
            
            pos += advance;
            column += advance;
        } else {
            buffer[0] = input[pos];
            buffer[1] = '\0';
            tokens[token_count].type = TOKEN_UNKNOWN;
            strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            tokens[token_count].line = line;
            tokens[token_count].column = column;
            token_count++;
            
            pos++;
            column++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        tokens[token_count].line = line;
        tokens[token_count].column = column;
        token_count++;
    }
    
    return token_count;
}

void print_token_type(enum TokenType type