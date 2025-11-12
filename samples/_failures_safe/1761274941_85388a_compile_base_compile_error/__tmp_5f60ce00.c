//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MUL,
    TOKEN_DIV,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_END,
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
    Token token = {TOKEN_INVALID, 0};
    
    while (lexer->pos < lexer->len && isspace(lexer->input[lexer->pos])) {
        lexer->pos++;
    }
    
    if (lexer->pos >= lexer->len) {
        token.type = TOKEN_END;
        return token;
    }
    
    char c = lexer->input[lexer->pos];
    
    if (isdigit(c)) {
        int num = 0;
        while (lexer->pos < lexer->len && isdigit(lexer->input[lexer->pos])) {
            num = num * 10 + (lexer->input[lexer->pos] - '0');
            lexer->pos++;
        }
        token.type = TOKEN_NUMBER;
        token.value = num;
    } else {
        lexer->pos++;
        switch (c) {
            case '+': token.type = TOKEN_PLUS; break;
            case '-': token.type = TOKEN_MINUS; break;
            case '*': token.type = TOKEN_MUL; break;
            case '/': token.type = TOKEN_DIV; break;
            case '(': token.type = TOKEN_LPAREN; break;
            case ')': token.type = TOKEN_RPAREN; break;
            default: token.type = TOKEN_INVALID; break;
        }
    }
    
    return token;
}

int parse_expression(Lexer* lexer, Token* current);
int parse_term(Lexer* lexer, Token* current);
int parse_factor(Lexer* lexer, Token* current);

int parse_factor(Lexer* lexer, Token* current) {
    int result = 0;
    
    if (current->type == TOKEN_NUMBER) {
        result = current->value;
        *current = lexer_next(lexer);
    } else if (current->type == TOKEN_LPAREN) {
        *current = lexer_next(lexer);
        result = parse_expression(lexer, current);
        if (current->type != TOKEN_RPAREN) {
            fprintf(stderr, "Error: Expected ')'\n");
            exit(1);
        }
        *current = lexer_next(lexer);
    } else {
        fprintf(stderr, "Error: Expected number or '('\n");
        exit(1);
    }
    
    return result;
}

int parse_term(Lexer* lexer, Token* current) {
    int result = parse_factor(lexer, current);
    
    while (current->type == TOKEN_MUL || current->type == TOKEN_DIV) {
        TokenType op = current->type;
        *current = lexer_next(lexer);
        int right = parse_factor(lexer, current);
        
        if (op == TOKEN_MUL) {
            result *= right;
        } else {
            if (right == 0) {
                fprintf(stderr, "Error: Division by zero\n");
                exit(1);
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
            result += right;
        } else {
            result -= right;
        }
    }
    
    return result;
}

int evaluate_expression(const char* input) {
    if (input == NULL || strlen(input) == 0) {
        fprintf(stderr, "Error: Empty input\n");
        exit(1);
    }
    
    Lexer lexer;
    lexer.input = input;
    lexer.pos = 0;
    lexer.len = strlen(input);
    
    Token current = lexer_next(&lexer);
    int result = parse_expression(&lexer, &current);
    
    if (current.type != TOKEN_END) {
        fprintf(stderr, "Error: Unexpected token at end\n");
        exit(1);
    }
    
    return result;
}

int main() {
    char buffer[256];
    
    printf("Enter arithmetic expression: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    if (strlen(buffer) == 0) {
        fprintf(stderr, "Error: Empty input\n");
        return 1;
    }
    
    int result = evaluate_expression(buffer