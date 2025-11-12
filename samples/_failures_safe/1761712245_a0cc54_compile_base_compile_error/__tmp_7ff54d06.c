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
        char next = input[pos + 1];
        
        if ((c == '=' && next == '=') || (c == '!' && next == '=') ||
            (c == '<' && next == '=') || (c == '>' && next == '=') ||
            (c == '&' && next == '&') || (c == '|' && next == '|')) {
            buffer[len++] = next;
        }
    }
    
    buffer[len] = '\0';
    return len;
}

int parse_number(const char *input, int pos, char *buffer) {
    int len = 0;
    int has_dot = 0;
    
    while (input[pos] != '\0') {
        char c = input[pos];
        if (isdigit(c)) {
            buffer[len++] = c;
        } else if (c == '.' && !has_dot) {
            buffer[len++] = c;
            has_dot = 1;
        } else {
            break;
        }
        pos++;
    }
    
    buffer[len] = '\0';
    return len;
}

int parse_string(const char *input, int pos, char *buffer) {
    int len = 0;
    
    if (input[pos] == '"') {
        buffer[len++] = '"';
        pos++;
        
        while (input[pos] != '\0' && input[pos] != '"') {
            if (len < 63) {
                buffer[len++] = input[pos];
            }
            pos++;
        }
        
        if (input[pos] == '"') {
            buffer[len++] = '"';
        }
    }
    
    buffer[len] = '\0';
    return len;
}

int parse_identifier(const char *input, int pos, char *buffer) {
    int len = 0;
    
    if (isalpha(input[pos]) || input[pos] == '_') {
        buffer[len++] = input[pos];
        pos++;
        
        while (input[pos] != '\0' && (isalnum(input[pos]) || input[pos] == '_')) {
            if (len < 63) {
                buffer[len++] = input[pos];
            }
            pos++;
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
    
    while (input[pos] != '\0' && *token_count < max_tokens - 1) {
        while (isspace(input[pos])) {
            if (input[pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
        }
        
        if (input[pos] == '\0') break;
        
        struct Token *token = &tokens[*token_count];
        token->line = line;
        token->column = column;
        
        int advance = 0;
        
        if (isdigit(input[pos])) {
            advance = parse_number(input, pos, token->value);
            token->type = TOKEN_NUMBER;
        } else if (isalpha(input[pos]) || input[pos] == '_') {
            advance = parse_identifier(input, pos, token->value);
            token->type = TOKEN_IDENTIFIER;
        } else if (input[pos] == '"') {
            advance = parse_string(input, pos, token->value);
            token->type = TOKEN_STRING;
        } else if (is_operator_char(input[pos])) {
            advance = parse_operator(input, pos, token->value);
            token->type = TOKEN_OPERATOR;
        } else {
            token->value[0] = input[pos];
            token->value[1] = '\0';
            token->type = TOKEN_UNKNOWN;
            advance = 1;
        }
        
        if (advance > 0) {
            column += advance;
            pos += advance;
            (*token_count)++;
        } else {
            break;
        }
    }
    
    if (*token_count < max_tokens) {
        tokens[*token_count].type = TOKEN_EOF;
        tokens[*token_count].value[0] = '\0';
        tokens[*token_count].line = line;
        tokens[*token_count].column = column;
        (*token_count)++;
    }
}

const char *token_type_name(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return