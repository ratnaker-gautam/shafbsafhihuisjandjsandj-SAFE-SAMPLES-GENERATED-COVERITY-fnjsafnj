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
    TOKEN_INVALID
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
        default: return (Token){TOKEN_INVALID, 0};
    }
}

typedef struct {
    Lexer lexer;
    Token current;
} Parser;

void parser_init(Parser* parser, const char* input) {
    parser->lexer.input = input;
    parser->lexer.position = 0;
    parser->lexer.length = strlen(input);
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
    
    if (len == 1) {
        printf("Empty expression\n");
        return 1;
    }
    
    Parser parser;
    parser_init(&parser, input);
    
    int result = parser_parse_expression(&parser);
    
    if (parser.current.type != TOKEN_EOF && parser.current.type != TOKEN_INVALID) {
        printf("Invalid expression\n");
        return 1;
    }
    
    printf("Result: %d\n", result);
    return 0;
}