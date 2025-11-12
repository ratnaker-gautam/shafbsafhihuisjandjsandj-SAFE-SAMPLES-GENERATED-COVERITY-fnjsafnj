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
    TOKEN_MUL,
    TOKEN_DIV,
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
    
    char c = lexer->input[lexer->pos];
    
    if (isdigit(c)) {
        int value = 0;
        while (lexer->pos < lexer->len && isdigit(lexer->input[lexer->pos])) {
            if (value > (INT_MAX - (lexer->input[lexer->pos] - '0')) / 10) {
                return (Token){TOKEN_INVALID, 0};
            }
            value = value * 10 + (lexer->input[lexer->pos] - '0');
            lexer->pos++;
        }
        return (Token){TOKEN_NUMBER, value};
    }
    
    lexer->pos++;
    
    switch (c) {
        case '+': return (Token){TOKEN_PLUS, 0};
        case '-': return (Token){TOKEN_MINUS, 0};
        case '*': return (Token){TOKEN_MUL, 0};
        case '/': return (Token){TOKEN_DIV, 0};
        case '(': return (Token){TOKEN_LPAREN, 0};
        case ')': return (Token){TOKEN_RPAREN, 0};
        default: return (Token){TOKEN_INVALID, 0};
    }
}

int parse_expression(Lexer* lexer, Token* current, int* error);
int parse_term(Lexer* lexer, Token* current, int* error);
int parse_factor(Lexer* lexer, Token* current, int* error);

int parse_factor(Lexer* lexer, Token* current, int* error) {
    if (lexer == NULL || current == NULL || error == NULL) {
        if (error != NULL) *error = 1;
        return 0;
    }
    
    if (*error) return 0;
    
    if (current->type == TOKEN_NUMBER) {
        int value = current->value;
        *current = lexer_next(lexer);
        return value;
    }
    
    if (current->type == TOKEN_LPAREN) {
        *current = lexer_next(lexer);
        int result = parse_expression(lexer, current, error);
        if (*error) return 0;
        
        if (current->type != TOKEN_RPAREN) {
            *error = 1;
            return 0;
        }
        *current = lexer_next(lexer);
        return result;
    }
    
    *error = 1;
    return 0;
}

int parse_term(Lexer* lexer, Token* current, int* error) {
    if (lexer == NULL || current == NULL || error == NULL) {
        if (error != NULL) *error = 1;
        return 0;
    }
    
    if (*error) return 0;
    
    int left = parse_factor(lexer, current, error);
    if (*error) return 0;
    
    while (current->type == TOKEN_MUL || current->type == TOKEN_DIV) {
        TokenType op = current->type;
        *current = lexer_next(lexer);
        
        int right = parse_factor(lexer, current, error);
        if (*error) return 0;
        
        if (op == TOKEN_MUL) {
            if (left != 0 && right > INT_MAX / left) {
                *error = 1;
                return 0;
            }
            left *= right;
        } else {
            if (right == 0) {
                *error = 1;
                return 0;
            }
            if (left == INT_MIN && right == -1) {
                *error = 1;
                return 0;
            }
            left /= right;
        }
    }
    
    return left;
}

int parse_expression(Lexer* lexer, Token* current, int* error) {
    if (lexer == NULL || current == NULL || error == NULL) {
        if (error != NULL) *error = 1;
        return 0;
    }
    
    if (*error) return 0;
    
    int left = parse_term(lexer, current, error);
    if (*error) return 0;
    
    while (current->type == TOKEN_PLUS || current->type == TOKEN_MINUS) {
        TokenType op = current->type;
        *current = lexer_next(lexer);
        
        int right = parse_term