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
    return c == '+' || c == '-' || c == '*' || c == '/';
}

struct Token get_next_token(struct Tokenizer *t) {
    struct Token tok = {TOK_INVALID, 0, 0};
    
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
    } else {
        result = -1;
    }
    
    return result;
}

int parse_term(struct Tokenizer *t, struct Token *lookahead) {
    int result = parse_factor(t, lookahead);
    if (result == -1) {
        return -1;
    }
    
    while (lookahead->type == TOK_OP && (lookahead->op == '*' || lookahead->op == '/')) {
        char op = lookahead->op;
        *lookahead = get_next_token(t);
        
        int right = parse_factor(t, lookahead);
        if (right == -1) {
            return -1;
        }
        
        if (op == '*') {
            if (result > 0 && right > INT_MAX / result) return -1;
            if (result < 0 && right < INT_MIN / result) return -1;
            result *= right;
        } else {
            if (right == 0) return -1;
            if (result == INT_MIN && right == -1) return -1;
            result /= right;
        }
    }
    
    return result;
}

int parse_expression(struct Tokenizer *t, struct Token *lookahead) {
    int result = parse_term(t, lookahead);
    if (result == -1) {
        return -1;
    }
    
    while (lookahead->type == TOK_OP && (lookahead->op == '+' || lookahead->op == '-')) {
        char op = lookahead->op;
        *lookahead = get_next_token(t);
        
        int right = parse_term(t, lookahead);
        if (right == -1) {
            return -1;
        }
        
        if (op == '+') {
            if (right > 0 && result > INT_MAX - right) return -1;
            if (right < 0 && result < INT_MIN - right) return -1;
            result += right;
        } else {
            if (right > 0 && result < INT_MIN + right) return -1;
            if (right < 0 && result > INT_MAX + right) return -1;
            result -= right;
        }
    }
    
    return result;
}

int main(void) {
    char input[256];
    
    printf("Enter arithmetic expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    struct Tokenizer tokenizer;
    init_tokenizer(&tokenizer, input);
    
    struct Token lookahead = get_next_token(&tokenizer);
    if (lookahead.type == TOK_INVALID) {
        fprintf(stderr, "Invalid token\n");
        return 1