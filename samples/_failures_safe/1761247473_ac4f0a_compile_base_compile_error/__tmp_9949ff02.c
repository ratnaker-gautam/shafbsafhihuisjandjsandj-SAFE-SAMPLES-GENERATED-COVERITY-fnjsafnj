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

void next_token(struct Parser *p) {
    if (p == NULL || p->input == NULL) return;
    
    while (p->input[p->pos] != '\0' && isspace((unsigned char)p->input[p->pos])) p->pos++;
    
    if (p->input[p->pos] == '\0') {
        p->current.type = TOK_EOF;
        return;
    }
    
    if (isdigit((unsigned char)p->input[p->pos])) {
        int val = 0;
        while (p->input[p->pos] != '\0' && isdigit((unsigned char)p->input[p->pos])) {
            int digit = p->input[p->pos] - '0';
            if (val > (INT_MAX - digit) / 10) {
                p->current.type = TOK_INVALID;
                return;
            }
            val = val * 10 + digit;
            p->pos++;
        }
        p->current.type = TOK_NUM;
        p->current.value = val;
        return;
    }
    
    if (strchr("+-*/()", p->input[p->pos]) != NULL) {
        p->current.type = TOK_OP;
        p->current.op = p->input[p->pos];
        p->pos++;
        return;
    }
    
    p->current.type = TOK_INVALID;
}

int parse_expression(struct Parser *p);

int parse_factor(struct Parser *p) {
    if (p == NULL) {
        fprintf(stderr, "Error: Null parser\n");
        exit(EXIT_FAILURE);
    }
    
    if (p->current.type == TOK_NUM) {
        int val = p->current.value;
        next_token(p);
        return val;
    } else if (p->current.type == TOK_OP && p->current.op == '(') {
        next_token(p);
        int result = parse_expression(p);
        if (p->current.type != TOK_OP || p->current.op != ')') {
            fprintf(stderr, "Error: Expected ')'\n");
            exit(EXIT_FAILURE);
        }
        next_token(p);
        return result;
    } else {
        fprintf(stderr, "Error: Expected number or '('\n");
        exit(EXIT_FAILURE);
    }
}

int parse_term(struct Parser *p) {
    if (p == NULL) {
        fprintf(stderr, "Error: Null parser\n");
        exit(EXIT_FAILURE);
    }
    
    int left = parse_factor(p);
    
    while (p->current.type == TOK_OP && (p->current.op == '*' || p->current.op == '/')) {
        char op = p->current.op;
        next_token(p);
        int right = parse_factor(p);
        
        if (op == '*') {
            if ((left > 0 && right > 0 && left > INT_MAX / right) ||
                (left < 0 && right < 0 && left < INT_MAX / right) ||
                (left > 0 && right < 0 && right < INT_MIN / left) ||
                (left < 0 && right > 0 && left < INT_MIN / right)) {
                fprintf(stderr, "Error: Integer overflow\n");
                exit(EXIT_FAILURE);
            }
            left *= right;
        } else {
            if (right == 0) {
                fprintf(stderr, "Error: Division by zero\n");
                exit(EXIT_FAILURE);
            }
            if (left == INT_MIN && right == -1) {
                fprintf(stderr, "Error: Integer overflow\n");
                exit(EXIT_FAILURE);
            }
            left /= right;
        }
    }
    
    return left;
}

int parse_expression(struct Parser *p) {
    if (p == NULL) {
        fprintf(stderr, "Error: Null parser\n");
        exit(EXIT_FAILURE);
    }
    
    int left = parse_term(p);
    
    while (p->current.type == TOK_OP && (p->current.op == '+' || p->current.op == '-')) {
        char op = p->current.op;
        next_token(p);
        int right = parse_term(p);
        
        if (op == '+') {
            if ((right > 0 && left > INT_MAX - right) ||
                (right < 0 && left < INT_MIN - right)) {
                fprintf(stderr, "Error: Integer overflow\n");
                exit(EXIT_FAILURE);
            }
            left += right;
        } else {
            if ((right > 0 && left < INT_MIN + right) ||
                (right < 0 && left > INT_MAX + right)) {
                fprintf(stderr, "Error: Integer overflow\n");
                exit(EXIT_FAILURE);
            }
            left -= right;
        }
    }
    
    return left;
}

int main(void) {
    char buffer[256];
    
    printf("Enter arithmetic expression: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(buffer