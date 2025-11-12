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
    TOKEN_ERROR
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
    enum { NODE_EXPRESSION, NODE_STATEMENT, NODE_LITERAL } type;
    struct Token token;
    struct ParserNode *left;
    struct ParserNode *right;
};

struct ParseResult {
    int success;
    struct ParserNode *ast;
    char error_message[128];
};

const char *keywords[] = {"if", "else", "while", "for", "return", "int", "float", "char", NULL};

int is_keyword(const char *str) {
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void init_lexer(struct LexerState *lexer, const char *input) {
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->input_length = strlen(input);
}

char peek_char(struct LexerState *lexer) {
    if (lexer->position >= lexer->input_length) {
        return '\0';
    }
    return lexer->input[lexer->position];
}

char next_char(struct LexerState *lexer) {
    if (lexer->position >= lexer->input_length) {
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
    while (lexer->position < lexer->input_length && isspace(peek_char(lexer))) {
        next_char(lexer);
    }
}

struct Token read_identifier(struct LexerState *lexer) {
    struct Token token;
    token.type = TOKEN_IDENTIFIER;
    token.line = lexer->line;
    token.column = lexer->column;
    int i = 0;
    
    while (lexer->position < lexer->input_length && 
           (isalnum(peek_char(lexer)) || peek_char(lexer) == '_')) {
        if (i < 63) {
            token.value[i++] = next_char(lexer);
        } else {
            next_char(lexer);
        }
    }
    token.value[i] = '\0';
    
    if (is_keyword(token.value)) {
        token.type = TOKEN_KEYWORD;
    }
    
    return token;
}

struct Token read_number(struct LexerState *lexer) {
    struct Token token;
    token.type = TOKEN_NUMBER;
    token.line = lexer->line;
    token.column = lexer->column;
    int i = 0;
    int has_dot = 0;
    
    while (lexer->position < lexer->input_length && 
           (isdigit(peek_char(lexer)) || peek_char(lexer) == '.')) {
        if (peek_char(lexer) == '.') {
            if (has_dot) break;
            has_dot = 1;
        }
        if (i < 63) {
            token.value[i++] = next_char(lexer);
        } else {
            next_char(lexer);
        }
    }
    token.value[i] = '\0';
    return token;
}

struct Token read_string(struct LexerState *lexer) {
    struct Token token;
    token.type = TOKEN_STRING;
    token.line = lexer->line;
    token.column = lexer->column;
    int i = 0;
    
    char quote = next_char(lexer);
    while (lexer->position < lexer->input_length && peek_char(lexer) != quote) {
        if (i < 63) {
            token.value[i++] = next_char(lexer);
        } else {
            next_char(lexer);
        }
    }
    
    if (peek_char(lexer) == quote) {
        next_char(lexer);
    } else {
        token.type = TOKEN_ERROR;
        strcpy(token.value, "Unterminated string");
    }
    token.value[i] = '\0';
    return token;
}

struct Token next_token(struct LexerState *lexer) {
    skip_whitespace(lexer);
    
    if (lexer->position >= lexer->input_length) {
        struct Token token;
        token.type = TOKEN_EOF;
        token.value[0] = '\0';
        token.line = lexer->line;
        token.column = lexer->column;
        return token;
    }
    
    char c = peek_char(lex