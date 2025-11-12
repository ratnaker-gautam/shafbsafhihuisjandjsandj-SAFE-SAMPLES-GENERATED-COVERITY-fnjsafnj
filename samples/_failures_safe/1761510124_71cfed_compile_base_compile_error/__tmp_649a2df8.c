//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_PAREN,
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

void init_parser(struct Parser *p, const char *input) {
    if (p == NULL || input == NULL) return;
    p->input = input;
    p->pos = 0;
    p->len = strlen(input);
    p->line = 1;
    p->column = 1;
    p->current.type = TOKEN_INVALID;
    memset(p->current.value, 0, sizeof(p->current.value));
    p->current.line = 1;
    p->current.column = 1;
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

void next_token(struct Parser *p) {
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
    
    if (p->pos >= p->len) {
        p->current.type = TOKEN_EOF;
        memset(p->current.value, 0, sizeof(p->current.value));
        p->current.line = p->line;
        p->current.column = p->column;
        return;
    }
    
    char c = p->input[p->pos];
    p->current.line = p->line;
    p->current.column = p->column;
    memset(p->current.value, 0, sizeof(p->current.value));
    
    if (isdigit(c)) {
        p->current.type = TOKEN_NUMBER;
        size_t i = 0;
        while (p->pos < p->len && i < sizeof(p->current.value) - 1 && isdigit(p->input[p->pos])) {
            p->current.value[i++] = p->input[p->pos++];
            p->column++;
        }
        p->current.value[i] = '\0';
    } else if (isalpha(c) || c == '_') {
        p->current.type = TOKEN_IDENTIFIER;
        size_t i = 0;
        while (p->pos < p->len && i < sizeof(p->current.value) - 1 && 
               (isalnum(p->input[p->pos]) || p->input[p->pos] == '_')) {
            p->current.value[i++] = p->input[p->pos++];
            p->column++;
        }
        p->current.value[i] = '\0';
    } else if (is_operator(c)) {
        p->current.type = TOKEN_OPERATOR;
        p->current.value[0] = c;
        p->current.value[1] = '\0';
        p->pos++;
        p->column++;
    } else if (c == '(' || c == ')') {
        p->current.type = TOKEN_PAREN;
        p->current.value[0] = c;
        p->current.value[1] = '\0';
        p->pos++;
        p->column++;
    } else {
        p->current.type = TOKEN_INVALID;
        p->current.value[0] = c;
        p->current.value[1] = '\0';
        p->pos++;
        p->column++;
    }
}

void parse_expression(struct Parser *p);

void parse_primary(struct Parser *p) {
    if (p == NULL) return;
    
    switch (p->current.type) {
        case TOKEN_NUMBER:
            printf("Number: %s\n", p->current.value);
            next_token(p);
            break;
        case TOKEN_IDENTIFIER:
            printf("Identifier: %s\n", p->current.value);
            next_token(p);
            break;
        case TOKEN_PAREN:
            if (strcmp(p->current.value, "(") == 0) {
                printf("Open paren\n");
                next_token(p);
                parse_expression(p);
                if (p->current.type == TOKEN_PAREN && strcmp(p->current.value, ")") == 0) {
                    printf("Close paren\n");
                    next_token(p);
                } else {
                    printf("Error: Expected closing parenthesis\n");
                }
            } else {
                printf("Error: Unexpected token '%s'\n", p->current.value);
            }
            break;
        default:
            printf("Error: Unexpected token '%s'\n", p->current.value);
            break;
    }
}

void parse_expression(struct Parser *p) {
    if (p == NULL) return;
    
    parse_primary(p);
    
    while (p->current.type == TOKEN_OPERATOR && 
           (strcmp(p->current.value, "+") == 0 || strcmp(p->current.value, "-") == 0 ||
            strcmp(p->current.value, "*") == 0 || strcmp(p->current.value, "/") ==