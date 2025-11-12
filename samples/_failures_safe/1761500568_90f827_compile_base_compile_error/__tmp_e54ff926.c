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
    TOKEN_ERROR
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
    Token token = {TOKEN_ERROR, 0};
    
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
            if (value > (INT_MAX - (lexer->input[lexer->pos] - '0')) / 10) {
                token.type = TOKEN_ERROR;
                return token;
            }
            value = value * 10 + (lexer->input[lexer->pos] - '0');
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
            default: token.type = TOKEN_ERROR; break;
        }
        lexer->pos++;
    }
    
    return token;
}

int parse_expression(Lexer* lexer, Token* current);
int parse_term(Lexer* lexer, Token* current);
int parse_factor(Lexer* lexer, Token* current);

int parse_factor(Lexer* lexer, Token* current) {
    if (current->type == TOKEN_NUMBER) {
        int value = current->value;
        *current = lexer_next(lexer);
        return value;
    } else if (current->type == TOKEN_LPAREN) {
        *current = lexer_next(lexer);
        int result = parse_expression(lexer, current);
        if (current->type != TOKEN_RPAREN) {
            return 0;
        }
        *current = lexer_next(lexer);
        return result;
    } else {
        return 0;
    }
}

int parse_term(Lexer* lexer, Token* current) {
    int result = parse_factor(lexer, current);
    
    while (current->type == TOKEN_MULTIPLY || current->type == TOKEN_DIVIDE) {
        TokenType op = current->type;
        *current = lexer_next(lexer);
        int right = parse_factor(lexer, current);
        
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

int parse_expression(Lexer* lexer, Token* current) {
    int result = parse_term(lexer, current);
    
    while (current->type == TOKEN_PLUS || current->type == TOKEN_MINUS) {
        TokenType op = current->type;
        *current = lexer_next(lexer);
        int right = parse_term(lexer, current);
        
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
    lexer_init(&lexer, input);
    Token current = lexer_next(&lexer);
    return parse_expression(&lexer, &current);
}

int main() {
    char input[256];
    
    printf("Enter arithmetic expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty input\n");