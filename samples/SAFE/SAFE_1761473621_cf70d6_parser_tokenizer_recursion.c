//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

void skip_whitespace(struct Tokenizer *t) {
    while (t->input[t->pos] && isspace(t->input[t->pos])) {
        t->pos++;
    }
}

struct Token get_next_token(struct Tokenizer *t) {
    struct Token tok = {TOK_INVALID, 0, 0};
    skip_whitespace(t);
    
    char current = t->input[t->pos];
    if (current == '\0') {
        tok.type = TOK_EOF;
        return tok;
    }
    
    if (isdigit(current)) {
        int val = 0;
        while (t->input[t->pos] && isdigit(t->input[t->pos])) {
            val = val * 10 + (t->input[t->pos] - '0');
            t->pos++;
        }
        tok.type = TOK_NUM;
        tok.value = val;
        return tok;
    }
    
    if (is_operator(current)) {
        tok.type = TOK_OP;
        tok.op = current;
        t->pos++;
        return tok;
    }
    
    tok.type = TOK_INVALID;
    return tok;
}

int parse_expression(struct Tokenizer *t, struct Token *lookahead);

int parse_factor(struct Tokenizer *t, struct Token *lookahead) {
    if (lookahead->type == TOK_NUM) {
        int value = lookahead->value;
        *lookahead = get_next_token(t);
        return value;
    } else if (lookahead->type == TOK_OP && lookahead->op == '(') {
        *lookahead = get_next_token(t);
        int result = parse_expression(t, lookahead);
        if (lookahead->type != TOK_OP || lookahead->op != ')') {
            fprintf(stderr, "Error: Expected ')'\n");
            exit(EXIT_FAILURE);
        }
        *lookahead = get_next_token(t);
        return result;
    } else {
        fprintf(stderr, "Error: Expected number or '('\n");
        exit(EXIT_FAILURE);
    }
}

int parse_term(struct Tokenizer *t, struct Token *lookahead) {
    int result = parse_factor(t, lookahead);
    
    while (lookahead->type == TOK_OP && (lookahead->op == '*' || lookahead->op == '/')) {
        char op = lookahead->op;
        *lookahead = get_next_token(t);
        int right = parse_factor(t, lookahead);
        
        if (op == '*') {
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

int parse_expression(struct Tokenizer *t, struct Token *lookahead) {
    int result = parse_term(t, lookahead);
    
    while (lookahead->type == TOK_OP && (lookahead->op == '+' || lookahead->op == '-')) {
        char op = lookahead->op;
        *lookahead = get_next_token(t);
        int right = parse_term(t, lookahead);
        
        if (op == '+') {
            result += right;
        } else {
            result -= right;
        }
    }
    
    return result;
}

int evaluate_expression(const char *expr) {
    struct Tokenizer tokenizer;
    init_tokenizer(&tokenizer, expr);
    
    struct Token lookahead = get_next_token(&tokenizer);
    int result = parse_expression(&tokenizer, &lookahead);
    
    if (lookahead.type != TOK_EOF) {
        fprintf(stderr, "Error: Unexpected token\n");
        exit(EXIT_FAILURE);
    }
    
    return result;
}

int main(void) {
    char buffer[256];
    
    printf("Enter arithmetic expressions (empty line to exit):\n");
    
    while (1) {
        printf("> ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (strlen(buffer) == 0) {
            break;
        }
        
        if (strlen(buffer) >= sizeof(buffer) - 1) {
            fprintf(stderr, "Error: Input too long\n");
            continue;
        }
        
        int result = evaluate_expression(buffer);
        printf("Result: %d\n", result);
    }
    
    return 0;
}