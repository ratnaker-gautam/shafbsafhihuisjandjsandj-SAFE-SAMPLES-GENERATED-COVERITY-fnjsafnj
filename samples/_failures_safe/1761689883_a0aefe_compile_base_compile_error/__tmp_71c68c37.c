//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <ctype.h>
#include <string.h>

typedef enum {
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_EOF,
    TOKEN_ERROR
} TokenType;

typedef struct {
    TokenType type;
    int value;
} Token;

typedef struct {
    const char* input;
    int position;
    int length;
} Lexer;

typedef struct {
    Lexer* lexer;
    Token current;
} Parser;

int lexer_init(Lexer* lexer, const char* input) {
    if (lexer == NULL || input == NULL) return 0;
    lexer->input = input;
    lexer->position = 0;
    lexer->length = strlen(input);
    return 1;
}

Token lexer_next(Lexer* lexer) {
    Token token = {TOKEN_ERROR, 0};
    
    if (lexer == NULL || lexer->position >= lexer->length) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    while (lexer->position < lexer->length && isspace(lexer->input[lexer->position])) {
        lexer->position++;
    }
    
    if (lexer->position >= lexer->length) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char current = lexer->input[lexer->position];
    
    if (isdigit(current)) {
        int value = 0;
        while (lexer->position < lexer->length && isdigit(lexer->input[lexer->position])) {
            if (value > (2147483647 - (lexer->input[lexer->position] - '0')) / 10) {
                token.type = TOKEN_ERROR;
                return token;
            }
            value = value * 10 + (lexer->input[lexer->position] - '0');
            lexer->position++;
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
            default: token.type = TOKEN_ERROR; break;
        }
        lexer->position++;
    }
    
    return token;
}

int parser_init(Parser* parser, Lexer* lexer) {
    if (parser == NULL || lexer == NULL) return 0;
    parser->lexer = lexer;
    parser->current = lexer_next(lexer);
    return 1;
}

void parser_advance(Parser* parser) {
    if (parser != NULL && parser->lexer != NULL) {
        parser->current = lexer_next(parser->lexer);
    }
}

int parser_expect(Parser* parser, TokenType type) {
    if (parser == NULL) return 0;
    if (parser->current.type == type) {
        parser_advance(parser);
        return 1;
    }
    return 0;
}

int parse_expression(Parser* parser);
int parse_term(Parser* parser);
int parse_factor(Parser* parser);

int parse_factor(Parser* parser) {
    if (parser == NULL) return 0;
    
    if (parser->current.type == TOKEN_NUMBER) {
        int value = parser->current.value;
        parser_advance(parser);
        return value;
    }
    
    if (parser_expect(parser, TOKEN_LPAREN)) {
        int result = parse_expression(parser);
        if (parser_expect(parser, TOKEN_RPAREN)) {
            return result;
        }
        return 0;
    }
    
    return 0;
}

int parse_term(Parser* parser) {
    if (parser == NULL) return 0;
    
    int left = parse_factor(parser);
    if (left == 0 && parser->current.type != TOKEN_NUMBER && parser->current.type != TOKEN_LPAREN) {
        return 0;
    }
    
    while (parser->current.type == TOKEN_MULTIPLY || parser->current.type == TOKEN_DIVIDE) {
        TokenType op = parser->current.type;
        parser_advance(parser);
        
        int right = parse_factor(parser);
        if (right == 0) return 0;
        
        if (op == TOKEN_MULTIPLY) {
            if (left > 0 && right > 0 && left > 2147483647 / right) return 0;
            if (left < 0 && right < 0 && left < 2147483647 / right) return 0;
            left *= right;
        } else {
            if (right == 0) return 0;
            left /= right;
        }
    }
    
    return left;
}

int parse_expression(Parser* parser) {
    if (parser == NULL) return 0;
    
    int left = parse_term(parser);
    if (left == 0 && parser->current.type != TOKEN_NUMBER && parser->current.type != TO