//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
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

int lexer_is_eof(struct Lexer *lexer) {
    if (lexer == NULL) return 1;
    return lexer->position >= lexer->length;
}

char lexer_peek(struct Lexer *lexer) {
    if (lexer == NULL || lexer_is_eof(lexer)) return '\0';
    return lexer->input[lexer->position];
}

char lexer_advance(struct Lexer *lexer) {
    if (lexer == NULL || lexer_is_eof(lexer)) return '\0';
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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

struct Token lexer_next_token(struct Lexer *lexer) {
    struct Token token = {TOKEN_UNKNOWN, "", 0, 0};
    if (lexer == NULL) return token;
    
    lexer_skip_whitespace(lexer);
    if (lexer_is_eof(lexer)) {
        token.type = TOKEN_EOF;
        token.line = lexer->line;
        token.column = lexer->column;
        return token;
    }
    
    token.line = lexer->line;
    token.column = lexer->column;
    char c = lexer_peek(lexer);
    
    if (isalpha(c) || c == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (!lexer_is_eof(lexer) && (isalnum(lexer_peek(lexer)) || lexer_peek(lexer) == '_')) {
            if (i < 63) {
                token.value[i++] = lexer_advance(lexer);
            } else {
                lexer_advance(lexer);
            }
        }
        token.value[i] = '\0';
    } else if (isdigit(c)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (!lexer_is_eof(lexer) && isdigit(lexer_peek(lexer))) {
            if (i < 63) {
                token.value[i++] = lexer_advance(lexer);
            } else {
                lexer_advance(lexer);
            }
        }
        token.value[i] = '\0';
    } else if (c == '"') {
        token.type = TOKEN_STRING;
        lexer_advance(lexer);
        int i = 0;
        while (!lexer_is_eof(lexer) && lexer_peek(lexer) != '"') {
            if (i < 63) {
                token.value[i++] = lexer_advance(lexer);
            } else {
                lexer_advance(lexer);
            }
        }
        if (lexer_peek(lexer) == '"') {
            lexer_advance(lexer);
        }
        token.value[i] = '\0';
    } else if (is_operator_char(c)) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (!lexer_is_eof(lexer) && is_operator_char(lexer_peek(lexer)) && i < 63) {
            token.value[i++] = lexer_advance(lexer);
        }
        token.value[i] = '\0';
    } else {
        token.type = TOKEN_UNKNOWN;
        token.value[0] = lexer_advance(lexer);
        token.value[1] = '\0';
    }
    
    return token;
}

void parser_init(struct Parser *parser, struct Lexer *lexer) {
    if (parser == NULL || lexer == NULL) return;
    parser->lexer = lexer;
    parser->error_count = 0;
    parser->current_token = lexer_next