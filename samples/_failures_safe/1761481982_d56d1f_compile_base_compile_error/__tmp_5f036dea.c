//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

enum TokenType { TOK_NUM, TOK_OP, TOK_EOF, TOK_INVALID };

struct Token {
    enum TokenType type;
    int value;
    char op;
};

struct Parser {
    const char *input;
    size_t pos;
    struct Token current;
};

void init_parser(struct Parser *p, const char *str) {
    p->input = str;
    p->pos = 0;
    p->current.type = TOK_INVALID;
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')';
}

void next_token(struct Parser *p) {
    while (p->input[p->pos] && isspace(p->input[p->pos])) {
        p->pos++;
    }
    
    if (!p->input[p->pos]) {
        p->current.type = TOK_EOF;
        return;
    }
    
    if (isdigit(p->input[p->pos])) {
        int val = 0;
        while (p->input[p->pos] && isdigit(p->input[p->pos])) {
            if (val > (INT_MAX - (p->input[p->pos] - '0')) / 10) {
                p->current.type = TOK_INVALID;
                return;
            }
            val = val * 10 + (p->input[p->pos] - '0');
            p->pos++;
        }
        p->current.type = TOK_NUM;
        p->current.value = val;
        return;
    }
    
    if (is_operator(p->input[p->pos])) {
        p->current.type = TOK_OP;
        p->current.op = p->input[p->pos];
        p->pos++;
        return;
    }
    
    p->current.type = TOK_INVALID;
}

int parse_expression(struct Parser *p);

int parse_factor(struct Parser *p) {
    if (p->current.type == TOK_NUM) {
        int val = p->current.value;
        next_token(p);
        return val;
    } else if (p->current.type == TOK_OP && p->current.op == '(') {
        next_token(p);
        int result = parse_expression(p);
        if (p->current.type != TOK_OP || p->current.op != ')') {
            return 0;
        }
        next_token(p);
        return result;
    }
    next_token(p);
    return 0;
}

int parse_term(struct Parser *p) {
    int left = parse_factor(p);
    
    while (p->current.type == TOK_OP && (p->current.op == '*' || p->current.op == '/')) {
        char op = p->current.op;
        next_token(p);
        int right = parse_factor(p);
        
        if (op == '*') {
            if (left > 0 && right > 0 && left > INT_MAX / right) {
                return 0;
            }
            if (left < 0 && right < 0 && left < INT_MAX / right) {
                return 0;
            }
            if (left > 0 && right < 0 && right < INT_MIN / left) {
                return 0;
            }
            if (left < 0 && right > 0 && left < INT_MIN / right) {
                return 0;
            }
            left *= right;
        } else if (op == '/') {
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

int parse_expression(struct Parser *p) {
    int left = parse_term(p);
    
    while (p->current.type == TOK_OP && (p->current.op == '+' || p->current.op == '-')) {
        char op = p->current.op;
        next_token(p);
        int right = parse_term(p);
        
        if (op == '+') {
            if (left > 0 && right > INT_MAX - left) {
                return 0;
            }
            if (left < 0 && right < INT_MIN - left) {
                return 0;
            }
            left += right;
        } else if (op == '-') {
            if (left < 0 && right > INT_MAX + left) {
                return 0;
            }
            if (left > 0 && right < INT_MIN + left) {
                return 0;
            }
            left -= right;
        }
    }
    
    return left;
}

int evaluate_expression(const char *expr) {
    struct Parser p;
    init_parser(&p, expr);
    next_token(&p);
    
    if (p.current.type == TOK_INVALID) {
        return 0;
    }
    
    int result = parse_expression(&p);
    
    if (p.current.type != TOK_EOF) {
        return 0;
    }
    
    return result;
}

int main(void) {
    char buffer[256];
    
    printf("Enter arithmetic expression (numbers and +-*/() only): ");
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        return 1;
    }
    
    buffer[strcspn(buffer, "\n")] = 0;
    
    if (strlen(buffer) == 0) {
        printf("Empty