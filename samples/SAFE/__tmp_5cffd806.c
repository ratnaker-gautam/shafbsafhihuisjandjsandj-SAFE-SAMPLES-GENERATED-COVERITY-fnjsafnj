//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

typedef enum {
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_EOF,
    TOKEN_INVALID
} TokenType;

typedef struct {
    TokenType type;
    int value;
} Token;

typedef struct {
    const char* input;
    size_t pos;
    size_t len;
} Lexer;

Token lexer_next(Lexer* lexer) {
    while (lexer->pos < lexer->len && isspace(lexer->input[lexer->pos])) {
        lexer->pos++;
    }
    
    if (lexer->pos >= lexer->len) {
        return (Token){TOKEN_EOF, 0};
    }
    
    char current = lexer->input[lexer->pos];
    
    if (isdigit(current)) {
        int value = 0;
        while (lexer->pos < lexer->len && isdigit(lexer->input[lexer->pos])) {
            int digit = lexer->input[lexer->pos] - '0';
            if (value > (INT_MAX - digit) / 10) {
                return (Token){TOKEN_INVALID, 0};
            }
            value = value * 10 + digit;
            lexer->pos++;
        }
        return (Token){TOKEN_NUMBER, value};
    }
    
    lexer->pos++;
    
    switch (current) {
        case '+': return (Token){TOKEN_PLUS, 0};
        case '-': return (Token){TOKEN_MINUS, 0};
        case '*': return (Token){TOKEN_MULTIPLY, 0};
        case '/': return (Token){TOKEN_DIVIDE, 0};
        case '(': return (Token){TOKEN_LPAREN, 0};
        case ')': return (Token){TOKEN_RPAREN, 0};
        default: return (Token){TOKEN_INVALID, 0};
    }
}

Token lexer_peek(Lexer* lexer) {
    size_t saved_pos = lexer->pos;
    Token token = lexer_next(lexer);
    lexer->pos = saved_pos;
    return token;
}

int parse_expression(Lexer* lexer);
int parse_term(Lexer* lexer);
int parse_factor(Lexer* lexer);

int parse_factor(Lexer* lexer) {
    Token token = lexer_next(lexer);
    
    if (token.type == TOKEN_NUMBER) {
        return token.value;
    }
    
    if (token.type == TOKEN_LPAREN) {
        int result = parse_expression(lexer);
        token = lexer_next(lexer);
        if (token.type != TOKEN_RPAREN) {
            return 0;
        }
        return result;
    }
    
    if (token.type == TOKEN_MINUS) {
        int factor = parse_factor(lexer);
        if (factor == INT_MIN) {
            return 0;
        }
        return -factor;
    }
    
    return 0;
}

int parse_term(Lexer* lexer) {
    int result = parse_factor(lexer);
    
    while (1) {
        Token token = lexer_peek(lexer);
        
        if (token.type == TOKEN_MULTIPLY) {
            lexer_next(lexer);
            int right = parse_factor(lexer);
            if (result != 0 && right != 0) {
                if ((result > 0 && right > 0 && result > INT_MAX / right) ||
                    (result < 0 && right < 0 && result < INT_MAX / right) ||
                    (result > 0 && right < 0 && right < INT_MIN / result) ||
                    (result < 0 && right > 0 && result < INT_MIN / right)) {
                    return 0;
                }
            }
            result *= right;
        } else if (token.type == TOKEN_DIVIDE) {
            lexer_next(lexer);
            int right = parse_factor(lexer);
            if (right == 0) {
                return 0;
            }
            if (result == INT_MIN && right == -1) {
                return 0;
            }
            result /= right;
        } else {
            break;
        }
    }
    
    return result;
}

int parse_expression(Lexer* lexer) {
    int result = parse_term(lexer);
    
    while (1) {
        Token token = lexer_peek(lexer);
        
        if (token.type == TOKEN_PLUS) {
            lexer_next(lexer);
            int right = parse_term(lexer);
            if (result > 0 && right > INT_MAX - result) {
                return 0;
            }
            if (result < 0 && right < INT_MIN - result) {
                return 0;
            }
            result += right;
        } else if (token.type == TOKEN_MINUS) {
            lexer_next(lexer);
            int right = parse_term(lexer);
            if (result < 0 && right > 0 && result < INT_MIN + right) {
                return 0;
            }
            if (result > 0 && right < 0 && result > INT_MAX + right) {
                return