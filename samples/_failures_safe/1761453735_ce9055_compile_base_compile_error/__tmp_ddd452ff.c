//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_ASSIGN,
    TOKEN_SEMICOLON,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_EOF,
    TOKEN_INVALID
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

int is_identifier_char(char c) {
    return isalnum((unsigned char)c) || c == '_';
}

void init_lexer(struct Lexer *lexer, const char *input) {
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
}

struct Token get_next_token(struct Lexer *lexer) {
    struct Token token;
    token.line = lexer->line;
    token.column = lexer->column;
    token.value[0] = '\0';
    
    if (lexer->input == NULL || lexer->input[lexer->position] == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    while (isspace((unsigned char)lexer->input[lexer->position])) {
        if (lexer->input[lexer->position] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
    }
    
    token.line = lexer->line;
    token.column = lexer->column;
    char current = lexer->input[lexer->position];
    
    switch (current) {
        case '+':
            token.type = TOKEN_PLUS;
            token.value[0] = '+';
            token.value[1] = '\0';
            lexer->position++;
            lexer->column++;
            break;
        case '-':
            token.type = TOKEN_MINUS;
            token.value[0] = '-';
            token.value[1] = '\0';
            lexer->position++;
            lexer->column++;
            break;
        case '*':
            token.type = TOKEN_MULTIPLY;
            token.value[0] = '*';
            token.value[1] = '\0';
            lexer->position++;
            lexer->column++;
            break;
        case '/':
            token.type = TOKEN_DIVIDE;
            token.value[0] = '/';
            token.value[1] = '\0';
            lexer->position++;
            lexer->column++;
            break;
        case '=':
            token.type = TOKEN_ASSIGN;
            token.value[0] = '=';
            token.value[1] = '\0';
            lexer->position++;
            lexer->column++;
            break;
        case ';':
            token.type = TOKEN_SEMICOLON;
            token.value[0] = ';';
            token.value[1] = '\0';
            lexer->position++;
            lexer->column++;
            break;
        case '(':
            token.type = TOKEN_LPAREN;
            token.value[0] = '(';
            token.value[1] = '\0';
            lexer->position++;
            lexer->column++;
            break;
        case ')':
            token.type = TOKEN_RPAREN;
            token.value[0] = ')';
            token.value[1] = '\0';
            lexer->position++;
            lexer->column++;
            break;
        default:
            if (isdigit((unsigned char)current)) {
                int i = 0;
                while (isdigit((unsigned char)lexer->input[lexer->position]) && i < 31) {
                    token.value[i++] = lexer->input[lexer->position++];
                    lexer->column++;
                }
                token.value[i] = '\0';
                token.type = TOKEN_NUMBER;
            } else if (isalpha((unsigned char)current) || current == '_') {
                int i = 0;
                while (is_identifier_char(lexer->input[lexer->position]) && i < 31) {
                    token.value[i++] = lexer->input[lexer->position++];
                    lexer->column++;
                }
                token.value[i] = '\0';
                token.type = TOKEN_IDENTIFIER;
            } else {
                token.type = TOKEN_INVALID;
                token.value[0] = current;
                token.value[1] = '\0';
                lexer->position++;
                lexer->column++;
            }
            break;
    }
    
    return token;
}

const char* token_type_to_string(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_PLUS: return "PLUS";
        case TOKEN_MINUS: return "MINUS";
        case TOKEN_MULTIPLY: return "MULTIPLY";
        case TOKEN_DIVIDE: return "DIVIDE";
        case TOKEN_ASSIGN: return "ASSIGN";
        case TOKEN_SEMICOLON: return "SEMICOLON