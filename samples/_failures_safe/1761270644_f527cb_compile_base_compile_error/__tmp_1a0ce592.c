//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

enum TokenType { TOK_NUMBER, TOK_IDENT, TOK_PLUS, TOK_MINUS, TOK_MUL, TOK_DIV, TOK_LPAREN, TOK_RPAREN, TOK_END, TOK_ERROR };

struct Token {
    enum TokenType type;
    int value;
    char name[32];
};

struct ParserState {
    const char *input;
    int pos;
    struct Token current;
};

void next_token(struct ParserState *state) {
    if (state == NULL || state->input == NULL) return;
    
    while (state->input[state->pos] != '\0' && isspace((unsigned char)state->input[state->pos])) state->pos++;
    
    char c = state->input[state->pos];
    if (c == '\0') {
        state->current.type = TOK_END;
        return;
    }
    
    if (isdigit((unsigned char)c)) {
        int val = 0;
        while (state->input[state->pos] != '\0' && isdigit((unsigned char)state->input[state->pos])) {
            int digit = state->input[state->pos] - '0';
            if (val > (INT_MAX - digit) / 10) {
                state->current.type = TOK_ERROR;
                return;
            }
            val = val * 10 + digit;
            state->pos++;
        }
        state->current.type = TOK_NUMBER;
        state->current.value = val;
        return;
    }
    
    if (isalpha((unsigned char)c)) {
        int i = 0;
        while (state->input[state->pos] != '\0' && isalnum((unsigned char)state->input[state->pos]) && i < 31) {
            state->current.name[i++] = state->input[state->pos++];
        }
        state->current.name[i] = '\0';
        state->current.type = TOK_IDENT;
        return;
    }
    
    switch (c) {
        case '+': state->current.type = TOK_PLUS; break;
        case '-': state->current.type = TOK_MINUS; break;
        case '*': state->current.type = TOK_MUL; break;
        case '/': state->current.type = TOK_DIV; break;
        case '(': state->current.type = TOK_LPAREN; break;
        case ')': state->current.type = TOK_RPAREN; break;
        default: state->current.type = TOK_ERROR; break;
    }
    
    if (state->current.type != TOK_ERROR) state->pos++;
}

int expect(struct ParserState *state, enum TokenType expected) {
    if (state == NULL) return 0;
    if (state->current.type == expected) {
        next_token(state);
        return 1;
    }
    return 0;
}

int parse_expression(struct ParserState *state);

int parse_factor(struct ParserState *state) {
    if (state == NULL) {
        printf("Null parser state\n");
        exit(1);
    }
    
    if (state->current.type == TOK_NUMBER) {
        int val = state->current.value;
        next_token(state);
        return val;
    }
    
    if (state->current.type == TOK_IDENT) {
        printf("Variable '%s' = ", state->current.name);
        int val;
        if (scanf("%d", &val) != 1) {
            printf("Invalid input for variable\n");
            exit(1);
        }
        next_token(state);
        return val;
    }
    
    if (expect(state, TOK_LPAREN)) {
        int val = parse_expression(state);
        if (!expect(state, TOK_RPAREN)) {
            printf("Missing closing parenthesis\n");
            exit(1);
        }
        return val;
    }
    
    printf("Unexpected token in factor\n");
    exit(1);
}

int parse_term(struct ParserState *state) {
    if (state == NULL) {
        printf("Null parser state\n");
        exit(1);
    }
    
    int left = parse_factor(state);
    
    while (state->current.type == TOK_MUL || state->current.type == TOK_DIV) {
        enum TokenType op = state->current.type;
        next_token(state);
        int right = parse_factor(state);
        
        if (op == TOK_MUL) {
            if (left != 0 && right != 0) {
                if ((left > 0 && right > 0 && left > INT_MAX / right) ||
                    (left > 0 && right < 0 && right < INT_MIN / left) ||
                    (left < 0 && right > 0 && left < INT_MIN / right) ||
                    (left < 0 && right < 0 && left < INT_MAX / right)) {
                    printf("Integer overflow in multiplication\n");
                    exit(1);
                }
            }
            left *= right;
        } else {
            if (right == 0) {
                printf("Division by zero\n");
                exit(1);
            }
            if (left == INT_MIN && right == -1) {
                printf("Integer overflow in division\n");
                exit(1);
            }
            left /= right;
        }
    }
    
    return left;
}

int parse_expression(struct ParserState *state) {
    if (state == NULL)