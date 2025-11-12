//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

enum TokenType {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_UNKNOWN
};

struct Token {
    enum TokenType type;
    char value[64];
    uint32_t line;
    uint32_t column;
};

struct LexerState {
    const char *input;
    uint32_t position;
    uint32_t line;
    uint32_t column;
    uint32_t input_length;
};

struct ParserNode {
    enum TokenType node_type;
    char value[64];
    struct ParserNode *left;
    struct ParserNode *right;
};

struct ParseResult {
    int success;
    struct ParserNode *ast;
    char error_msg[128];
};

struct Parser {
    struct LexerState lexer;
    struct Token current_token;
    int has_error;
};

void init_lexer(struct LexerState *lexer, const char *input) {
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

int is_delimiter_char(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == ';' || c == ',';
}

struct Token get_next_token(struct LexerState *lexer) {
    struct Token token = {TOKEN_UNKNOWN, "", lexer->line, lexer->column};
    
    if (lexer == NULL || lexer->input == NULL) {
        strcpy(token.value, "NULL_STATE");
        return token;
    }
    
    while (lexer->position < lexer->input_length && isspace(lexer->input[lexer->position])) {
        if (lexer->input[lexer->position] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
    }
    
    if (lexer->position >= lexer->input_length) {
        token.type = TOKEN_EOF;
        strcpy(token.value, "EOF");
        return token;
    }
    
    char current = lexer->input[lexer->position];
    token.line = lexer->line;
    token.column = lexer->column;
    
    if (isalpha(current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        size_t i = 0;
        while (lexer->position < lexer->input_length && 
               (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') &&
               i < sizeof(token.value) - 1) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
    } else if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        size_t i = 0;
        while (lexer->position < lexer->input_length && 
               isdigit(lexer->input[lexer->position]) &&
               i < sizeof(token.value) - 1) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        size_t i = 0;
        lexer->position++;
        lexer->column++;
        while (lexer->position < lexer->input_length && 
               lexer->input[lexer->position] != '"' &&
               i < sizeof(token.value) - 1) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
        if (lexer->position < lexer->input_length && lexer->input[lexer->position] == '"') {
            lexer->position++;
            lexer->column++;
        }
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        size_t i = 0;
        while (lexer->position < lexer->input_length && 
               is_operator_char(lexer->input[lexer->position]) &&
               i < sizeof(token.value) - 1) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
    } else if (is_delimiter_char(current)) {
        token.type = TOKEN_DELIMITER;
        token.value[0] = lexer->input[lexer->position++];
        token.value[1] = '\0';
        lexer->column++;
    } else {
        token.type = TOKEN_UNKNOWN;
        token.value[0] = lexer->input[lexer->position++];
        token.value