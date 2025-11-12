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
    struct Token tokens[MAX_TOKENS];
    size_t token_count;
};

void init_lexer(struct LexerState *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->length = strlen(input);
    lexer->line = 1;
    lexer->column = 1;
    lexer->token_count = 0;
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int is_delimiter_char(char c) {
    return strchr("(),;{}[]", c) != NULL;
}

void add_token(struct LexerState *lexer, enum TokenType type, const char *value) {
    if (lexer == NULL || value == NULL || lexer->token_count >= MAX_TOKENS) return;
    struct Token *token = &lexer->tokens[lexer->token_count];
    token->type = type;
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
    token->line = lexer->line;
    token->column = lexer->column;
    lexer->token_count++;
}

char peek_char(const struct LexerState *lexer) {
    if (lexer == NULL || lexer->position >= lexer->length) return '\0';
    return lexer->input[lexer->position];
}

char next_char(struct LexerState *lexer) {
    if (lexer == NULL || lexer->position >= lexer->length) return '\0';
    char c = lexer->input[lexer->position++];
    if (c == '\n') {
        lexer->line++;
        lexer->column = 1;
    } else {
        lexer->column++;
    }
    return c;
}

void skip_whitespace(struct LexerState *lexer) {
    if (lexer == NULL) return;
    while (lexer->position < lexer->length && isspace(peek_char(lexer))) {
        next_char(lexer);
    }
}

void read_identifier(struct LexerState *lexer) {
    if (lexer == NULL) return;
    char buffer[MAX_TOKEN_LEN];
    size_t pos = 0;
    while (lexer->position < lexer->length && (isalnum(peek_char(lexer)) || peek_char(lexer) == '_')) {
        if (pos >= MAX_TOKEN_LEN - 1) break;
        buffer[pos++] = next_char(lexer);
    }
    buffer[pos] = '\0';
    add_token(lexer, TOKEN_IDENTIFIER, buffer);
}

void read_number(struct LexerState *lexer) {
    if (lexer == NULL) return;
    char buffer[MAX_TOKEN_LEN];
    size_t pos = 0;
    while (lexer->position < lexer->length && isdigit(peek_char(lexer))) {
        if (pos >= MAX_TOKEN_LEN - 1) break;
        buffer[pos++] = next_char(lexer);
    }
    buffer[pos] = '\0';
    add_token(lexer, TOKEN_NUMBER, buffer);
}

void read_string(struct LexerState *lexer) {
    if (lexer == NULL) return;
    char buffer[MAX_TOKEN_LEN];
    size_t pos = 0;
    char quote = next_char(lexer);
    while (lexer->position < lexer->length && peek_char(lexer) != quote) {
        if (pos >= MAX_TOKEN_LEN - 1) break;
        buffer[pos++] = next_char(lexer);
    }
    if (peek_char(lexer) == quote) {
        next_char(lexer);
    }
    buffer[pos] = '\0';
    add_token(lexer, TOKEN_STRING, buffer);
}

void read_operator(struct LexerState *lexer) {
    if (lexer == NULL) return;
    char buffer[3] = {0};
    buffer[0] = next_char(lexer);
    if (lexer->position < lexer->length && is_operator_char(peek_char(lexer))) {
        buffer[1] = peek_char(lexer);
        if ((buffer[0] == '&' && buffer[1] == '&') ||
            (buffer[0] == '|' && buffer[1] == '|') ||
            (buffer[0] == '=' && buffer[1] == '=') ||
            (buffer[0] == '!' && buffer[1] == '=') ||
            (buffer