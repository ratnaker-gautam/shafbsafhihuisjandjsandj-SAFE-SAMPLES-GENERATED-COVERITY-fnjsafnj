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

void init_lexer(struct LexerState *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->length = strlen(input);
    lexer->line = 1;
    lexer->column = 1;
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int is_delimiter_char(char c) {
    return strchr("(),;{}[]", c) != NULL;
}

int get_next_token(struct LexerState *lexer, struct Token *token) {
    if (lexer == NULL || token == NULL) return 0;
    if (lexer->position >= lexer->length) {
        token->type = TOKEN_EOF;
        strcpy(token->value, "");
        token->line = lexer->line;
        token->column = lexer->column;
        return 1;
    }

    while (lexer->position < lexer->length && isspace(lexer->input[lexer->position])) {
        if (lexer->input[lexer->position] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
    }

    if (lexer->position >= lexer->length) {
        token->type = TOKEN_EOF;
        strcpy(token->value, "");
        token->line = lexer->line;
        token->column = lexer->column;
        return 1;
    }

    char current = lexer->input[lexer->position];
    token->line = lexer->line;
    token->column = lexer->column;

    if (isalpha(current) || current == '_') {
        size_t len = 0;
        while (lexer->position < lexer->length && 
               (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') &&
               len < MAX_TOKEN_LEN - 1) {
            token->value[len++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token->value[len] = '\0';
        token->type = TOKEN_IDENTIFIER;
        return 1;
    } else if (isdigit(current)) {
        size_t len = 0;
        while (lexer->position < lexer->length && 
               isdigit(lexer->input[lexer->position]) &&
               len < MAX_TOKEN_LEN - 1) {
            token->value[len++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token->value[len] = '\0';
        token->type = TOKEN_NUMBER;
        return 1;
    } else if (current == '"') {
        size_t len = 0;
        lexer->position++;
        lexer->column++;
        while (lexer->position < lexer->length && 
               lexer->input[lexer->position] != '"' &&
               len < MAX_TOKEN_LEN - 1) {
            token->value[len++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        if (lexer->position < lexer->length && lexer->input[lexer->position] == '"') {
            lexer->position++;
            lexer->column++;
        }
        token->value[len] = '\0';
        token->type = TOKEN_STRING;
        return 1;
    } else if (is_operator_char(current)) {
        size_t len = 0;
        while (lexer->position < lexer->length && 
               is_operator_char(lexer->input[lexer->position]) &&
               len < MAX_TOKEN_LEN - 1) {
            token->value[len++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token->value[len] = '\0';
        token->type = TOKEN_OPERATOR;
        return 1;
    } else if (is_delimiter_char(current)) {
        token->value[0] = lexer->input[lexer->position++];
        token->value[1] = '\0';
        lexer->column++;
        token->type = TOKEN_DELIMITER;
        return 1;
    } else {
        token->value[0] = lexer->input[lexer->position++];
        token->value[1] = '\