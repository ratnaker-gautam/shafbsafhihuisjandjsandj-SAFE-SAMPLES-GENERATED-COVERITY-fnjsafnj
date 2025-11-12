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

typedef struct {
    Lexer* lexer;
    Token current;
} Parser;

void lexer_init(Lexer* lexer, const char* input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->pos = 0;
    lexer->len = strlen(input);
}

Token lexer_next(Lexer* lexer) {
    if (lexer == NULL) return (Token){TOKEN_INVALID, 0};
    
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
        case '*': return (Token){TOKEN_MULTIPLY, 0};
        case '/': return (Token){TOKEN_DIVIDE, 0};
        case '(': return (Token){TOKEN_LPAREN, 0};
        case ')': return (Token){TOKEN_RPAREN, 0};
        default: return (Token){TOKEN_INVALID, 0};
    }
}

void parser_init(Parser* parser, Lexer* lexer) {
    if (parser == NULL || lexer == NULL) return;
    parser->lexer = lexer;
    parser->current = lexer_next(lexer);
}

void parser_advance(Parser* parser) {
    if (parser == NULL) return;
    parser->current = lexer_next(parser->lexer);
}

int parser_accept(Parser* parser, TokenType type) {
    if (parser == NULL) return 0;
    if (parser->current.type == type) {
        parser_advance(parser);
        return 1;
    }
    return 0;
}

int parser_expect(Parser* parser, TokenType type) {
    if (parser == NULL) return 0;
    if (parser_accept(parser, type)) {
        return 1;
    }
    return 0;
}

int parser_expression(Parser* parser);
int parser_term(Parser* parser);
int parser_factor(Parser* parser);

int parser_factor(Parser* parser) {
    if (parser == NULL) return -1;
    
    if (parser_accept(parser, TOKEN_NUMBER)) {
        return parser->current.value;
    }
    
    if (parser_accept(parser, TOKEN_LPAREN)) {
        int result = parser_expression(parser);
        if (!parser_expect(parser, TOKEN_RPAREN)) {
            return -1;
        }
        return result;
    }
    
    return -1;
}

int parser_term(Parser* parser) {
    if (parser == NULL) return -1;
    
    int result = parser_factor(parser);
    if (result == -1) {
        return -1;
    }
    
    while (parser->current.type == TOKEN_MULTIPLY || parser->current.type == TOKEN_DIVIDE) {
        TokenType op = parser->current.type;
        parser_advance(parser);
        
        int right = parser_factor(parser);
        if (right == -1) {
            return -1;
        }
        
        if (op == TOKEN_MULTIPLY) {
            if (result > 0 && right > 0 && result > INT_MAX / right) {
                return -1;
            }
            if (result < 0 && right < 0 && result < INT_MAX / right) {
                return -1;
            }
            result *= right;
        } else {
            if (right == 0) {
                return -1;
            }
            result /= right;
        }
    }
    
    return result;
}

int parser_expression(Parser* parser) {
    if (parser == NULL) return -1;
    
    int result = parser_term(parser);
    if (result == -1) {
        return -1;
    }
    
    while (parser->current.type == TOKEN_PLUS