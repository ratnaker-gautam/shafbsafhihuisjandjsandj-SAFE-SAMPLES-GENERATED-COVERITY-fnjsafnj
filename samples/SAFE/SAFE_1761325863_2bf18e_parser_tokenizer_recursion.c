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
    size_t pos;
    size_t len;
} Lexer;

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

typedef struct {
    Lexer lexer;
    Token current;
} Parser;

void parser_init(Parser* parser, const char* input) {
    lexer_init(&parser->lexer, input);
    parser->current = lexer_next(&parser->lexer);
}

int parser_accept(Parser* parser, TokenType type) {
    if (parser->current.type == type) {
        parser->current = lexer_next(&parser->lexer);
        return 1;
    }
    return 0;
}

int parser_expect(Parser* parser, TokenType type) {
    if (parser_accept(parser, type)) {
        return 1;
    }
    return 0;
}

int parser_expression(Parser* parser);
int parser_term(Parser* parser);
int parser_factor(Parser* parser);

int parser_factor(Parser* parser) {
    if (parser_accept(parser, TOKEN_NUMBER)) {
        return parser->current.value;
    }
    
    if (parser_accept(parser, TOKEN_LPAREN)) {
        int result = parser_expression(parser);
        if (parser_expect(parser, TOKEN_RPAREN)) {
            return result;
        }
        return 0;
    }
    
    return 0;
}

int parser_term(Parser* parser) {
    int result = parser_factor(parser);
    
    while (1) {
        if (parser_accept(parser, TOKEN_MULTIPLY)) {
            result *= parser_factor(parser);
        } else if (parser_accept(parser, TOKEN_DIVIDE)) {
            int divisor = parser_factor(parser);
            if (divisor != 0) {
                result /= divisor;
            } else {
                result = 0;
            }
        } else {
            break;
        }
    }
    
    return result;
}

int parser_expression(Parser* parser) {
    int result = parser_term(parser);
    
    while (1) {
        if (parser_accept(parser, TOKEN_PLUS)) {
            result += parser_term(parser);
        } else if (parser_accept(parser, TOKEN_MINUS)) {
            result -= parser_term(parser);
        } else {
            break;
        }
    }
    
    return result;
}

int evaluate_expression(const char* input) {
    Parser parser;
    parser_init(&parser, input);
    return parser_expression(&parser);
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
    }
    
    int result = evaluate_expression(input);
    printf("Result: %d\n", result);
    
    return 0;
}