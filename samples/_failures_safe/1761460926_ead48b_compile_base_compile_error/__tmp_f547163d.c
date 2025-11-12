//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_ERROR
};

struct Token {
    enum TokenType type;
    char value[32];
    int line;
    int column;
};

struct Lexer {
    const char *input;
    int position;
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || 
           c == '=' || c == '<' || c == '>' || c == '!';
}

void init_lexer(struct Lexer *lexer, const char *input) {
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
}

struct Token get_next_token(struct Lexer *lexer) {
    struct Token token = {TOKEN_ERROR, "", lexer->line, lexer->column};
    
    if (lexer->input == NULL || lexer->position < 0) {
        return token;
    }
    
    while (lexer->input[lexer->position] != '\0') {
        char current = lexer->input[lexer->position];
        
        if (isspace(current)) {
            if (current == '\n') {
                lexer->line++;
                lexer->column = 1;
            } else {
                lexer->column++;
            }
            lexer->position++;
            continue;
        }
        
        if (isdigit(current)) {
            int i = 0;
            while (isdigit(lexer->input[lexer->position]) && i < 31) {
                token.value[i++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            token.value[i] = '\0';
            token.type = TOKEN_NUMBER;
            token.line = lexer->line;
            token.column = lexer->column - i;
            return token;
        }
        
        if (isalpha(current) || current == '_') {
            int i = 0;
            while ((isalnum(lexer->input[lexer->position]) || 
                   lexer->input[lexer->position] == '_') && i < 31) {
                token.value[i++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            token.value[i] = '\0';
            token.type = TOKEN_IDENTIFIER;
            token.line = lexer->line;
            token.column = lexer->column - i;
            return token;
        }
        
        if (current == '"') {
            int i = 0;
            lexer->position++;
            lexer->column++;
            
            while (lexer->input[lexer->position] != '"' && 
                   lexer->input[lexer->position] != '\0' && i < 31) {
                token.value[i++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            
            if (lexer->input[lexer->position] == '"') {
                lexer->position++;
                lexer->column++;
                token.value[i] = '\0';
                token.type = TOKEN_STRING;
                token.line = lexer->line;
                token.column = lexer->column - i - 2;
                return token;
            } else {
                token.type = TOKEN_ERROR;
                return token;
            }
        }
        
        if (is_operator_char(current)) {
            int i = 0;
            while (is_operator_char(lexer->input[lexer->position]) && i < 31) {
                token.value[i++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            token.value[i] = '\0';
            token.type = TOKEN_OPERATOR;
            token.line = lexer->line;
            token.column = lexer->column - i;
            return token;
        }
        
        token.type = TOKEN_ERROR;
        return token;
    }
    
    token.type = TOKEN_EOF;
    token.line = lexer->line;
    token.column = lexer->column;
    return token;
}

void print_token(struct Token token) {
    switch (token.type) {
        case TOKEN_NUMBER:
            printf("NUMBER: %s at line %d, column %d\n", token.value, token.line, token.column);
            break;
        case TOKEN_IDENTIFIER:
            printf("IDENTIFIER: %s at line %d, column %d\n", token.value, token.line, token.column);
            break;
        case TOKEN_OPERATOR:
            printf("OPERATOR: %s at line %d, column %d\n", token.value, token.line, token.column);
            break;
        case TOKEN_STRING:
            printf("STRING: \"%s\" at line %d, column %d\n", token.value, token.line, token.column);
            break;
        case TOKEN_EOF:
            printf("EOF at line %d, column %d\n", token.line, token.column);
            break;
        case TOKEN_ERROR:
            printf("ERROR at line %d, column %d\n", token.line, token.column);
            break;
    }
}

int main(void) {