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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
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
    token_init(&token, TOKEN_ERROR, "", lexer->line, lexer->column);
    
    if (lexer == NULL) return token;
    if (lexer->input == NULL) return token;
    
    if (lexer->position >= lexer->input_length) {
        token_init(&token, TOKEN_EOF, "EOF", lexer->line, lexer->column);
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
            token_init(&token, TOKEN_EOF, "EOF", lexer->line, lexer->column);
            return token;
        }
        current = lexer->input[lexer->position];
    }
    
    int start_pos = lexer->position;
    int start_line = lexer->line;
    int start_col = lexer->column;
    
    if (isalpha(current) || current == '_') {
        while (lexer->position < lexer->input_length && 
               (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_')) {
            lexer->position++;
            lexer->column++;
        }
        int length = lexer->position - start_pos;
        if (length >= 64) length = 63;
        char identifier[64];
        strncpy(identifier, &lexer->input[start_pos], length);
        identifier[length] = '\0';
        token_init(&token, TOKEN_IDENTIFIER, identifier, start_line, start_col);
    }
    else if (isdigit(current)) {
        while (lexer->position < lexer->input_length && isdigit(lexer->input[lexer->position])) {
            lexer->position++;
            lexer->column++;
        }
        int length = lexer->position - start_pos;
        if (length >= 64) length = 63;
        char number[64];
        strncpy(number, &lexer->input[start_pos], length);
        number[length] = '\0';
        token_init(&token, TOKEN_NUMBER, number, start_line, start_col);
    }
    else if (is_operator_char(current)) {
        lexer->position++;
        lexer->column++;
        char op[2] = {current, '\0'};
        token_init(&token, TOKEN_OPERATOR, op, start_line, start_col);
    }
    else if (current == '"') {
        lexer->position++;
        lexer->column++;
        int string_start = lexer->position;
        while (lexer->position < lexer->input_length && lexer->input[lexer->position] != '"') {
            lexer->position++;
            lexer->column++;
        }
        if (lexer->position >= lexer->input_length) {
            token_init(&token, TOKEN_ERROR, "Unterminated string", start_line, start_col);
            return token;
        }
        int length = lexer->position - string_start;
        if (length >= 64) length = 63;
        char string_val[64];
        strncpy(string_val, &lexer->input[string_start], length);
        string_val[length] = '\0';
        lexer->position++;
        lexer->column++;
        token_init(&token, TOKEN_STRING, string_val, start_line, start_col);
    }
    else {
        char unknown[2] = {current, '\0'};
        token_init(&token, TOKEN_ERROR,