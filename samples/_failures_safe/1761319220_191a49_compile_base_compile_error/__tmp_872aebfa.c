//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

enum TokenType { TOK_NUMBER, TOK_OPERATOR, TOK_LPAREN, TOK_RPAREN, TOK_END, TOK_ERROR };

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

struct Token get_token(struct Tokenizer *t) {
    struct Token tok = {TOK_ERROR, 0, 0};
    
    if (t->input == NULL) {
        return tok;
    }
    
    while (isspace((unsigned char)t->input[t->pos])) {
        t->pos++;
    }
    
    if (t->input[t->pos] == '\0') {
        tok.type = TOK_END;
        return tok;
    }
    
    if (isdigit((unsigned char)t->input[t->pos])) {
        int val = 0;
        while (isdigit((unsigned char)t->input[t->pos])) {
            if (val > (INT_MAX - (t->input[t->pos] - '0')) / 10) {
                tok.type = TOK_ERROR;
                return tok;
            }
            val = val * 10 + (t->input[t->pos] - '0');
            t->pos++;
        }
        tok.type = TOK_NUMBER;
        tok.value = val;
        return tok;
    }
    
    if (t->input[t->pos] == '+' || t->input[t->pos] == '-' || 
        t->input[t->pos] == '*' || t->input[t->pos] == '/') {
        tok.type = TOK_OPERATOR;
        tok.op = t->input[t->pos];
        t->pos++;
        return tok;
    }
    
    if (t->input[t->pos] == '(') {
        tok.type = TOK_LPAREN;
        t->pos++;
        return tok;
    }
    
    if (t->input[t->pos] == ')') {
        tok.type = TOK_RPAREN;
        t->pos++;
        return tok;
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
        *lookahead = get_token(t);
    } else if (lookahead->type == TOK_LPAREN) {
        *lookahead = get_token(t);
        result = parse_expression(t, lookahead);
        if (lookahead->type != TOK_RPAREN) {
            fprintf(stderr, "Error: Expected ')'\n");
            exit(1);
        }
        *lookahead = get_token(t);
    } else {
        fprintf(stderr, "Error: Expected number or '('\n");
        exit(1);
    }
    
    return result;
}

int parse_term(struct Tokenizer *t, struct Token *lookahead) {
    int result = parse_factor(t, lookahead);
    
    while (lookahead->type == TOK_OPERATOR && 
           (lookahead->op == '*' || lookahead->op == '/')) {
        char op = lookahead->op;
        *lookahead = get_token(t);
        int right = parse_factor(t, lookahead);
        
        if (op == '*') {
            if (result != 0 && right > INT_MAX / result) {
                fprintf(stderr, "Error: Integer overflow\n");
                exit(1);
            }
            if (result != 0 && right < INT_MIN / result) {
                fprintf(stderr, "Error: Integer overflow\n");
                exit(1);
            }
            result *= right;
        } else if (op == '/') {
            if (right == 0) {
                fprintf(stderr, "Error: Division by zero\n");
                exit(1);
            }
            result /= right;
        }
    }
    
    return result;
}

int parse_expression(struct Tokenizer *t, struct Token *lookahead) {
    int result = parse_term(t, lookahead);
    
    while (lookahead->type == TOK_OPERATOR && 
           (lookahead->op == '+' || lookahead->op == '-')) {
        char op = lookahead->op;
        *lookahead = get_token(t);
        int right = parse_term(t, lookahead);
        
        if (op == '+') {
            if (right > 0 && result > INT_MAX - right) {
                fprintf(stderr, "Error: Integer overflow\n");
                exit(1);
            }
            if (right < 0 && result < INT_MIN - right) {
                fprintf(stderr, "Error: Integer overflow\n");
                exit(1);
            }
            result += right;
        } else if (op == '-') {
            if (right < 0 && result > INT_MAX + right) {
                fprintf(stderr, "Error: Integer overflow\n");
                exit(1);
            }