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
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_PAREN,
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

struct Lexer {
    const char *input;
    size_t position;
    size_t length;
    int line;
    int column;
};

struct Parser {
    struct Token *tokens;
    size_t token_count;
    size_t current_token;
};

void lexer_init(struct Lexer *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->length = strlen(input);
    lexer->line = 1;
    lexer->column = 1;
}

int lexer_is_at_end(struct Lexer *lexer) {
    if (lexer == NULL) return 1;
    return lexer->position >= lexer->length;
}

char lexer_peek(struct Lexer *lexer) {
    if (lexer == NULL || lexer_is_at_end(lexer)) {
        return '\0';
    }
    return lexer->input[lexer->position];
}

char lexer_advance(struct Lexer *lexer) {
    if (lexer == NULL || lexer_is_at_end(lexer)) {
        return '\0';
    }
    char c = lexer->input[lexer->position++];
    if (c == '\n') {
        lexer->line++;
        lexer->column = 1;
    } else {
        lexer->column++;
    }
    return c;
}

void lexer_skip_whitespace(struct Lexer *lexer) {
    if (lexer == NULL) return;
    while (!lexer_is_at_end(lexer) && isspace(lexer_peek(lexer))) {
        lexer_advance(lexer);
    }
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void lexer_read_number(struct Lexer *lexer, struct Token *token) {
    if (lexer == NULL || token == NULL) return;
    size_t index = 0;
    while (!lexer_is_at_end(lexer) && (isdigit(lexer_peek(lexer)) || lexer_peek(lexer) == '.')) {
        if (index < MAX_TOKEN_LEN - 1) {
            token->value[index++] = lexer_advance(lexer);
        } else {
            lexer_advance(lexer);
        }
    }
    token->value[index] = '\0';
    token->type = TOKEN_NUMBER;
}

void lexer_read_identifier(struct Lexer *lexer, struct Token *token) {
    if (lexer == NULL || token == NULL) return;
    size_t index = 0;
    while (!lexer_is_at_end(lexer) && (isalnum(lexer_peek(lexer)) || lexer_peek(lexer) == '_')) {
        if (index < MAX_TOKEN_LEN - 1) {
            token->value[index++] = lexer_advance(lexer);
        } else {
            lexer_advance(lexer);
        }
    }
    token->value[index] = '\0';
    token->type = TOKEN_IDENTIFIER;
}

void lexer_read_string(struct Lexer *lexer, struct Token *token) {
    if (lexer == NULL || token == NULL) return;
    size_t index = 0;
    lexer_advance(lexer);
    while (!lexer_is_at_end(lexer) && lexer_peek(lexer) != '"') {
        if (index < MAX_TOKEN_LEN - 1) {
            token->value[index++] = lexer_advance(lexer);
        } else {
            lexer_advance(lexer);
        }
    }
    if (lexer_peek(lexer) == '"') {
        lexer_advance(lexer);
    }
    token->value[index] = '\0';
    token->type = TOKEN_STRING;
}

int lexer_next_token(struct Lexer *lexer, struct Token *token) {
    if (lexer == NULL || token == NULL) return 0;
    
    lexer_skip_whitespace(lexer);
    if (lexer_is_at_end(lexer)) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }
    
    token->line = lexer->line;
    token->column = lexer->column;
    
    char c = lexer_peek(lexer);
    if (isdigit(c)) {
        lexer_read_number(lexer, token);
    } else if (isalpha(c) || c == '_') {
        lexer_read_identifier(lexer, token);
    } else if