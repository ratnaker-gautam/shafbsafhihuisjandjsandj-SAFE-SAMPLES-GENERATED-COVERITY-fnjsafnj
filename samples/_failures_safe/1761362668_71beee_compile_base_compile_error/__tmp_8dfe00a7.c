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
    int length;
};

struct Parser {
    struct Lexer *lexer;
    struct Token current_token;
    int error_count;
};

void lexer_init(struct Lexer *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->length = strlen(input);
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

struct Token lexer_next_token(struct Lexer *lexer) {
    struct Token token = {TOKEN_UNKNOWN, "", lexer->line, lexer->column};
    
    if (lexer == NULL || lexer->position >= lexer->length) {
        token.type = TOKEN_EOF;
        return token;
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
        token.type = TOKEN_EOF;
        return token;
    }

    char current = lexer->input[lexer->position];
    token.line = lexer->line;
    token.column = lexer->column;

    if (isalpha(current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (lexer->position < lexer->length && 
               (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') &&
               i < 63) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
    } else if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (lexer->position < lexer->length && 
               isdigit(lexer->input[lexer->position]) && i < 63) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (lexer->position < lexer->length && 
               is_operator_char(lexer->input[lexer->position]) && i < 63) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        int i = 0;
        lexer->position++;
        lexer->column++;
        while (lexer->position < lexer->length && 
               lexer->input[lexer->position] != '"' && i < 63) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
        if (lexer->position < lexer->length && lexer->input[lexer->position] == '"') {
            lexer->position++;
            lexer->column++;
        }
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
    if (parser == NULL || lexer == NULL) return;
    parser->lexer = lexer;
    parser->error_count = 0;
    parser->current_token = lexer_next_token(lexer);
}

void parser_advance(struct Parser *parser) {
    if (parser == NULL || parser->lexer == NULL) return;
    parser->current_token = lexer_next_token(parser->lexer);
}

void parser_parse_expression(struct Parser *parser) {
    if (parser == NULL) return;
    
    if (parser->current_token.type == TOKEN_IDENTIFIER) {
        printf("Found identifier: %s\n", parser->current_token.value);
        parser_advance(parser);
    } else if (parser->current_token.type == TOKEN_NUMBER) {
        printf("Found number: %s\n", parser->current_token.value);
        parser_advance(parser