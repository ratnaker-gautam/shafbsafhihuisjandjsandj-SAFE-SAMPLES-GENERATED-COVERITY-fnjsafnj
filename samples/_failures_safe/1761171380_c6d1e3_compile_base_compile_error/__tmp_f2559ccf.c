//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
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

struct Lexer {
    const char *input;
    size_t position;
    size_t length;
    uint32_t line;
    uint32_t column;
};

struct Parser {
    struct Lexer *lexer;
    struct Token current_token;
    struct Token previous_token;
};

void lexer_init(struct Lexer *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->length = strlen(input);
    lexer->line = 1;
    lexer->column = 1;
}

int lexer_is_eof(struct Lexer *lexer) {
    if (lexer == NULL) return 1;
    return lexer->position >= lexer->length;
}

char lexer_peek(struct Lexer *lexer) {
    if (lexer_is_eof(lexer)) {
        return '\0';
    }
    return lexer->input[lexer->position];
}

char lexer_advance(struct Lexer *lexer) {
    if (lexer_is_eof(lexer)) {
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
    while (!lexer_is_eof(lexer) && isspace(lexer_peek(lexer))) {
        lexer_advance(lexer);
    }
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int is_delimiter_char(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ',' || c == ';' || c == ':';
}

void token_init(struct Token *token, enum TokenType type, const char *value, uint32_t line, uint32_t column) {
    if (token == NULL) return;
    token->type = type;
    if (value != NULL) {
        strncpy(token->value, value, sizeof(token->value) - 1);
        token->value[sizeof(token->value) - 1] = '\0';
    } else {
        token->value[0] = '\0';
    }
    token->line = line;
    token->column = column;
}

struct Token lexer_next_token(struct Lexer *lexer) {
    struct Token token;
    if (lexer == NULL) {
        token_init(&token, TOKEN_EOF, "", 1, 1);
        return token;
    }
    
    lexer_skip_whitespace(lexer);
    
    if (lexer_is_eof(lexer)) {
        token_init(&token, TOKEN_EOF, "", lexer->line, lexer->column);
        return token;
    }
    
    char c = lexer_peek(lexer);
    uint32_t line = lexer->line;
    uint32_t column = lexer->column;
    
    if (isalpha(c) || c == '_') {
        char buffer[64];
        size_t i = 0;
        while (!lexer_is_eof(lexer) && (isalnum(lexer_peek(lexer)) || lexer_peek(lexer) == '_')) {
            if (i < sizeof(buffer) - 1) {
                buffer[i++] = lexer_advance(lexer);
            } else {
                lexer_advance(lexer);
            }
        }
        buffer[i] = '\0';
        token_init(&token, TOKEN_IDENTIFIER, buffer, line, column);
        return token;
    }
    
    if (isdigit(c)) {
        char buffer[64];
        size_t i = 0;
        while (!lexer_is_eof(lexer) && isdigit(lexer_peek(lexer))) {
            if (i < sizeof(buffer) - 1) {
                buffer[i++] = lexer_advance(lexer);
            } else {
                lexer_advance(lexer);
            }
        }
        buffer[i] = '\0';
        token_init(&token, TOKEN_NUMBER, buffer, line, column);
        return token;
    }
    
    if (c == '"') {
        char buffer[64];
        size_t i = 0;
        lexer_advance(lexer);
        while (!lexer_is_eof(lexer) && lexer_peek(lexer) != '"') {
            if (i < sizeof(buffer) - 1) {
                buffer[i