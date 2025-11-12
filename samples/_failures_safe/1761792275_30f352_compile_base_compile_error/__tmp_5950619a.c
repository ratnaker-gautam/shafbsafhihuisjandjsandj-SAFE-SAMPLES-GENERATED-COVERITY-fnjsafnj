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
    lexer->input = input;
    lexer->pos = 0;
    lexer->len = strlen(input);
}

Token lexer_next(Lexer* lexer) {
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
    parser->lexer = lexer;
    parser->current = lexer_next(lexer);
}

void parser_advance(Parser* parser) {
    parser->current = lexer_next(parser->lexer);
}

int parser_expect(Parser* parser, TokenType type) {
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
    if (parser->current.type == TOKEN_NUMBER) {
        int value = parser->current.value;
        parser_advance(parser);
        return value;
    } else if (parser_expect(parser, TOKEN_LPAREN)) {
        int result = parser_parse_expression(parser);
        if (!parser_expect(parser, TOKEN_RPAREN)) {
            return 0;
        }
        return result;
    }
    return 0;
}

int parser_parse_term(Parser* parser) {
    int result = parser_parse_factor(parser);
    
    while (parser->current.type == TOKEN_MULTIPLY || parser->current.type == TOKEN_DIVIDE) {
        TokenType op = parser->current.type;
        parser_advance(parser);
        
        int right = parser_parse_factor(parser);
        
        if (op == TOKEN_MULTIPLY) {
            if (result != 0 && right > INT_MAX / result) {
                return 0;
            }
            result *= right;
        } else {
            if (right == 0) {
                return 0;
            }
            result /= right;
        }
    }
    
    return result;
}

int parser_parse_expression(Parser* parser) {
    int result = parser_parse_term(parser);
    
    while (parser->current.type == TOKEN_PLUS || parser->current.type == TOKEN_MINUS) {
        TokenType op = parser->current.type;
        parser_advance(parser);
        
        int right = parser_parse_term(parser);
        
        if (op == TOKEN_PLUS) {
            if (result > INT_MAX - right) {
                return 0;
            }
            result += right;
        } else {
            if (result < INT_MIN + right) {
                return 0;
            }
            result -= right;
        }
    }
    
    return result;
}

int evaluate_expression(const char* input) {
    Lexer lexer;
    Parser parser;
    
    lexer_init(&lexer, input);
    parser_init(&parser, &lexer);
    
    int result = parser_parse_expression(&parser);
    
    if (parser.current.type != TOKEN_EOF) {
        return 0;
    }
    
    return result;
}

int main() {
    char input[