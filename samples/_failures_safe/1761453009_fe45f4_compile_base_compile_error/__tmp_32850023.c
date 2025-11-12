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
    char value[64];
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

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

void parser_init(struct Parser *p, const char *input) {
    p->input = input;
    p->pos = 0;
    p->len = strlen(input);
    p->line = 1;
    p->column = 1;
    p->current.type = TOKEN_INVALID;
    memset(p->current.value, 0, sizeof(p->current.value));
}

void skip_whitespace(struct Parser *p) {
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

int parse_number(struct Parser *p) {
    size_t start = p->pos;
    while (p->pos < p->len && isdigit(p->input[p->pos])) {
        p->pos++;
        p->column++;
    }
    if (p->pos - start > 63) return 0;
    strncpy(p->current.value, p->input + start, p->pos - start);
    p->current.value[p->pos - start] = '\0';
    return 1;
}

int parse_identifier(struct Parser *p) {
    size_t start = p->pos;
    if (p->pos < p->len && (isalpha(p->input[p->pos]) || p->input[p->pos] == '_')) {
        p->pos++;
        p->column++;
        while (p->pos < p->len && (isalnum(p->input[p->pos]) || p->input[p->pos] == '_')) {
            p->pos++;
            p->column++;
        }
        if (p->pos - start > 63) return 0;
        strncpy(p->current.value, p->input + start, p->pos - start);
        p->current.value[p->pos - start] = '\0';
        return 1;
    }
    return 0;
}

int parse_operator(struct Parser *p) {
    if (p->pos < p->len && is_operator_char(p->input[p->pos])) {
        size_t start = p->pos;
        p->pos++;
        p->column++;
        if (p->pos - start > 63) return 0;
        strncpy(p->current.value, p->input + start, p->pos - start);
        p->current.value[p->pos - start] = '\0';
        return 1;
    }
    return 0;
}

int parse_string(struct Parser *p) {
    if (p->pos < p->len && p->input[p->pos] == '"') {
        size_t start = p->pos;
        p->pos++;
        p->column++;
        while (p->pos < p->len && p->input[p->pos] != '"') {
            if (p->input[p->pos] == '\n') {
                p->line++;
                p->column = 1;
            } else {
                p->column++;
            }
            p->pos++;
        }
        if (p->pos >= p->len) return 0;
        p->pos++;
        p->column++;
        if (p->pos - start > 63) return 0;
        strncpy(p->current.value, p->input + start, p->pos - start);
        p->current.value[p->pos - start] = '\0';
        return 1;
    }
    return 0;
}

int next_token(struct Parser *p) {
    skip_whitespace(p);
    if (p->pos >= p->len) {
        p->current.type = TOKEN_EOF;
        memset(p->current.value, 0, sizeof(p->current.value));
        return 1;
    }
    p->current.line = p->line;
    p->current.column = p->column;
    if (parse_number(p)) {
        p->current.type = TOKEN_NUMBER;
        return 1;
    }
    if (parse_identifier(p)) {
        p->current.type = TOKEN_IDENTIFIER;
        return 1;
    }
    if (parse_operator(p)) {
        p->current.type = TOKEN_OPERATOR;
        return 1;
    }
    if (parse_string(p)) {
        p->current.type = TOKEN_STRING;
        return 1;
    }
    p->current.type = TOKEN_INVALID;
    p->current.value[0] = p->input[p->pos];
    p->current.value[1] = '\0';
    p->pos++;
    p->column++;
    return 0;
}

void print_token(struct Token *t