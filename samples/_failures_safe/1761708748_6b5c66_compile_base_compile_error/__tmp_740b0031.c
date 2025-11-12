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
    TOKEN_UNKNOWN
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
    int has_error;
};

void lexer_init(struct Lexer *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->input_length = strlen(input);
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

void token_init(struct Token *token, enum TokenType type, const char *value, int line, int column) {
    if (token == NULL || value == NULL) return;
    token->type = type;
    strncpy(token->value, value, sizeof(token->value) - 1);
    token->value[sizeof(token->value) - 1] = '\0';
    token->line = line;
    token->column = column;
}

struct Token lexer_next_token(struct Lexer *lexer) {
    struct Token token;
    token_init(&token, TOKEN_UNKNOWN, "", lexer->line, lexer->column);
    
    if (lexer == NULL) return token;
    if (lexer->position >= lexer->input_length) {
        token.type = TOKEN_EOF;
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
    
    if (isalpha(current) || current == '_') {
        char identifier[64] = {0};
        int i = 0;
        while ((isalnum(current) || current == '_') && i < 63) {
            identifier[i++] = current;
            lexer->position++;
            lexer->column++;
            if (lexer->position >= lexer->input_length) break;
            current = lexer->input[lexer->position];
        }
        identifier[i] = '\0';
        token_init(&token, TOKEN_IDENTIFIER, identifier, token.line, token.column);
    } else if (isdigit(current)) {
        char number[64] = {0};
        int i = 0;
        while (isdigit(current) && i < 63) {
            number[i++] = current;
            lexer->position++;
            lexer->column++;
            if (lexer->position >= lexer->input_length) break;
            current = lexer->input[lexer->position];
        }
        number[i] = '\0';
        token_init(&token, TOKEN_NUMBER, number, token.line, token.column);
    } else if (current == '"') {
        char string[64] = {0};
        int i = 0;
        lexer->position++;
        lexer->column++;
        if (lexer->position >= lexer->input_length) {
            token_init(&token, TOKEN_UNKNOWN, "", token.line, token.column);
            return token;
        }
        current = lexer->input[lexer->position];
        while (current != '"' && i < 63) {
            string[i++] = current;
            lexer->position++;
            lexer->column++;
            if (lexer->position >= lexer->input_length) break;
            current = lexer->input[lexer->position];
        }
        string[i] = '\0';
        if (current == '"') {
            lexer->position++;
            lexer->column++;
        }
        token_init(&token, TOKEN_STRING, string, token.line, token.column);
    } else if (is_operator_char(current)) {
        char operator[3] = {0};
        operator[0] = current;
        lexer->position++;
        lexer->column++;
        if (lexer->position < lexer->input_length) {
            char next = lexer->input[lexer->position];
            if ((current == '=' && next == '=') || (current == '!' && next == '=') ||
                (current == '<' && next == '=') || (current == '>' && next == '=') ||
                (current == '&' && next == '&') || (current == '|' && next == '|')) {
                operator[1] =