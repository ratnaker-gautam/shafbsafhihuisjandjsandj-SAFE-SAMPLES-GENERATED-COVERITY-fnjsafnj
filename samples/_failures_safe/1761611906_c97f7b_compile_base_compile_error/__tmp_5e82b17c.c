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
    int error_count;
};

void lexer_init(struct Lexer *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->input_length = (int)strlen(input);
}

int lexer_is_eof(struct Lexer *lexer) {
    if (lexer == NULL) return 1;
    return lexer->position >= lexer->input_length;
}

char lexer_peek(struct Lexer *lexer) {
    if (lexer_is_eof(lexer)) return '\0';
    return lexer->input[lexer->position];
}

char lexer_advance(struct Lexer *lexer) {
    if (lexer_is_eof(lexer)) return '\0';
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

struct Token lexer_next_token(struct Lexer *lexer) {
    struct Token token = {TOKEN_UNKNOWN, "", 0, 0};
    if (lexer == NULL) return token;
    
    lexer_skip_whitespace(lexer);
    token.line = lexer->line;
    token.column = lexer->column;
    
    if (lexer_is_eof(lexer)) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char c = lexer_peek(lexer);
    int value_index = 0;
    
    if (isalpha(c) || c == '_') {
        token.type = TOKEN_IDENTIFIER;
        while (!lexer_is_eof(lexer) && (isalnum(lexer_peek(lexer)) || lexer_peek(lexer) == '_')) {
            if (value_index < 63) {
                token.value[value_index++] = lexer_advance(lexer);
            } else {
                lexer_advance(lexer);
            }
        }
        token.value[value_index] = '\0';
    } else if (isdigit(c)) {
        token.type = TOKEN_NUMBER;
        while (!lexer_is_eof(lexer) && isdigit(lexer_peek(lexer))) {
            if (value_index < 63) {
                token.value[value_index++] = lexer_advance(lexer);
            } else {
                lexer_advance(lexer);
            }
        }
        token.value[value_index] = '\0';
    } else if (c == '"') {
        token.type = TOKEN_STRING;
        lexer_advance(lexer);
        while (!lexer_is_eof(lexer) && lexer_peek(lexer) != '"') {
            if (value_index < 63) {
                token.value[value_index++] = lexer_advance(lexer);
            } else {
                lexer_advance(lexer);
            }
        }
        if (lexer_peek(lexer) == '"') {
            lexer_advance(lexer);
        }
        token.value[value_index] = '\0';
    } else if (strchr("+-*/=<>!&|", c) != NULL) {
        token.type = TOKEN_OPERATOR;
        if (value_index < 63) {
            token.value[value_index++] = lexer_advance(lexer);
        }
        char next = lexer_peek(lexer);
        if ((c == '=' && next == '=') || (c == '!' && next == '=') ||
            (c == '<' && next == '=') || (c == '>' && next == '=') ||
            (c == '&' && next == '&') || (c == '|' && next == '|')) {
            if (value_index < 63) {
                token.value[value_index++] = lexer_advance(lexer);
            }
        }
        token.value[value_index] = '\0';
    } else {
        token.type = TOKEN_UNKNOWN;
        if (value_index < 63) {
            token.value[value_index++] = lexer_advance(lexer);
        }
        token.value[value_index] = '\0';
    }
    
    return token;
}