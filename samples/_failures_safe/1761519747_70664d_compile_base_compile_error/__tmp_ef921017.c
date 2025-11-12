//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

enum TokenType { TOKEN_NUMBER, TOKEN_OPERATOR, TOKEN_PAREN, TOKEN_END };

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
    if (p == NULL || p->input == NULL) {
        fprintf(stderr, "Invalid parser state\n");
        exit(1);
    }
    
    while (p->input[p->pos] != '\0' && isspace((unsigned char)p->input[p->pos])) {
        p->pos++;
    }
    
    if (p->input[p->pos] == '\0') {
        p->current.type = TOKEN_END;
        return;
    }
    
    if (isdigit((unsigned char)p->input[p->pos])) {
        int val = 0;
        while (p->input[p->pos] != '\0' && isdigit((unsigned char)p->input[p->pos])) {
            int digit = p->input[p->pos] - '0';
            if (val > (INT_MAX - digit) / 10) {
                fprintf(stderr, "Number too large\n");
                exit(1);
            }
            val = val * 10 + digit;
            p->pos++;
        }
        p->current.type = TOKEN_NUMBER;
        p->current.value = val;
        return;
    }
    
    if (strchr("+-*/", p->input[p->pos]) != NULL) {
        p->current.type = TOKEN_OPERATOR;
        p->current.op = p->input[p->pos];
        p->pos++;
        return;
    }
    
    if (p->input[p->pos] == '(' || p->input[p->pos] == ')') {
        p->current.type = TOKEN_PAREN;
        p->current.op = p->input[p->pos];
        p->pos++;
        return;
    }
    
    fprintf(stderr, "Invalid character: %c\n", p->input[p->pos]);
    exit(1);
}

int parse_expression(struct Parser *p);

int parse_factor(struct Parser *p) {
    if (p == NULL) {
        fprintf(stderr, "Invalid parser\n");
        exit(1);
    }
    
    if (p->current.type == TOKEN_NUMBER) {
        int val = p->current.value;
        next_token(p);
        return val;
    }
    
    if (p->current.type == TOKEN_PAREN && p->current.op == '(') {
        next_token(p);
        int val = parse_expression(p);
        if (p->current.type != TOKEN_PAREN || p->current.op != ')') {
            fprintf(stderr, "Expected closing parenthesis\n");
            exit(1);
        }
        next_token(p);
        return val;
    }
    
    fprintf(stderr, "Expected number or parenthesis\n");
    exit(1);
}

int parse_term(struct Parser *p) {
    if (p == NULL) {
        fprintf(stderr, "Invalid parser\n");
        exit(1);
    }
    
    int left = parse_factor(p);
    
    while (p->current.type == TOKEN_OPERATOR && 
           (p->current.op == '*' || p->current.op == '/')) {
        char op = p->current.op;
        next_token(p);
        int right = parse_factor(p);
        
        if (op == '*') {
            if ((left > 0 && right > 0 && left > INT_MAX / right) ||
                (left < 0 && right < 0 && left < INT_MAX / right) ||
                (left > 0 && right < 0 && right < INT_MIN / left) ||
                (left < 0 && right > 0 && left < INT_MIN / right)) {
                fprintf(stderr, "Multiplication overflow\n");
                exit(1);
            }
            left *= right;
        } else {
            if (right == 0) {
                fprintf(stderr, "Division by zero\n");
                exit(1);
            }
            if (left == INT_MIN && right == -1) {
                fprintf(stderr, "Division overflow\n");
                exit(1);
            }
            left /= right;
        }
    }
    
    return left;
}

int parse_expression(struct Parser *p) {
    if (p == NULL) {
        fprintf(stderr, "Invalid parser\n");
        exit(1);
    }
    
    int left = parse_term(p);
    
    while (p->current.type == TOKEN_OPERATOR && 
           (p->current.op == '+' || p->current.op == '-')) {
        char op = p->current.op;
        next_token(p);
        int right = parse_term(p);
        
        if (op == '+') {
            if ((right > 0 && left > INT_MAX - right) ||
                (right < 0 && left < INT_MIN - right)) {
                fprintf(stderr, "Addition overflow\n");
                exit(1);
            }
            left += right;
        } else {
            if ((right > 0 && left < INT_MIN + right) ||
                (right < 0 && left > INT_MAX + right)) {
                fprintf(stderr, "Subtraction overflow\n");
                exit(1);
            }
            left -= right;
        }
    }
    
    return left;
}

int main(void) {
    char input[256];
    
    printf