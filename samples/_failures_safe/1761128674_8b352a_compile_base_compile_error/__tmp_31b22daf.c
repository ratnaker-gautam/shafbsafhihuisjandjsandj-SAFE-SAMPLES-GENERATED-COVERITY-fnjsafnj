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

struct ParserState {
    struct LexerState *lexer;
    struct Token current_token;
};

const char *keywords[] = {
    "if", "else", "while", "for", "return", "int", "char", "void"
};
const size_t num_keywords = 8;

int is_keyword(const char *str) {
    if (!str) return 0;
    for (size_t i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void init_lexer(struct LexerState *lexer, const char *input) {
    if (!lexer || !input) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->input_length = strlen(input);
}

char peek_char(struct LexerState *lexer) {
    if (!lexer || lexer->position >= lexer->input_length) {
        return '\0';
    }
    return lexer->input[lexer->position];
}

char next_char(struct LexerState *lexer) {
    if (!lexer || lexer->position >= lexer->input_length) {
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

void skip_whitespace(struct LexerState *lexer) {
    if (!lexer) return;
    while (lexer->position < lexer->input_length) {
        char c = peek_char(lexer);
        if (!isspace(c)) break;
        next_char(lexer);
    }
}

struct Token next_token(struct LexerState *lexer) {
    struct Token token = {TOKEN_INVALID, "", lexer->line, lexer->column};
    if (!lexer) return token;
    
    skip_whitespace(lexer);
    
    if (lexer->position >= lexer->input_length) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char c = peek_char(lexer);
    token.line = lexer->line;
    token.column = lexer->column;
    
    if (isalpha(c) || c == '_') {
        token.type = TOKEN_IDENTIFIER;
        size_t i = 0;
        while ((isalnum(c) || c == '_') && i < 63) {
            token.value[i++] = next_char(lexer);
            c = peek_char(lexer);
        }
        token.value[i] = '\0';
        if (is_keyword(token.value)) {
            token.type = TOKEN_KEYWORD;
        }
    } else if (isdigit(c)) {
        token.type = TOKEN_NUMBER;
        size_t i = 0;
        while (isdigit(c) && i < 63) {
            token.value[i++] = next_char(lexer);
            c = peek_char(lexer);
        }
        token.value[i] = '\0';
    } else if (c == '"') {
        token.type = TOKEN_STRING;
        next_char(lexer);
        size_t i = 0;
        c = peek_char(lexer);
        while (c != '"' && c != '\0' && i < 63) {
            token.value[i++] = next_char(lexer);
            c = peek_char(lexer);
        }
        token.value[i] = '\0';
        if (c == '"') {
            next_char(lexer);
        }
    } else if (strchr("+-*/=<>!&|", c)) {
        token.type = TOKEN_OPERATOR;
        size_t i = 0;
        while (strchr("+-*/=<>!&|", c) && i < 63) {
            token.value[i++] = next_char(lexer);
            c = peek_char(lexer);
        }
        token.value[i] = '\0';
    } else if (strchr("(),;{}[]", c)) {
        token.type = TOKEN_DELIMITER;
        token.value[0] = next_char(lexer);
        token.value[1] = '\0';
    } else {
        token.type = TOKEN_INVALID;
        token.value[0] = next_char(lexer