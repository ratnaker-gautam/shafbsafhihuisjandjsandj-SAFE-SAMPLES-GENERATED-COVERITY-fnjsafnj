//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

enum TokenType {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
    TOKEN_KEYWORD,
    TOKEN_EOF,
    TOKEN_INVALID
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

const char *keywords[] = {
    "if", "else", "while", "for", "return", "int", "char", "void"
};
const uint32_t num_keywords = 8;

void init_lexer(struct LexerState *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->input_length = strlen(input);
}

int is_keyword(const char *str) {
    if (str == NULL) return 0;
    for (uint32_t i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void skip_whitespace(struct LexerState *lexer) {
    if (lexer == NULL) return;
    while (lexer->position < lexer->input_length && isspace(lexer->input[lexer->position])) {
        if (lexer->input[lexer->position] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
    }
}

struct Token get_next_token(struct LexerState *lexer) {
    struct Token token = {TOKEN_INVALID, "", lexer->line, lexer->column};
    if (lexer == NULL || lexer->position >= lexer->input_length) {
        token.type = TOKEN_EOF;
        return token;
    }

    skip_whitespace(lexer);
    if (lexer->position >= lexer->input_length) {
        token.type = TOKEN_EOF;
        return token;
    }

    char current = lexer->input[lexer->position];
    uint32_t value_pos = 0;

    if (isalpha(current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        while (lexer->position < lexer->input_length && 
               (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') &&
               value_pos < 63) {
            token.value[value_pos++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[value_pos] = '\0';
        if (is_keyword(token.value)) {
            token.type = TOKEN_KEYWORD;
        }
    } else if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        while (lexer->position < lexer->input_length && 
               isdigit(lexer->input[lexer->position]) && value_pos < 63) {
            token.value[value_pos++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[value_pos] = '\0';
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        lexer->position++;
        lexer->column++;
        while (lexer->position < lexer->input_length && 
               lexer->input[lexer->position] != '"' && value_pos < 63) {
            token.value[value_pos++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[value_pos] = '\0';
        if (lexer->position < lexer->input_length && lexer->input[lexer->position] == '"') {
            lexer->position++;
            lexer->column++;
        }
    } else if (strchr("+-*/=<>!&|", current) != NULL) {
        token.type = TOKEN_OPERATOR;
        token.value[value_pos++] = lexer->input[lexer->position++];
        lexer->column++;
        if (lexer->position < lexer->input_length && strchr("=+-", lexer->input[lexer->position]) != NULL) {
            token.value[value_pos++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[value_pos] = '\0';
    } else if (strchr("(),;{}[]", current) != NULL) {
        token.type = TOKEN_DELIMITER;
        token.value[0] = lexer->input[lexer->position++];