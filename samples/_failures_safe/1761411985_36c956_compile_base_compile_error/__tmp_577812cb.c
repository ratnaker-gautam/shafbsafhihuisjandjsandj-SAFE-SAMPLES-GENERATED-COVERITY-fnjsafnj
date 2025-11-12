//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
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
    char value[32];
    int line;
    int column;
};

struct Parser {
    const char *input;
    size_t pos;
    size_t len;
    int line;
    int column;
    struct Token current;
};

void init_parser(struct Parser *p, const char *text) {
    if (p == NULL || text == NULL) return;
    p->input = text;
    p->pos = 0;
    p->len = strlen(text);
    p->line = 1;
    p->column = 1;
    p->current.type = TOKEN_ERROR;
    memset(p->current.value, 0, sizeof(p->current.value));
    p->current.line = 1;
    p->current.column = 1;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(struct Parser *p) {
    if (p == NULL || p->pos >= p->len) return;
    while (p->pos < p->len && isspace(p->input[p->pos])) {
        if (p->input[p->pos] == '\n') {
            p->line++;
            p->column = 1;
        } else {
            p->column++;
        }
        p->pos++;
    }
}

struct Token get_next_token(struct Parser *p) {
    if (p == NULL) {
        struct Token error_token;
        error_token.type = TOKEN_ERROR;
        memset(error_token.value, 0, sizeof(error_token.value));
        error_token.line = 1;
        error_token.column = 1;
        return error_token;
    }
    
    skip_whitespace(p);
    
    if (p->pos >= p->len) {
        struct Token eof_token;
        eof_token.type = TOKEN_EOF;
        memset(eof_token.value, 0, sizeof(eof_token.value));
        eof_token.line = p->line;
        eof_token.column = p->column;
        return eof_token;
    }
    
    char current_char = p->input[p->pos];
    struct Token token;
    token.line = p->line;
    token.column = p->column;
    memset(token.value, 0, sizeof(token.value));
    
    if (isdigit(current_char)) {
        token.type = TOKEN_NUMBER;
        size_t i = 0;
        while (p->pos < p->len && i < sizeof(token.value) - 1 && isdigit(p->input[p->pos])) {
            token.value[i++] = p->input[p->pos++];
            p->column++;
        }
        token.value[i] = '\0';
    } else if (isalpha(current_char) || current_char == '_') {
        token.type = TOKEN_IDENTIFIER;
        size_t i = 0;
        while (p->pos < p->len && i < sizeof(token.value) - 1 && 
               (isalnum(p->input[p->pos]) || p->input[p->pos] == '_')) {
            token.value[i++] = p->input[p->pos++];
            p->column++;
        }
        token.value[i] = '\0';
    } else if (is_operator_char(current_char)) {
        token.type = TOKEN_OPERATOR;
        size_t i = 0;
        while (p->pos < p->len && i < sizeof(token.value) - 1 && 
               is_operator_char(p->input[p->pos])) {
            token.value[i++] = p->input[p->pos++];
            p->column++;
        }
        token.value[i] = '\0';
    } else if (current_char == '"') {
        token.type = TOKEN_STRING;
        size_t i = 0;
        p->pos++;
        p->column++;
        while (p->pos < p->len && i < sizeof(token.value) - 1 && p->input[p->pos] != '"') {
            token.value[i++] = p->input[p->pos++];
            p->column++;
        }
        if (p->pos < p->len && p->input[p->pos] == '"') {
            p->pos++;
            p->column++;
        }
        token.value[i] = '\0';
    } else {
        token.type = TOKEN_ERROR;
        token.value[0] = current_char;
        token.value[1] = '\0';
        p->pos++;
        p->column++;
    }
    
    return token;
}

void parse_expression(struct Parser *p) {
    if (p == NULL) return;
    
    struct Token token = p->current;
    
    switch (token.type) {
        case TOKEN_NUMBER:
            printf("Number: %s (line %d, column %d)\n", token.value, token.line, token.column);
            break;
        case TOKEN_IDENTIFIER:
            printf("Identifier: %s (line %d, column %d)\n", token.value, token.line, token.column);
            break;
        case TOKEN_OPERATOR:
            printf("Operator: %s (line %d, column %d)\