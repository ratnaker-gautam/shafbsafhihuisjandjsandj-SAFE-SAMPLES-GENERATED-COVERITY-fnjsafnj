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

void lexer_init(Lexer* lexer, const char* input) {
    lexer->input = input;
    lexer->position = 0;
    lexer->length = (int)strlen(input);
}

Token lexer_next(Lexer* lexer) {
    while (lexer->position < lexer->length && isspace(lexer->input[lexer->position])) {
        lexer->position++;
    }
    
    if (lexer->position >= lexer->length) {
        return (Token){TOKEN_EOF, 0};
    }
    
    char current = lexer->input[lexer->position];
    
    if (isdigit(current)) {
        int value = 0;
        while (lexer->position < lexer->length && isdigit(lexer->input[lexer->position])) {
            value = value * 10 + (lexer->input[lexer->position] - '0');
            lexer->position++;
        }
        return (Token){TOKEN_NUMBER, value};
    }
    
    lexer->position++;
    
    switch (current) {
        case '+': return (Token){TOKEN_PLUS, 0};
        case '-': return (Token){TOKEN_MINUS, 0};
        case '*': return (Token){TOKEN_MULTIPLY, 0};
        case '/': return (Token){TOKEN_DIVIDE, 0};
        case '(': return (Token){TOKEN_LPAREN, 0};
        case ')': return (Token){TOKEN_RPAREN, 0};
        default: return (Token){TOKEN_ERROR, 0};
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
            result *= right;
        } else if (op == TOKEN_DIVIDE) {
            if (right != 0) {
                result /= right;
            } else {
                return 0;
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
    char input[256];
    
    printf("Enter arithmetic expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;