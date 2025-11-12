//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

void next_token(struct Parser *p) {
    while (isspace(p->input[p->pos])) {
        p->pos++;
    }
    
    char c = p->input[p->pos];
    if (c == '\0') {
        p->current.type = TOKEN_END;
        return;
    }
    
    if (isdigit(c)) {
        int val = 0;
        while (isdigit(p->input[p->pos])) {
            val = val * 10 + (p->input[p->pos] - '0');
            p->pos++;
        }
        p->current.type = TOKEN_NUMBER;
        p->current.value = val;
        return;
    }
    
    if (is_operator(c)) {
        p->current.type = TOKEN_OPERATOR;
        p->current.op = c;
        p->pos++;
        return;
    }
    
    if (c == '(' || c == ')') {
        p->current.type = TOKEN_PAREN;
        p->current.op = c;
        p->pos++;
        return;
    }
    
    p->current.type = TOKEN_END;
}

void init_parser(struct Parser *p, const char *input) {
    p->input = input;
    p->pos = 0;
    next_token(p);
}

int parse_expression(struct Parser *p);

int parse_factor(struct Parser *p) {
    if (p->current.type == TOKEN_NUMBER) {
        int val = p->current.value;
        next_token(p);
        return val;
    }
    
    if (p->current.type == TOKEN_PAREN && p->current.op == '(') {
        next_token(p);
        int result = parse_expression(p);
        if (p->current.type != TOKEN_PAREN || p->current.op != ')') {
            fprintf(stderr, "Error: Expected ')'\n");
            exit(1);
        }
        next_token(p);
        return result;
    }
    
    fprintf(stderr, "Error: Expected number or '('\n");
    exit(1);
}

int parse_term(struct Parser *p) {
    int left = parse_factor(p);
    
    while (p->current.type == TOKEN_OPERATOR && 
           (p->current.op == '*' || p->current.op == '/')) {
        char op = p->current.op;
        next_token(p);
        int right = parse_factor(p);
        
        if (op == '*') {
            left *= right;
        } else {
            if (right == 0) {
                fprintf(stderr, "Error: Division by zero\n");
                exit(1);
            }
            left /= right;
        }
    }
    
    return left;
}

int parse_expression(struct Parser *p) {
    int left = parse_term(p);
    
    while (p->current.type == TOKEN_OPERATOR && 
           (p->current.op == '+' || p->current.op == '-')) {
        char op = p->current.op;
        next_token(p);
        int right = parse_term(p);
        
        if (op == '+') {
            left += right;
        } else {
            left -= right;
        }
    }
    
    return left;
}

int evaluate_expression(const char *input) {
    struct Parser parser;
    init_parser(&parser, input);
    int result = parse_expression(&parser);
    
    if (parser.current.type != TOKEN_END) {
        fprintf(stderr, "Error: Unexpected token\n");
        exit(1);
    }
    
    return result;
}

int main() {
    char input[256];
    
    printf("Enter arithmetic expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Error: Empty input\n");
        return 1;
    }
    
    int result = evaluate_expression(input);
    printf("Result: %d\n", result);
    
    return 0;
}