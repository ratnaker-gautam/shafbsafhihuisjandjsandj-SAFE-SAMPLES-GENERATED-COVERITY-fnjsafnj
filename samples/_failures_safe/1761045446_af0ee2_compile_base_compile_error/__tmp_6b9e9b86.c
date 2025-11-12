//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
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
        int val = 0;
        while (lexer->pos < lexer->len && isdigit(lexer->input[lexer->pos])) {
            if (val > (INT_MAX - (lexer->input[lexer->pos] - '0')) / 10) {
                token.type = TOKEN_INVALID;
                return token;
            }
            val = val * 10 + (lexer->input[lexer->pos] - '0');
            lexer->pos++;
        }
        token.type = TOKEN_NUMBER;
        token.value = val;
    } else {
        switch (c) {
            case '+': token.type = TOKEN_PLUS; break;
            case '-': token.type = TOKEN_MINUS; break;
            case '*': token.type = TOKEN_MUL; break;
            case '/': token.type = TOKEN_DIV; break;
            case '(': token.type = TOKEN_LPAREN; break;
            case ')': token.type = TOKEN_RPAREN; break;
            default: token.type = TOKEN_INVALID; break;
        }
        lexer->pos++;
    }
    return token;
}

typedef struct {
    Lexer lexer;
    Token current;
} Parser;

void parser_init(Parser* parser, const char* input) {
    if (parser == NULL || input == NULL) return;
    lexer_init(&parser->lexer, input);
    parser->current = lexer_next(&parser->lexer);
}

void parser_advance(Parser* parser) {
    if (parser == NULL) return;
    parser->current = lexer_next(&parser->lexer);
}

int parser_parse_expression(Parser* parser);
int parser_parse_term(Parser* parser);
int parser_parse_factor(Parser* parser);

int parser_parse_factor(Parser* parser) {
    if (parser == NULL) return 0;
    int result = 0;
    if (parser->current.type == TOKEN_NUMBER) {
        result = parser->current.value;
        parser_advance(parser);
    } else if (parser->current.type == TOKEN_LPAREN) {
        parser_advance(parser);
        result = parser_parse_expression(parser);
        if (parser->current.type != TOKEN_RPAREN) {
            return 0;
        }
        parser_advance(parser);
    } else {
        return 0;
    }
    return result;
}

int parser_parse_term(Parser* parser) {
    if (parser == NULL) return 0;
    int result = parser_parse_factor(parser);
    while (parser->current.type == TOKEN_MUL || parser->current.type == TOKEN_DIV) {
        TokenType op = parser->current.type;
        parser_advance(parser);
        int right = parser_parse_factor(parser);
        if (op == TOKEN_MUL) {
            if (result > 0 && right > 0 && result > INT_MAX / right) return 0;
            result *= right;
        } else {
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
            if (result > 0 && right > INT_MAX - result) return 0;
            result += right;
        } else {
            if (result < 0 && right > 0 && result < INT_MIN + right) return 0;
            result -= right;
        }
    }
    return result;
}

int main(void) {
    char input[256];
    printf("Enter arithmetic expression: ");
    if