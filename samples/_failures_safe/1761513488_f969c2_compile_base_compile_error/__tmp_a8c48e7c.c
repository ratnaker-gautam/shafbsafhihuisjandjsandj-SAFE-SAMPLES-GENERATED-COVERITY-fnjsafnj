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
    if (lexer == NULL || lexer->input == NULL) {
        return (Token){TOKEN_INVALID, 0};
    }
    
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

int parse_expression(Lexer* lexer, Token* current);
int parse_term(Lexer* lexer, Token* current);
int parse_factor(Lexer* lexer, Token* current);

int parse_factor(Lexer* lexer, Token* current) {
    if (lexer == NULL || current == NULL) {
        return 0;
    }
    
    if (current->type == TOKEN_NUMBER) {
        int value = current->value;
        *current = lexer_next(lexer);
        return value;
    } else if (current->type == TOKEN_LPAREN) {
        *current = lexer_next(lexer);
        int result = parse_expression(lexer, current);
        if (current->type != TOKEN_RPAREN) {
            return 0;
        }
        *current = lexer_next(lexer);
        return result;
    } else {
        return 0;
    }
}

int parse_term(Lexer* lexer, Token* current) {
    if (lexer == NULL || current == NULL) {
        return 0;
    }
    
    int result = parse_factor(lexer, current);
    
    while (current->type == TOKEN_MULTIPLY || current->type == TOKEN_DIVIDE) {
        TokenType op = current->type;
        *current = lexer_next(lexer);
        int right = parse_factor(lexer, current);
        
        if (op == TOKEN_MULTIPLY) {
            if (result > 0 && right > 0 && result > INT_MAX / right) {
                return 0;
            }
            if (result < 0 && right < 0 && result < INT_MAX / right) {
                return 0;
            }
            if (result > 0 && right < 0 && right < INT_MIN / result) {
                return 0;
            }
            if (result < 0 && right > 0 && result < INT_MIN / right) {
                return 0;
            }
            result *= right;
        } else {
            if (right == 0) {
                return 0;
            }
            if (result == INT_MIN && right == -1) {
                return 0;
            }
            result /= right;
        }
    }
    
    return result;
}

int parse_expression(Lexer* lexer, Token* current) {
    if (lexer == NULL || current == NULL) {
        return 0;
    }
    
    int result = parse_term(lexer, current);
    
    while (current->type == TOKEN_PLUS || current->type == TOKEN_MINUS) {
        TokenType op = current->type;
        *current = lexer_next(lexer);
        int right = parse_term(lexer, current);
        
        if (op == TOKEN_PLUS) {
            if (result > 0 && right > INT_MAX - result) {
                return 0;
            }
            if (result < 0 && right < INT_MIN - result) {
                return 0;
            }
            result += right;
        } else {
            if (right > 0 && result < INT_MIN + right) {
                return 0