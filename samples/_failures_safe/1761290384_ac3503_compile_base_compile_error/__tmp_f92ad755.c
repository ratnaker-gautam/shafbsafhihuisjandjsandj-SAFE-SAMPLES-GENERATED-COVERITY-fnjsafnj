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
    uint32_t length;
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

static int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

static int is_delimiter_char(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ',' || c == ';';
}

static int is_keyword(const char *str) {
    const char *keywords[] = {"if", "else", "while", "for", "return", "int", "float", "char"};
    size_t num_keywords = sizeof(keywords) / sizeof(keywords[0]);
    for (size_t i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

static void init_lexer(struct LexerState *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->length = strlen(input);
}

static char peek_char(const struct LexerState *lexer) {
    if (lexer->position >= lexer->length) return '\0';
    return lexer->input[lexer->position];
}

static char next_char(struct LexerState *lexer) {
    if (lexer->position >= lexer->length) return '\0';
    char c = lexer->input[lexer->position++];
    if (c == '\n') {
        lexer->line++;
        lexer->column = 1;
    } else {
        lexer->column++;
    }
    return c;
}

static void skip_whitespace(struct LexerState *lexer) {
    while (lexer->position < lexer->length && isspace(peek_char(lexer))) {
        next_char(lexer);
    }
}

static struct Token next_token(struct LexerState *lexer) {
    struct Token token = {TOKEN_INVALID, "", lexer->line, lexer->column};
    skip_whitespace(lexer);
    if (lexer->position >= lexer->length) {
        token.type = TOKEN_EOF;
        return token;
    }
    char current = peek_char(lexer);
    if (isalpha(current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        size_t i = 0;
        while ((isalnum(current) || current == '_') && i < sizeof(token.value) - 1) {
            token.value[i++] = next_char(lexer);
            current = peek_char(lexer);
        }
        token.value[i] = '\0';
        if (is_keyword(token.value)) {
            token.type = TOKEN_KEYWORD;
        }
    } else if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        size_t i = 0;
        while (isdigit(current) && i < sizeof(token.value) - 1) {
            token.value[i++] = next_char(lexer);
            current = peek_char(lexer);
        }
        token.value[i] = '\0';
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        next_char(lexer);
        size_t i = 0;
        current = peek_char(lexer);
        while (current != '"' && current != '\0' && i < sizeof(token.value) - 1) {
            token.value[i++] = next_char(lexer);
            current = peek_char(lexer);
        }
        token.value[i] = '\0';
        if (current == '"') {
            next_char(lexer);
        }
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        size_t i = 0;
        while (is_operator_char(current) && i < sizeof(token.value) - 1) {
            token.value[i++] = next_char(lexer);
            current = peek_char(lexer);
        }
        token.value[i] = '\0';
    } else if (is_delimiter_char(current)) {
        token.type = TOKEN_DELIMITER;
        token.value[0] = next_char(lexer);
        token.value[1] = '\0';
    } else {
        token.type = TO