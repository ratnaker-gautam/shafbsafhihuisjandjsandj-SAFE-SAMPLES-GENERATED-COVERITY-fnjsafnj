//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
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
    char value[32];
} Token;

typedef struct {
    const char* input;
    size_t pos;
    size_t len;
} Lexer;

Token lexer_next(Lexer* lexer) {
    Token token = {TOKEN_INVALID, ""};
    
    while (lexer->pos < lexer->len && isspace(lexer->input[lexer->pos])) {
        lexer->pos++;
    }
    
    if (lexer->pos >= lexer->len) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char current = lexer->input[lexer->pos];
    
    if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (lexer->pos < lexer->len && i < 31 && isdigit(lexer->input[lexer->pos])) {
            token.value[i++] = lexer->input[lexer->pos++];
        }
        token.value[i] = '\0';
        return token;
    }
    
    if (isalpha(current)) {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (lexer->pos < lexer->len && i < 31 && isalnum(lexer->input[lexer->pos])) {
            token.value[i++] = lexer->input[lexer->pos++];
        }
        token.value[i] = '\0';
        return token;
    }
    
    switch (current) {
        case '+': token.type = TOKEN_PLUS; break;
        case '-': token.type = TOKEN_MINUS; break;
        case '*': token.type = TOKEN_MULTIPLY; break;
        case '/': token.type = TOKEN_DIVIDE; break;
        case '(': token.type = TOKEN_LPAREN; break;
        case ')': token.type = TOKEN_RPAREN; break;
        default: token.type = TOKEN_INVALID; break;
    }
    
    if (token.type != TOKEN_INVALID) {
        token.value[0] = current;
        token.value[1] = '\0';
        lexer->pos++;
    }
    
    return token;
}

int parse_expression(Lexer* lexer, Token* current);
int parse_term(Lexer* lexer, Token* current);
int parse_factor(Lexer* lexer, Token* current);

int parse_factor(Lexer* lexer, Token* current) {
    if (current->type == TOKEN_NUMBER) {
        int value = atoi(current->value);
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
    }
    return 0;
}

int parse_term(Lexer* lexer, Token* current) {
    int result = parse_factor(lexer, current);
    
    while (current->type == TOKEN_MULTIPLY || current->type == TOKEN_DIVIDE) {
        TokenType op = current->type;
        *current = lexer_next(lexer);
        int right = parse_factor(lexer, current);
        
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
        return 0;
    }
    
    Lexer lexer;
    lexer.input = input;
    lexer.pos = 0;
    lexer.len = strlen(input);
    
    Token current = lexer_next(&lexer);
    return parse_expression(&lexer, &current);
}

int main() {
    char input[256];
    
    printf("Enter arithmetic expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len -