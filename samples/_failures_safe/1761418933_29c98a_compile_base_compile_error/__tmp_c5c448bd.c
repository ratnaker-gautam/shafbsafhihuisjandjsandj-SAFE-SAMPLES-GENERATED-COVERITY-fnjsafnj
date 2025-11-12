//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_ERROR
};

struct Token {
    enum TokenType type;
    char value[64];
    int line;
    int column;
};

struct Lexer {
    const char *input;
    int position;
    int line;
    int column;
    int input_length;
};

struct Parser {
    struct Lexer *lexer;
    struct Token current_token;
    int error_count;
};

struct ASTNode {
    enum { NODE_IDENTIFIER, NODE_NUMBER, NODE_STRING, NODE_BINARY_OP } type;
    union {
        char identifier[64];
        double number;
        char string[64];
        struct {
            struct ASTNode *left;
            struct ASTNode *right;
            char operator[8];
        } binary_op;
    } value;
};

void init_lexer(struct Lexer *lexer, const char *input) {
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->input_length = (int)strlen(input);
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

struct Token get_next_token(struct Lexer *lexer) {
    struct Token token = {TOKEN_ERROR, "", lexer->line, lexer->column};
    
    if (lexer->position >= lexer->input_length) {
        token.type = TOKEN_EOF;
        strcpy(token.value, "");
        return token;
    }
    
    char current = lexer->input[lexer->position];
    
    while (isspace(current)) {
        if (current == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
        if (lexer->position >= lexer->input_length) {
            token.type = TOKEN_EOF;
            return token;
        }
        current = lexer->input[lexer->position];
    }
    
    token.line = lexer->line;
    token.column = lexer->column;
    
    if (isalpha(current)) {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (i < 63 && (isalnum(current) || current == '_')) {
            token.value[i++] = current;
            lexer->position++;
            lexer->column++;
            if (lexer->position >= lexer->input_length) break;
            current = lexer->input[lexer->position];
        }
        token.value[i] = '\0';
        return token;
    }
    
    if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        int dot_count = 0;
        while (i < 63 && (isdigit(current) || current == '.')) {
            if (current == '.') dot_count++;
            if (dot_count > 1) {
                token.type = TOKEN_ERROR;
                strcpy(token.value, "Invalid number format");
                return token;
            }
            token.value[i++] = current;
            lexer->position++;
            lexer->column++;
            if (lexer->position >= lexer->input_length) break;
            current = lexer->input[lexer->position];
        }
        token.value[i] = '\0';
        return token;
    }
    
    if (current == '"') {
        token.type = TOKEN_STRING;
        int i = 0;
        lexer->position++;
        lexer->column++;
        if (lexer->position >= lexer->input_length) {
            token.type = TOKEN_ERROR;
            strcpy(token.value, "Unterminated string");
            return token;
        }
        current = lexer->input[lexer->position];
        while (i < 63 && current != '"') {
            if (current == '\\') {
                lexer->position++;
                lexer->column++;
                if (lexer->position >= lexer->input_length) {
                    token.type = TOKEN_ERROR;
                    strcpy(token.value, "Unterminated string");
                    return token;
                }
                current = lexer->input[lexer->position];
            }
            token.value[i++] = current;
            lexer->position++;
            lexer->column++;
            if (lexer->position >= lexer->input_length) {
                token.type = TOKEN_ERROR;
                strcpy(token.value, "Unterminated string");
                return token;
            }
            current = lexer->input[lexer->position];
        }
        token.value[i] = '\0';
        if (current != '"') {
            token.type = TOKEN_ERROR;
            strcpy(token.value, "Unterminated string");
            return token;
        }
        lexer->position++;
        lexer->column++;
        return token;
    }
    
    if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (i < 7 && is_operator_char(current)) {
            token.value[i++] = current;
            lexer->position++;
            lexer->column++;
            if (lexer->