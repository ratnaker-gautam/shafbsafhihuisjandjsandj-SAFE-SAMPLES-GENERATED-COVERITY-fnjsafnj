//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
            if (value > (2147483647 - (lexer->input[lexer->pos] - '0')) / 10) {
                value = 2147483647;
            } else {
                value = value * 10 + (lexer->input[lexer->pos] - '0');
            }
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

typedef struct {
    Lexer lexer;
    Token current;
} Parser;

void parser_init(Parser* parser, const char* input) {
    lexer_init(&parser->lexer, input);
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
        return -parser_parse_factor(parser);
    } else if (parser->current.type == TOKEN_PLUS) {
        parser_advance(parser);
        return parser_parse_factor(parser);
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
            result *= right;
        } else if (op == TOKEN_DIVIDE) {
            if (right != 0) {
                result /= right;
            } else {
                result = 0;
            }
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
            result += right;
        } else {
            result -= right;
        }
    }
    
    return result;
}

int evaluate_expression(const char* input) {
    Parser parser;
    parser_init(&parser, input);
    return parser_parse_expression(&parser);
}

int main() {
    char buffer[256];
    
    printf("Enter arithmetic expressions (empty line to exit):\n");
    
    while (1) {
        printf("> ");
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(buffer);
        if (len > 0