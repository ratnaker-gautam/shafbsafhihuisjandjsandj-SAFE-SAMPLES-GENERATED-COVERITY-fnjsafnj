//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64
#define MAX_INPUT_LEN 1024

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
    char value[MAX_TOKEN_LEN];
    int line;
    int column;
};

struct LexerState {
    const char *input;
    size_t position;
    size_t length;
    int line;
    int column;
};

struct ParserState {
    struct Token *tokens;
    size_t token_count;
    size_t current_token;
};

struct ExpressionNode {
    enum TokenType operator;
    struct Token operand1;
    struct Token operand2;
    struct ExpressionNode *left;
    struct ExpressionNode *right;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_delimiter_char(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ',' || c == ';';
}

void init_lexer(struct LexerState *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->length = strlen(input);
    lexer->line = 1;
    lexer->column = 1;
}

void skip_whitespace(struct LexerState *lexer) {
    if (lexer == NULL) return;
    while (lexer->position < lexer->length && isspace(lexer->input[lexer->position])) {
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
    struct Token token = {TOKEN_UNKNOWN, "", lexer->line, lexer->column};
    if (lexer == NULL || lexer->position >= lexer->length) {
        token.type = TOKEN_EOF;
        return token;
    }

    skip_whitespace(lexer);
    if (lexer->position >= lexer->length) {
        token.type = TOKEN_EOF;
        return token;
    }

    char current = lexer->input[lexer->position];
    size_t start_pos = lexer->position;
    int token_len = 0;

    if (isalpha(current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        while (lexer->position < lexer->length && 
               (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') &&
               token_len < MAX_TOKEN_LEN - 1) {
            token.value[token_len++] = lexer->input[lexer->position++];
            lexer->column++;
        }
    } else if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        while (lexer->position < lexer->length && 
               isdigit(lexer->input[lexer->position]) &&
               token_len < MAX_TOKEN_LEN - 1) {
            token.value[token_len++] = lexer->input[lexer->position++];
            lexer->column++;
        }
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        token.value[token_len++] = lexer->input[lexer->position++];
        lexer->column++;
        while (lexer->position < lexer->length && 
               lexer->input[lexer->position] != '"' &&
               token_len < MAX_TOKEN_LEN - 1) {
            token.value[token_len++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        if (lexer->position < lexer->length && lexer->input[lexer->position] == '"') {
            token.value[token_len++] = lexer->input[lexer->position++];
            lexer->column++;
        }
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        while (lexer->position < lexer->length && 
               is_operator_char(lexer->input[lexer->position]) &&
               token_len < MAX_TOKEN_LEN - 1) {
            token.value[token_len++] = lexer->input[lexer->position++];
            lexer->column++;
        }
    } else if (is_delimiter_char(current)) {
        token.type = TOKEN_DELIMITER;
        token.value[token_len++] = lexer->input[lexer->position++];
        lexer->column++;
    } else {
        token.type = TOKEN_UNKNOWN;
        token.value[token_len++] = lexer->input[lexer->position++];
        lexer->column++;
    }

    token.value[token_len] = '\0';