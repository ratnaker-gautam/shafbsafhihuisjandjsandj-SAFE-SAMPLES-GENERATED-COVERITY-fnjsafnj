//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

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

void init_parser(struct Parser *p, const char *input) {
    p->input = input;
    p->pos = 0;
    p->current.type = TOKEN_INVALID;
    p->current.value = 0;
}

int is_digit(char c) {
    return c >= '0' && c <= '9';
}

void skip_whitespace(struct Parser *p) {
    while (p->input[p->pos] == ' ' || p->input[p->pos] == '\t') {
        p->pos++;
    }
}

struct Token get_next_token(struct Parser *p) {
    skip_whitespace(p);
    
    if (p->input[p->pos] == '\0') {
        return (struct Token){TOKEN_EOF, 0};
    }
    
    char current_char = p->input[p->pos];
    
    if (is_digit(current_char)) {
        int value = 0;
        while (is_digit(p->input[p->pos])) {
            if (value > (INT_MAX - (p->input[p->pos] - '0')) / 10) {
                return (struct Token){TOKEN_INVALID, 0};
            }
            value = value * 10 + (p->input[p->pos] - '0');
            p->pos++;
        }
        return (struct Token){TOKEN_NUMBER, value};
    }
    
    p->pos++;
    
    switch (current_char) {
        case '+': return (struct Token){TOKEN_PLUS, 0};
        case '-': return (struct Token){TOKEN_MINUS, 0};
        case '*': return (struct Token){TOKEN_MULTIPLY, 0};
        case '/': return (struct Token){TOKEN_DIVIDE, 0};
        case '(': return (struct Token){TOKEN_LPAREN, 0};
        case ')': return (struct Token){TOKEN_RPAREN, 0};
        default: return (struct Token){TOKEN_INVALID, 0};
    }
}

void consume_token(struct Parser *p, enum TokenType expected) {
    if (p->current.type == expected) {
        p->current = get_next_token(p);
    } else {
        p->current.type = TOKEN_INVALID;
    }
}

int parse_expression(struct Parser *p);

int parse_factor(struct Parser *p) {
    if (p->current.type == TOKEN_NUMBER) {
        int value = p->current.value;
        consume_token(p, TOKEN_NUMBER);
        return value;
    } else if (p->current.type == TOKEN_LPAREN) {
        consume_token(p, TOKEN_LPAREN);
        int value = parse_expression(p);
        if (p->current.type == TOKEN_RPAREN) {
            consume_token(p, TOKEN_RPAREN);
            return value;
        } else {
            p->current.type = TOKEN_INVALID;
            return 0;
        }
    } else {
        p->current.type = TOKEN_INVALID;
        return 0;
    }
}

int parse_term(struct Parser *p) {
    int result = parse_factor(p);
    
    while (p->current.type == TOKEN_MULTIPLY || p->current.type == TOKEN_DIVIDE) {
        enum TokenType op = p->current.type;
        consume_token(p, op);
        int right = parse_factor(p);
        
        if (op == TOKEN_MULTIPLY) {
            if (result > 0 && right > 0 && result > INT_MAX / right) {
                p->current.type = TOKEN_INVALID;
                return 0;
            }
            if (result < 0 && right < 0 && result < INT_MAX / right) {
                p->current.type = TOKEN_INVALID;
                return 0;
            }
            if (result > 0 && right < 0 && right < INT_MIN / result) {
                p->current.type = TOKEN_INVALID;
                return 0;
            }
            if (result < 0 && right > 0 && result < INT_MIN / right) {
                p->current.type = TOKEN_INVALID;
                return 0;
            }
            result *= right;
        } else {
            if (right == 0) {
                p->current.type = TOKEN_INVALID;
                return 0;
            }
            if (result == INT_MIN && right == -1) {
                p->current.type = TOKEN_INVALID;
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
        consume_token(p, op);
        int right = parse_term(p);
        
        if (op == TOKEN_PLUS) {