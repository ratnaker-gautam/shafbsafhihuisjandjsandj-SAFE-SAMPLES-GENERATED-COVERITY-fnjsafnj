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

void skip_whitespace(struct Parser *p) {
    while (p->input[p->pos] && isspace(p->input[p->pos])) {
        p->pos++;
    }
}

struct Token get_next_token(struct Parser *p) {
    skip_whitespace(p);
    
    if (p->input[p->pos] == '\0') {
        return (struct Token){TOK_EOF, 0, 0};
    }
    
    if (isdigit(p->input[p->pos])) {
        int val = 0;
        while (p->input[p->pos] && isdigit(p->input[p->pos])) {
            if (val > (INT_MAX - (p->input[p->pos] - '0')) / 10) {
                return (struct Token){TOK_INVALID, 0, 0};
            }
            val = val * 10 + (p->input[p->pos] - '0');
            p->pos++;
        }
        return (struct Token){TOK_NUM, val, 0};
    }
    
    if (is_operator(p->input[p->pos])) {
        char op = p->input[p->pos];
        p->pos++;
        return (struct Token){TOK_OP, 0, op};
    }
    
    return (struct Token){TOK_INVALID, 0, 0};
}

void advance(struct Parser *p) {
    p->current = get_next_token(p);
}

int parse_expression(struct Parser *p);

int parse_factor(struct Parser *p) {
    if (p->current.type == TOK_NUM) {
        int val = p->current.value;
        advance(p);
        return val;
    } else if (p->current.type == TOK_OP && p->current.op == '(') {
        advance(p);
        int result = parse_expression(p);
        if (p->current.type != TOK_OP || p->current.op != ')') {
            fprintf(stderr, "Error: Expected ')'\n");
            exit(EXIT_FAILURE);
        }
        advance(p);
        return result;
    } else {
        fprintf(stderr, "Error: Expected number or '('\n");
        exit(EXIT_FAILURE);
    }
}

int parse_term(struct Parser *p) {
    int result = parse_factor(p);
    
    while (p->current.type == TOK_OP && (p->current.op == '*' || p->current.op == '/')) {
        char op = p->current.op;
        advance(p);
        int right = parse_factor(p);
        
        if (op == '*') {
            if (result > 0 && right > 0 && result > INT_MAX / right) {
                fprintf(stderr, "Error: Integer overflow\n");
                exit(EXIT_FAILURE);
            }
            result *= right;
        } else {
            if (right == 0) {
                fprintf(stderr, "Error: Division by zero\n");
                exit(EXIT_FAILURE);
            }
            result /= right;
        }
    }
    
    return result;
}

int parse_expression(struct Parser *p) {
    int result = parse_term(p);
    
    while (p->current.type == TOK_OP && (p->current.op == '+' || p->current.op == '-')) {
        char op = p->current.op;
        advance(p);
        int right = parse_term(p);
        
        if (op == '+') {
            if (result > 0 && right > INT_MAX - result) {
                fprintf(stderr, "Error: Integer overflow\n");
                exit(EXIT_FAILURE);
            }
            result += right;
        } else {
            if (right < 0 && result > INT_MAX + right) {
                fprintf(stderr, "Error: Integer overflow\n");
                exit(EXIT_FAILURE);
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
    
    if (parser.current.type == TOK_EOF) {
        return 0;
    }
    
    int result = parse_expression(&parser);
    
    if (parser.current.type != TOK_EOF) {
        fprintf(stderr, "Error: Unexpected token\n");
        exit(EXIT_FAILURE);
    }
    
    return result;
}

int main(void) {
    char input[256];
    
    printf("Enter arithmetic expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len