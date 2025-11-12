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
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->input_length = (int)strlen(input);
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

struct Token lexer_next_token(struct Lexer *lexer) {
    struct Token token;
    token.line = lexer->line;
    token.column = lexer->column;
    token.value[0] = '\0';

    if (lexer->position >= lexer->input_length) {
        token.type = TOKEN_EOF;
        return token;
    }

    char current = lexer->input[lexer->position];

    while (isspace((unsigned char)current)) {
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

    if (isalpha((unsigned char)current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while ((isalnum((unsigned char)current) || current == '_') && i < 63) {
            token.value[i++] = current;
            lexer->position++;
            lexer->column++;
            if (lexer->position >= lexer->input_length) break;
            current = lexer->input[lexer->position];
        }
        token.value[i] = '\0';
    } else if (isdigit((unsigned char)current)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (isdigit((unsigned char)current) && i < 63) {
            token.value[i++] = current;
            lexer->position++;
            lexer->column++;
            if (lexer->position >= lexer->input_length) break;
            current = lexer->input[lexer->position];
        }
        token.value[i] = '\0';
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        int i = 0;
        lexer->position++;
        lexer->column++;
        if (lexer->position >= lexer->input_length) {
            token.type = TOKEN_UNKNOWN;
            return token;
        }
        current = lexer->input[lexer->position];
        while (current != '"' && i < 63) {
            token.value[i++] = current;
            lexer->position++;
            lexer->column++;
            if (lexer->position >= lexer->input_length) break;
            current = lexer->input[lexer->position];
        }
        token.value[i] = '\0';
        if (current == '"') {
            lexer->position++;
            lexer->column++;
        }
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        token.value[0] = current;
        token.value[1] = '\0';
        lexer->position++;
        lexer->column++;
    } else {
        token.type = TOKEN_UNKNOWN;
        token.value[0] = current;
        token.value[1] = '\0';
        lexer->position++;
        lexer->column++;
    }

    return token;
}

void parser_init(struct Parser *parser, struct Lexer *lexer) {
    parser->lexer = lexer;
    parser->has_error = 0;
    parser->current_token = lexer_next_token(lexer);
}

void parser_advance(struct Parser *parser) {
    parser->current_token = lexer_next_token(parser->lexer);
}

void parser_parse_expression(struct Parser *parser) {
    if (parser->current_token.type == TOKEN_NUMBER || parser->current_token.type == TOKEN_IDENTIFIER) {
        parser_advance(parser);
        if (parser->current_token.type == TOKEN_OPERATOR) {
            parser_advance(parser);
            if (parser->current_token.type == TOKEN_NUMBER || parser->current_token.type == TOKEN_IDENTIFIER) {
                parser_advance(parser);
            } else {
                parser->has_error = 1;
            }
        }
    } else if (parser->current_token.type == TOKEN_STRING) {
        parser_advance(parser);
    } else