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
    Token token = {TOKEN_INVALID, 0};
    if (lexer == NULL || lexer->pos >= lexer->len) {
        token.type = TOKEN_EOF;
        return token;
    }

    while (lexer->pos < lexer->len && isspace(lexer->input[lexer->pos])) {
        lexer->pos++;
    }

    if (lexer->pos >= lexer->len) {
        token.type = TOKEN_EOF;
        return token;
    }

    char c = lexer->input[lexer->pos];
    if (isdigit(c)) {
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
        switch (c) {
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

void parser_init(Parser* parser, Lexer* lexer) {
    if (parser == NULL || lexer == NULL) return;
    parser->lexer = lexer;
    parser->current = lexer_next(lexer);
}

void parser_advance(Parser* parser) {
    if (parser == NULL) return;
    parser->current = lexer_next(parser->lexer);
}

int parser_expect(Parser* parser, TokenType type) {
    if (parser == NULL) return 0;
    if (parser->current.type == type) {
        parser_advance(parser);
        return 1;
    }
    return 0;
}

int parser_parse_expression(Parser* parser);
int parser_parse_term(Parser* parser);
int parser_parse_factor(Parser* parser);

int parser_parse_factor(Parser* parser) {
    if (parser == NULL) return 0;
    if (parser->current.type == TOKEN_NUMBER) {
        int value = parser->current.value;
        parser_advance(parser);
        return value;
    }
    if (parser_expect(parser, TOKEN_LPAREN)) {
        int result = parser_parse_expression(parser);
        if (parser_expect(parser, TOKEN_RPAREN)) {
            return result;
        }
        return 0;
    }
    return 0;
}

int parser_parse_term(Parser* parser) {
    if (parser == NULL) return 0;
    int result = parser_parse_factor(parser);
    while (parser->current.type == TOKEN_MULTIPLY || parser->current.type == TOKEN_DIVIDE) {
        TokenType op = parser->current.type;
        parser_advance(parser);
        int right = parser_parse_factor(parser);
        if (op == TOKEN_MULTIPLY) {
            result *= right;
        } else if (op == TOKEN_DIVIDE) {
            if (right == 0) return 0;
            result /= right;
        }
    }
    return result;
}

int parser_parse_expression(Parser* parser) {
    if (parser == NULL) return 0;
    int result = parser_parse_term(parser);
    while (parser->current.type == TOKEN_PLUS || parser->current.type == TOKEN_MINUS) {
        TokenType op = parser->current.type;
        parser_advance(parser);
        int right = parser_parse_term(parser);
        if (op == TOKEN_PLUS) {
            result += right;
        } else {
            result -= right;
        }
    }
    return result;
}

int evaluate_expression(const char* input) {
    if (input == NULL) return 0;
    Lexer lexer;
    Parser parser