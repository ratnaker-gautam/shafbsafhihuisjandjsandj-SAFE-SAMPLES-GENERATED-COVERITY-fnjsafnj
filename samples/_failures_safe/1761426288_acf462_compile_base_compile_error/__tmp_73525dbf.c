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
    if (!lexer || !input) return;
    lexer->input = input;
    lexer->pos = 0;
    lexer->len = strlen(input);
}

Token lexer_next(Lexer* lexer) {
    if (!lexer) return (Token){TOKEN_INVALID, 0};
    
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
    if (!parser || !lexer) return;
    parser->lexer = lexer;
    parser->current = lexer_next(lexer);
}

Token parser_current(Parser* parser) {
    if (!parser) return (Token){TOKEN_INVALID, 0};
    return parser->current;
}

void parser_advance(Parser* parser) {
    if (!parser) return;
    parser->current = lexer_next(parser->lexer);
}

int parser_expect(Parser* parser, TokenType type) {
    if (!parser) return 0;
    if (parser_current(parser).type == type) {
        parser_advance(parser);
        return 1;
    }
    return 0;
}

int parse_expression(Parser* parser);
int parse_term(Parser* parser);
int parse_factor(Parser* parser);

int parse_factor(Parser* parser) {
    if (!parser) return -1;
    Token token = parser_current(parser);
    
    if (token.type == TOKEN_NUMBER) {
        parser_advance(parser);
        return token.value;
    }
    
    if (parser_expect(parser, TOKEN_LPAREN)) {
        int result = parse_expression(parser);
        if (!parser_expect(parser, TOKEN_RPAREN)) {
            return -1;
        }
        return result;
    }
    
    return -1;
}

int parse_term(Parser* parser) {
    if (!parser) return -1;
    int left = parse_factor(parser);
    if (left == -1) return -1;
    
    while (1) {
        Token token = parser_current(parser);
        if (token.type == TOKEN_MULTIPLY || token.type == TOKEN_DIVIDE) {
            parser_advance(parser);
            int right = parse_factor(parser);
            if (right == -1) return -1;
            
            if (token.type == TOKEN_MULTIPLY) {
                if (left > 0 && right > 0 && left > INT_MAX / right) return -1;
                if (left < 0 && right < 0 && left < INT_MAX / right) return -1;
                left *= right;
            } else {
                if (right == 0) return -1;
                left /= right;
            }
        } else {
            break;
        }
    }
    
    return left;
}

int parse_expression(Parser* parser) {
    if (!parser) return -1;
    int left = parse_term(parser);
    if (left == -1) return -1;
    
    while (1) {
        Token token = parser_current(parser);
        if (token.type == TOKEN_PLUS || token.type == TOKEN_MINUS) {
            parser_advance(parser);
            int right = parse_term