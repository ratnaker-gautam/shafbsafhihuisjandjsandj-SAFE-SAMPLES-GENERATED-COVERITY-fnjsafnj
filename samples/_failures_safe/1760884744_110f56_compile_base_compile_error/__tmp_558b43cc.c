//DeepSeek-V3 SAFE v1.3 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_EOF,
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    int value;
};

struct Parser {
    const char *input;
    size_t pos;
    struct Token current;
};

void init_parser(struct Parser *p, const char *str) {
    p->input = str;
    p->pos = 0;
    p->current.type = TOKEN_INVALID;
    p->current.value = 0;
}

void skip_whitespace(struct Parser *p) {
    while (p->input[p->pos] && isspace(p->input[p->pos])) {
        p->pos++;
    }
}

struct Token get_next_token(struct Parser *p) {
    skip_whitespace(p);
    
    if (p->input[p->pos] == '\0') {
        struct Token token = {TOKEN_EOF, 0};
        return token;
    }
    
    char c = p->input[p->pos];
    
    if (isdigit(c)) {
        int value = 0;
        while (p->input[p->pos] && isdigit(p->input[p->pos])) {
            if (value > (INT_MAX - (p->input[p->pos] - '0')) / 10) {
                struct Token token = {TOKEN_INVALID, 0};
                return token;
            }
            value = value * 10 + (p->input[p->pos] - '0');
            p->pos++;
        }
        struct Token token = {TOKEN_NUMBER, value};
        return token;
    }
    
    p->pos++;
    
    switch (c) {
        case '+': { struct Token token = {TOKEN_PLUS, 0}; return token; }
        case '-': { struct Token token = {TOKEN_MINUS, 0}; return token; }
        case '*': { struct Token token = {TOKEN_MULTIPLY, 0}; return token; }
        case '/': { struct Token token = {TOKEN_DIVIDE, 0}; return token; }
        case '(': { struct Token token = {TOKEN_LPAREN, 0}; return token; }
        case ')': { struct Token token = {TOKEN_RPAREN, 0}; return token; }
        default: { struct Token token = {TOKEN_INVALID, 0}; return token; }
    }
}

void advance(struct Parser *p) {
    p->current = get_next_token(p);
}

int parse_expression(struct Parser *p);

int parse_factor(struct Parser *p) {
    if (p->current.type == TOKEN_NUMBER) {
        int value = p->current.value;
        advance(p);
        return value;
    } else if (p->current.type == TOKEN_LPAREN) {
        advance(p);
        int value = parse_expression(p);
        if (p->current.type != TOKEN_RPAREN) {
            return 0;
        }
        advance(p);
        return value;
    } else if (p->current.type == TOKEN_MINUS) {
        advance(p);
        return -parse_factor(p);
    } else if (p->current.type == TOKEN_PLUS) {
        advance(p);
        return parse_factor(p);
    }
    return 0;
}

int parse_term(struct Parser *p) {
    int result = parse_factor(p);
    
    while (p->current.type == TOKEN_MULTIPLY || p->current.type == TOKEN_DIVIDE) {
        enum TokenType op = p->current.type;
        advance(p);
        int right = parse_factor(p);
        
        if (op == TOKEN_MULTIPLY) {
            if (result != 0 && right > INT_MAX / result) {
                return 0;
            }
            result *= right;
        } else {
            if (right == 0) {
                return 0;
            }
            result /= right;
        }
    }
    
    return result;
}

int parse_expression(struct Parser *p) {
    int result = parse_term(p);
    
    while (p->current.type == TOKEN_PLUS || p->current.type == TOKEN_MINUS) {
        enum TokenType op = p->current.type;
        advance(p);
        int right = parse_term(p);
        
        if (op == TOKEN_PLUS) {
            if (result > INT_MAX - right) {
                return 0;
            }
            result += right;
        } else {
            if (result < INT_MIN + right) {
                return 0;
            }
            result -= right;
        }
    }
    
    return result;
}

int evaluate_expression(const char *expr) {
    struct Parser parser;
    init_parser(&parser, expr);
    advance(&parser);
    
    if (