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
        case '*': return (Token){TOKEN_MUL, 0};
        case '/': return (Token){TOKEN_DIV, 0};
        case '(': return (Token){TOKEN_LPAREN, 0};
        case ')': return (Token){TOKEN_RPAREN, 0};
        default: return (Token){TOKEN_INVALID, 0};
    }
}

typedef struct {
    Lexer lexer;
    Token current;
} Parser;

void parser_init(Parser* parser, const char* input) {
    parser->lexer.input = input;
    parser->lexer.pos = 0;
    parser->lexer.len = strlen(input);
    parser->current = lexer_next(&parser->lexer);
}

void parser_advance(Parser* parser) {
    parser->current = lexer_next(&parser->lexer);
}

int parser_parse_expression(Parser* parser);

int parser_parse_factor(Parser* parser) {
    if (parser->current.type == TOKEN_NUMBER) {
        int value = parser->current.value;
        parser_advance(parser);
        return value;
    } else if (parser->current.type == TOKEN_LPAREN) {
        parser_advance(parser);
        int value = parser_parse_expression(parser);
        if (parser->current.type != TOKEN_RPAREN) {
            return 0;
        }
        parser_advance(parser);
        return value;
    } else if (parser->current.type == TOKEN_MINUS) {
        parser_advance(parser);
        int value = parser_parse_factor(parser);
        if (value == INT_MIN) {
            return 0;
        }
        return -value;
    }
    return 0;
}

int parser_parse_term(Parser* parser) {
    int left = parser_parse_factor(parser);
    
    while (parser->current.type == TOKEN_MUL || parser->current.type == TOKEN_DIV) {
        TokenType op = parser->current.type;
        parser_advance(parser);
        int right = parser_parse_factor(parser);
        
        if (op == TOKEN_MUL) {
            if (left != 0 && right > INT_MAX / left) {
                return 0;
            }
            if (left != 0 && right < INT_MIN / left) {
                return 0;
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
    }
    
    return left;
}

int parser_parse_expression(Parser* parser) {
    int left = parser_parse_term(parser);
    
    while (parser->current.type == TOKEN_PLUS || parser->current.type == TOKEN_MINUS) {
        TokenType op = parser->current.type;
        parser_advance(parser);
        int right = parser_parse_term(parser);
        
        if (op == TOKEN_PLUS) {
            if (right > INT_MAX - left) {
                return 0;
            }
            left += right;
        } else {
            if (right < INT_MIN + left) {
                return 0;
            }
            left -= right;
        }
    }
    
    return left;
}

int evaluate_expression(const char* input) {
    Parser parser;
    parser_init(&parser, input);
    return parser_parse_expression(&parser);
}

int main(void) {
    char buffer[256];
    
    printf("Enter arithmetic expressions (empty line to exit):\n");
    
    while (1) {
        printf("> ");
        
        if (fgets