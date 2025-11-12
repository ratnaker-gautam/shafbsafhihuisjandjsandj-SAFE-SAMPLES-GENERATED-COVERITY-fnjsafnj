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

struct Tokenizer {
    const char *input;
    int pos;
};

void init_tokenizer(struct Tokenizer *t, const char *str) {
    t->input = str;
    t->pos = 0;
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')';
}

struct Token get_next_token(struct Tokenizer *t) {
    struct Token tok = {TOK_INVALID, 0, 0};
    
    if (t->input == NULL) {
        return tok;
    }
    
    while (t->input[t->pos] && isspace(t->input[t->pos])) {
        t->pos++;
    }
    
    if (t->input[t->pos] == '\0') {
        tok.type = TOK_EOF;
        return tok;
    }
    
    if (isdigit(t->input[t->pos])) {
        int val = 0;
        while (t->input[t->pos] && isdigit(t->input[t->pos])) {
            if (val > (INT_MAX - (t->input[t->pos] - '0')) / 10) {
                tok.type = TOK_INVALID;
                return tok;
            }
            val = val * 10 + (t->input[t->pos] - '0');
            t->pos++;
        }
        tok.type = TOK_NUM;
        tok.value = val;
        return tok;
    }
    
    if (is_operator(t->input[t->pos])) {
        tok.type = TOK_OP;
        tok.op = t->input[t->pos];
        t->pos++;
        return tok;
    }
    
    tok.type = TOK_INVALID;
    return tok;
}

int parse_expression(struct Tokenizer *t, struct Token *lookahead);
int parse_term(struct Tokenizer *t, struct Token *lookahead);
int parse_factor(struct Tokenizer *t, struct Token *lookahead);

int parse_factor(struct Tokenizer *t, struct Token *lookahead) {
    int result = 0;
    
    if (lookahead->type == TOK_NUM) {
        result = lookahead->value;
        *lookahead = get_next_token(t);
    } else if (lookahead->type == TOK_OP && lookahead->op == '(') {
        *lookahead = get_next_token(t);
        result = parse_expression(t, lookahead);
        if (lookahead->type != TOK_OP || lookahead->op != ')') {
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
    
    while (lookahead->type == TOK_OP && (lookahead->op == '*' || lookahead->op == '/')) {
        char op = lookahead->op;
        *lookahead = get_next_token(t);
        int right = parse_factor(t, lookahead);
        
        if (op == '*') {
            if (result > 0 && right > INT_MAX / result) return 0;
            if (result < 0 && right < INT_MIN / result) return 0;
            result *= right;
        } else {
            if (right == 0) return 0;
            if (result == INT_MIN && right == -1) return 0;
            result /= right;
        }
    }
    
    return result;
}

int parse_expression(struct Tokenizer *t, struct Token *lookahead) {
    int result = parse_term(t, lookahead);
    
    while (lookahead->type == TOK_OP && (lookahead->op == '+' || lookahead->op == '-')) {
        char op = lookahead->op;
        *lookahead = get_next_token(t);
        int right = parse_term(t, lookahead);
        
        if (op == '+') {
            if (right > 0 && result > INT_MAX - right) return 0;
            if (right < 0 && result < INT_MIN - right) return 0;
            result += right;
        } else {
            if (right > 0 && result < INT_MIN + right) return 0;
            if (right < 0 && result > INT_MAX + right) return 0;
            result -= right;
        }
    }
    
    return result;
}

int evaluate_expression(const char *expr) {
    if (expr == NULL) {
        return 0;
    }
    
    struct Tokenizer tokenizer;
    init_tokenizer(&tokenizer, expr);
    
    struct Token lookahead = get_next_token(&tokenizer);
    if (lookahead.type == TOK_INVALID) {
        return 0;
    }
    
    int result = parse_expression(&tokenizer, &lookahead);
    
    if (lookahead.type != TOK_EOF) {
        return 0;
    }
    
    return result;
}

int main(void) {
    char input[256];
    
    printf