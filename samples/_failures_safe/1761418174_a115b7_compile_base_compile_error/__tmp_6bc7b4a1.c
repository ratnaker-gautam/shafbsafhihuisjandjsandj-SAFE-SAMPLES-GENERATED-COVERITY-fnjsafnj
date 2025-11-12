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
    Token token = {TOKEN_INVALID, 0};
    
    while (lexer->pos < lexer->len && isspace(lexer->input[lexer->pos])) {
        lexer->pos++;
    }
    
    if (lexer->pos >= lexer->len) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char current = lexer->input[lexer->pos];
    
    if (isdigit(current)) {
        int value = 0;
        while (lexer->pos < lexer->len && isdigit(lexer->input[lexer->pos])) {
            if (value > (INT_MAX - (lexer->input[lexer->pos] - '0')) / 10) {
                token.type = TOKEN_INVALID;
                return token;
            }
            value = value * 10 + (lexer->input[lexer->pos] - '0');
            lexer->pos++;
        }
        token.type = TOKEN_NUMBER;
        token.value = value;
    } else {
        lexer->pos++;
        switch (current) {
            case '+': token.type = TOKEN_PLUS; break;
            case '-': token.type = TOKEN_MINUS; break;
            case '*': token.type = TOKEN_MULTIPLY; break;
            case '/': token.type = TOKEN_DIVIDE; break;
            case '(': token.type = TOKEN_LPAREN; break;
            case ')': token.type = TOKEN_RPAREN; break;
            default: token.type = TOKEN_INVALID; break;
        }
    }
    
    return token;
}

int parse_expression(Lexer* lexer, Token* current);
int parse_term(Lexer* lexer, Token* current);
int parse_factor(Lexer* lexer, Token* current);

int parse_factor(Lexer* lexer, Token* current) {
    if (current->type == TOKEN_NUMBER) {
        int value = current->value;
        *current = lexer_next(lexer);
        return value;
    } else if (current->type == TOKEN_LPAREN) {
        *current = lexer_next(lexer);
        int result = parse_expression(lexer, current);
        if (current->type != TOKEN_RPAREN) {
            fprintf(stderr, "Error: Expected ')'\n");
            exit(1);
        }
        *current = lexer_next(lexer);
        return result;
    } else {
        fprintf(stderr, "Error: Expected number or '('\n");
        exit(1);
    }
}

int parse_term(Lexer* lexer, Token* current) {
    int result = parse_factor(lexer, current);
    
    while (current->type == TOKEN_MULTIPLY || current->type == TOKEN_DIVIDE) {
        TokenType op = current->type;
        *current = lexer_next(lexer);
        int right = parse_factor(lexer, current);
        
        if (op == TOKEN_MULTIPLY) {
            if ((result > 0 && right > 0 && result > INT_MAX / right) ||
                (result > 0 && right < 0 && right < INT_MIN / result) ||
                (result < 0 && right > 0 && result < INT_MIN / right) ||
                (result < 0 && right < 0 && result < INT_MAX / right)) {
                fprintf(stderr, "Error: Integer overflow\n");
                exit(1);
            }
            result *= right;
        } else {
            if (right == 0) {
                fprintf(stderr, "Error: Division by zero\n");
                exit(1);
            }
            if (result == INT_MIN && right == -1) {
                fprintf(stderr, "Error: Integer overflow\n");
                exit(1);
            }
            result /= right;
        }
    }
    
    return result;
}

int parse_expression(Lexer* lexer, Token* current) {
    int result = parse_term(lexer, current);
    
    while (current->type == TOKEN_PLUS || current->type == TOKEN_MINUS) {
        TokenType op = current->type;
        *current = lexer_next(lexer);
        int right = parse_term(lexer, current);
        
        if (op == TOKEN_PLUS) {
            if ((result > 0 && right > INT_MAX - result) ||
                (result < 0 && right < INT_MIN - result)) {
                fprintf(stderr, "Error: Integer overflow\n");
                exit(1);
            }
            result += right;
        } else {
            if ((result < 0 && right > INT_MAX + result) ||
                (result > 0 && right < INT_MIN + result)) {
                fprintf(stderr, "Error: Integer overflow\n");
                exit(1);
            }