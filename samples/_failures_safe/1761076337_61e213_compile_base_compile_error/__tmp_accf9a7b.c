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

struct Lexer {
    const char *input;
    uint32_t position;
    uint32_t line;
    uint32_t column;
    uint32_t length;
};

struct Parser {
    struct Lexer *lexer;
    struct Token current;
    struct Token previous;
    uint32_t error_count;
};

void lexer_init(struct Lexer *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->length = strlen(input);
}

int lexer_is_at_end(struct Lexer *lexer) {
    if (lexer == NULL) return 1;
    return lexer->position >= lexer->length;
}

char lexer_peek(struct Lexer *lexer) {
    if (lexer == NULL || lexer_is_at_end(lexer)) return '\0';
    return lexer->input[lexer->position];
}

char lexer_advance(struct Lexer *lexer) {
    if (lexer == NULL || lexer_is_at_end(lexer)) return '\0';
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
    while (!lexer_is_at_end(lexer)) {
        char c = lexer_peek(lexer);
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            lexer_advance(lexer);
        } else {
            break;
        }
    }
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int is_delimiter_char(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ',' || c == ';' || c == ':';
}

void token_init(struct Token *token, enum TokenType type, const char *value, uint32_t line, uint32_t column) {
    if (token == NULL || value == NULL) return;
    token->type = type;
    strncpy(token->value, value, sizeof(token->value) - 1);
    token->value[sizeof(token->value) - 1] = '\0';
    token->line = line;
    token->column = column;
}

struct Token lexer_next_token(struct Lexer *lexer) {
    struct Token token;
    token_init(&token, TOKEN_UNKNOWN, "", 0, 0);
    if (lexer == NULL) return token;
    
    lexer_skip_whitespace(lexer);
    if (lexer_is_at_end(lexer)) {
        token_init(&token, TOKEN_EOF, "EOF", lexer->line, lexer->column);
        return token;
    }
    
    uint32_t start_line = lexer->line;
    uint32_t start_column = lexer->column;
    char c = lexer_peek(lexer);
    
    if (isalpha(c) || c == '_') {
        char identifier[64] = {0};
        uint32_t i = 0;
        while (!lexer_is_at_end(lexer) && (isalnum(lexer_peek(lexer)) || lexer_peek(lexer) == '_')) {
            if (i < sizeof(identifier) - 1) {
                identifier[i++] = lexer_advance(lexer);
            } else {
                lexer_advance(lexer);
            }
        }
        identifier[i] = '\0';
        token_init(&token, TOKEN_IDENTIFIER, identifier, start_line, start_column);
    } else if (isdigit(c)) {
        char number[64] = {0};
        uint32_t i = 0;
        while (!lexer_is_at_end(lexer) && isdigit(lexer_peek(lexer))) {
            if (i < sizeof(number) - 1) {
                number[i++] = lexer_advance(lexer);
            } else {
                lexer_advance(lexer);
            }
        }
        number[i] = '\0';
        token_init(&token, TOKEN_NUMBER, number, start_line, start_column);
    } else if (c == '"') {
        lexer_advance(lexer);
        char string[64] = {0};
        uint32_t i = 0;
        while (!lexer_is_at_end(lex