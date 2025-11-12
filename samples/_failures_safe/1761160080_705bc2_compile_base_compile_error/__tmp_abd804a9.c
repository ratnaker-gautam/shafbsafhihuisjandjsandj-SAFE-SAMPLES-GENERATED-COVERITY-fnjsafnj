//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOK_NUMBER, TOK_OPERATOR, TOK_LPAREN, TOK_RPAREN, TOK_EOF, TOK_ERROR };

struct Token {
    enum TokenType type;
    int value;
    char op;
};

struct Tokenizer {
    const char *input;
    int pos;
};

void init_tokenizer(struct Tokenizer *t, const char *str) {
    t->input = str;
    t->pos = 0;
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

struct Token get_next_token(struct Tokenizer *t) {
    struct Token tok = {TOK_ERROR, 0, 0};
    
    while (t->input[t->pos] && isspace(t->input[t->pos])) {
        t->pos++;
    }
    
    if (t->input[t->pos] == '\0') {
        tok.type = TOK_EOF;
        return tok;
    }
    
    char current = t->input[t->pos];
    
    if (isdigit(current)) {
        int num = 0;
        while (t->input[t->pos] && isdigit(t->input[t->pos])) {
            if (num > (2147483647 - (t->input[t->pos] - '0')) / 10) {
                tok.type = TOK_ERROR;
                return tok;
            }
            num = num * 10 + (t->input[t->pos] - '0');
            t->pos++;
        }
        tok.type = TOK_NUMBER;
        tok.value = num;
    } else if (is_operator(current)) {
        tok.type = TOK_OPERATOR;
        tok.op = current;
        t->pos++;
    } else if (current == '(') {
        tok.type = TOK_LPAREN;
        t->pos++;
    } else if (current == ')') {
        tok.type = TOK_RPAREN;
        t->pos++;
    } else {
        tok.type = TOK_ERROR;
    }
    
    return tok;
}

int parse_expression(struct Tokenizer *t, struct Token *lookahead);
int parse_term(struct Tokenizer *t, struct Token *lookahead);
int parse_factor(struct Tokenizer *t, struct Token *lookahead);

int parse_factor(struct Tokenizer *t, struct Token *lookahead) {
    int result = 0;
    
    if (lookahead->type == TOK_NUMBER) {
        result = lookahead->value;
        *lookahead = get_next_token(t);
    } else if (lookahead->type == TOK_LPAREN) {
        *lookahead = get_next_token(t);
        result = parse_expression(t, lookahead);
        if (lookahead->type != TOK_RPAREN) {
            return 0;
        }
        *lookahead = get_next_token(t);
    } else {
        return 0;
    }
    
    return result;
}

int parse_term(struct Tokenizer *t, struct Token *lookahead) {
    int result = parse_factor(t, lookahead);
    
    while (lookahead->type == TOK_OPERATOR && (lookahead->op == '*' || lookahead->op == '/')) {
        char op = lookahead->op;
        *lookahead = get_next_token(t);
        int right = parse_factor(t, lookahead);
        
        if (op == '*') {
            if (result > 0 && right > 0 && result > 2147483647 / right) {
                return 0;
            }
            result *= right;
        } else if (op == '/') {
            if (right == 0) {
                return 0;
            }
            result /= right;
        }
    }
    
    return result;
}

int parse_expression(struct Tokenizer *t, struct Token *lookahead) {
    int result = parse_term(t, lookahead);
    
    while (lookahead->type == TOK_OPERATOR && (lookahead->op == '+' || lookahead->op == '-')) {
        char op = lookahead->op;
        *lookahead = get_next_token(t);
        int right = parse_term(t, lookahead);
        
        if (op == '+') {
            if (result > 0 && right > 0 && result > 2147483647 - right) {
                return 0;
            }
            result += right;
        } else if (op == '-') {
            if (result < 0 && right > 0 && result < -2147483647 + right) {
                return 0;
            }
            result -= right;
        }
    }
    
    return result;
}

int evaluate_expression(const char *expr) {
    struct Tokenizer tokenizer;
    init_tokenizer(&tokenizer, expr);
    
    struct Token lookahead = get_next_token(&tokenizer);
    if (lookahead.type == TOK_ERROR) {
        return 0;
    }
    
    int result = parse_expression(&tokenizer, &lookahead);
    
    if (lookahead.type != TOK_EOF) {
        return 0;
    }
    
    return result;
}

int main() {
    char input[256];
    
    printf("Enter arithmetic expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;