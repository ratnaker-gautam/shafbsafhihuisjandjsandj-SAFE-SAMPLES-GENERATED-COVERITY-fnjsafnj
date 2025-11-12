//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
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

void next_token(struct Parser *p) {
    if (!p || !p->input) return;
    
    while (p->input[p->pos] && isspace(p->input[p->pos])) {
        p->pos++;
    }
    
    if (!p->input[p->pos]) {
        p->current.type = TOK_EOF;
        return;
    }
    
    char c = p->input[p->pos];
    if (isdigit(c)) {
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
    } else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')') {
        p->current.type = TOK_OP;
        p->current.op = c;
        p->pos++;
    } else {
        p->current.type = TOK_INVALID;
    }
}

int parse_expression(struct Parser *p);

int parse_factor(struct Parser *p) {
    if (!p) return 0;
    
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
    if (!p) return 0;
    
    int left = parse_factor(p);
    
    while (p->current.type == TOK_OP && (p->current.op == '*' || p->current.op == '/')) {
        char op = p->current.op;
        next_token(p);
        int right = parse_factor(p);
        
        if (op == '*') {
            if (left != 0 && right > INT_MAX / left) {
                return 0;
            }
            left *= right;
        } else if (op == '/') {
            if (right == 0) {
                return 0;
            }
            left /= right;
        }
    }
    
    return left;
}

int parse_expression(struct Parser *p) {
    if (!p) return 0;
    
    int left = parse_term(p);
    
    while (p->current.type == TOK_OP && (p->current.op == '+' || p->current.op == '-')) {
        char op = p->current.op;
        next_token(p);
        int right = parse_term(p);
        
        if (op == '+') {
            if (right > 0 && left > INT_MAX - right) {
                return 0;
            }
            left += right;
        } else if (op == '-') {
            if (right < 0 && left > INT_MAX + right) {
                return 0;
            }
            left -= right;
        }
    }
    
    return left;
}

int evaluate_expression(const char *expr) {
    if (!expr) return 0;
    
    struct Parser p;
    p.input = expr;
    p.pos = 0;
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
    char input[256];
    
    printf("Enter arithmetic expression (+, -, *, /, numbers, parentheses): ");
    if (!fgets(input, sizeof(input), stdin)) {
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = 0;
    }
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    int result = evaluate_expression(input);
    printf("Result: %d\n", result);
    
    return 0;
}