//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

typedef enum {
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MUL,
    TOKEN_DIV,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_END,
    TOKEN_ERROR
} TokenType;

typedef struct {
    TokenType type;
    int value;
} Token;

typedef struct {
    const char *input;
    size_t pos;
    Token current;
} Parser;

void next_token(Parser *p) {
    if (p == NULL || p->input == NULL) {
        return;
    }
    
    while (p->input[p->pos] != '\0' && isspace((unsigned char)p->input[p->pos])) {
        p->pos++;
    }
    
    if (p->input[p->pos] == '\0') {
        p->current.type = TOKEN_END;
        return;
    }
    
    char c = p->input[p->pos];
    if (isdigit((unsigned char)c)) {
        int val = 0;
        while (p->input[p->pos] != '\0' && isdigit((unsigned char)p->input[p->pos])) {
            int digit = p->input[p->pos] - '0';
            if (val > (INT_MAX - digit) / 10) {
                p->current.type = TOKEN_ERROR;
                return;
            }
            val = val * 10 + digit;
            p->pos++;
        }
        p->current.type = TOKEN_NUMBER;
        p->current.value = val;
    } else {
        p->pos++;
        switch (c) {
            case '+': p->current.type = TOKEN_PLUS; break;
            case '-': p->current.type = TOKEN_MINUS; break;
            case '*': p->current.type = TOKEN_MUL; break;
            case '/': p->current.type = TOKEN_DIV; break;
            case '(': p->current.type = TOKEN_LPAREN; break;
            case ')': p->current.type = TOKEN_RPAREN; break;
            default: p->current.type = TOKEN_ERROR; break;
        }
    }
}

int parse_expression(Parser *p);

int parse_factor(Parser *p) {
    if (p == NULL) {
        return 0;
    }
    
    if (p->current.type == TOKEN_NUMBER) {
        int val = p->current.value;
        next_token(p);
        return val;
    } else if (p->current.type == TOKEN_LPAREN) {
        next_token(p);
        int val = parse_expression(p);
        if (p->current.type != TOKEN_RPAREN) {
            return 0;
        }
        next_token(p);
        return val;
    } else if (p->current.type == TOKEN_MINUS) {
        next_token(p);
        int val = parse_factor(p);
        if (val == INT_MIN) {
            return 0;
        }
        return -val;
    }
    return 0;
}

int parse_term(Parser *p) {
    if (p == NULL) {
        return 0;
    }
    
    int left = parse_factor(p);
    
    while (p->current.type == TOKEN_MUL || p->current.type == TOKEN_DIV) {
        TokenType op = p->current.type;
        next_token(p);
        int right = parse_factor(p);
        
        if (op == TOKEN_MUL) {
            if (left != 0 && right != 0) {
                if ((left > 0 && right > 0 && left > INT_MAX / right) ||
                    (left > 0 && right < 0 && right < INT_MIN / left) ||
                    (left < 0 && right > 0 && left < INT_MIN / right) ||
                    (left < 0 && right < 0 && left < INT_MAX / right)) {
                    return 0;
                }
            }
            left *= right;
        } else {
            if (right == 0) {
                return 0;
            }
            if (left == INT_MIN && right == -1) {
                return 0;
            }
            left /= right;
        }
    }
    
    return left;
}

int parse_expression(Parser *p) {
    if (p == NULL) {
        return 0;
    }
    
    int left = parse_term(p);
    
    while (p->current.type == TOKEN_PLUS || p->current.type == TOKEN_MINUS) {
        TokenType op = p->current.type;
        next_token(p);
        int right = parse_term(p);
        
        if (op == TOKEN_PLUS) {
            if ((right > 0 && left > INT_MAX - right) ||
                (right < 0 && left < INT_MIN - right)) {
                return 0;
            }
            left += right;
        } else {
            if ((right > 0 && left < INT_MIN + right) ||
                (right < 0 && left > INT_MAX + right)) {
                return 0;
            }
            left -= right;
        }
    }
    
    return left;
}

int evaluate_expression(const char *expr) {
    if (expr == NULL) {
        return 0;
    }
    
    Parser p;
    p.input = expr;
    p.pos = 0;
    next_token(&p);
    
    if (p.current.type == TOKEN_ERROR) {
        return