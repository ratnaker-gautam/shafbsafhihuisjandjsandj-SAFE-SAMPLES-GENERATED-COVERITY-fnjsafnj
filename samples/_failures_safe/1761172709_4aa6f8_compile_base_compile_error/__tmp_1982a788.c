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

void lexer_init(Lexer* lexer, const char* input) {
    lexer->input = input;
    lexer->pos = 0;
    lexer->len = strlen(input);
}

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
        switch (current) {
            case '+': token.type = TOKEN_PLUS; break;
            case '-': token.type = TOKEN_MINUS; break;
            case '*': token.type = TOKEN_MULTIPLY; break;
            case '/': token.type = TOKEN_DIVIDE; break;
            case '(': token.type = TOKEN_LPAREN; break;
            case ')': token.type = TOKEN_RPAREN; break;
            default: token.type = TOKEN_INVALID; break;
        }
        lexer->pos++;
    }
    
    return token;
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
    } else if (token.type == TOKEN_LPAREN) {
        int result = parse_expression(lexer);
        token = lexer_next(lexer);
        if (token.type != TOKEN_RPAREN) {
            return 0;
        }
        return result;
    } else {
        return 0;
    }
}

int parse_term(Lexer* lexer) {
    int left = parse_factor(lexer);
    
    while (1) {
        Token token = lexer_peek(lexer);
        if (token.type == TOKEN_MULTIPLY || token.type == TOKEN_DIVIDE) {
            lexer_next(lexer);
            int right = parse_factor(lexer);
            
            if (token.type == TOKEN_MULTIPLY) {
                if (left != 0 && right != 0) {
                    if ((right > 0 && left > INT_MAX / right) || 
                        (right > 0 && left < INT_MIN / right) ||
                        (right < 0 && left < INT_MAX / right) ||
                        (right < 0 && left > INT_MIN / right)) {
                        return 0;
                    }
                }
                left *= right;
            } else {
                if (right == 0) {
                    return 0;
                }
                if (left == INT_MIN && right == -1) {
                    return 0;
                }
                left /= right;
            }
        } else {
            break;
        }
    }
    
    return left;
}

int parse_expression(Lexer* lexer) {
    int left = parse_term(lexer);
    
    while (1) {
        Token token = lexer_peek(lexer);
        if (token.type == TOKEN_PLUS || token.type == TOKEN_MINUS) {
            lexer_next(lexer);
            int right = parse_term(lexer);
            
            if (token.type == TOKEN_PLUS) {
                if ((right > 0 && left > INT_MAX - right) || 
                    (right < 0 && left < INT_MIN - right)) {
                    return 0;
                }
                left += right;
            } else {
                if ((right > 0 && left < INT_MIN + right) ||
                    (right < 0 && left > INT_MAX + right)) {
                    return 0;
                }
                left -= right;
            }
        } else {
            break;
        }
    }
    
    return left;
}

int evaluate_expression(const char* input