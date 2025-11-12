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
    TOKEN_INVALID
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
    p->current.type = TOKEN_INVALID;
    memset(p->current.value, 0, sizeof(p->current.value));
    p->current.line = 1;
    p->current.column = 1;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(struct Parser *p) {
    if (p == NULL || p->input == NULL) return;
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
    if (p == NULL || p->input == NULL) {
        struct Token invalid = {TOKEN_INVALID, "", 0, 0};
        return invalid;
    }
    
    skip_whitespace(p);
    
    if (p->pos >= p->len) {
        struct Token eof = {TOKEN_EOF, "", p->line, p->column};
        return eof;
    }
    
    char c = p->input[p->pos];
    struct Token token;
    token.line = p->line;
    token.column = p->column;
    memset(token.value, 0, sizeof(token.value));
    
    if (isdigit(c)) {
        token.type = TOKEN_NUMBER;
        size_t i = 0;
        while (p->pos < p->len && i < sizeof(token.value) - 1 && isdigit(p->input[p->pos])) {
            token.value[i++] = p->input[p->pos++];
            p->column++;
        }
        token.value[i] = '\0';
    } else if (isalpha(c) || c == '_') {
        token.type = TOKEN_IDENTIFIER;
        size_t i = 0;
        while (p->pos < p->len && i < sizeof(token.value) - 1 && 
               (isalnum(p->input[p->pos]) || p->input[p->pos] == '_')) {
            token.value[i++] = p->input[p->pos++];
            p->column++;
        }
        token.value[i] = '\0';
    } else if (is_operator_char(c)) {
        token.type = TOKEN_OPERATOR;
        size_t i = 0;
        while (p->pos < p->len && i < sizeof(token.value) - 1 && is_operator_char(p->input[p->pos])) {
            token.value[i++] = p->input[p->pos++];
            p->column++;
        }
        token.value[i] = '\0';
    } else if (c == '"') {
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
        token.type = TOKEN_INVALID;
        token.value[0] = c;
        token.value[1] = '\0';
        p->pos++;
        p->column++;
    }
    
    return token;
}

void print_token(const struct Token *token) {
    if (token == NULL) return;
    
    switch (token->type) {
        case TOKEN_NUMBER:
            printf("NUMBER: %s at line %d, column %d\n", token->value, token->line, token->column);
            break;
        case TOKEN_IDENTIFIER:
            printf("IDENTIFIER: %s at line %d, column %d\n", token->value, token->line, token->column);
            break;
        case TOKEN_OPERATOR:
            printf("OPERATOR: %s at line %d, column %d\n", token->value, token->line, token->column);
            break;
        case TOKEN_STRING:
            printf("STRING: \"%s\" at line %d, column %d\n", token->value, token->line, token->column);
            break;
        case TOKEN_EOF:
            printf("EOF at line