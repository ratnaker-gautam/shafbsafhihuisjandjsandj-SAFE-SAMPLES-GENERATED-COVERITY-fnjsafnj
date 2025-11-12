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
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->input_length = (int)strlen(input);
}

int lexer_is_at_end(struct Lexer *lexer) {
    if (lexer == NULL) return 1;
    return lexer->position >= lexer->input_length;
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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

struct Token lexer_next_token(struct Lexer *lexer) {
    struct Token token = {TOKEN_UNKNOWN, "", 0, 0};
    if (lexer == NULL) return token;
    
    lexer_skip_whitespace(lexer);
    if (lexer_is_at_end(lexer)) {
        token.type = TOKEN_EOF;
        token.line = lexer->line;
        token.column = lexer->column;
        return token;
    }
    
    char c = lexer_peek(lexer);
    token.line = lexer->line;
    token.column = lexer->column;
    
    if (isalpha(c) || c == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (!lexer_is_at_end(lexer) && (isalnum(c) || c == '_')) {
            if (i < 63) {
                token.value[i++] = c;
            }
            lexer_advance(lexer);
            c = lexer_peek(lexer);
        }
        token.value[i] = '\0';
    } else if (isdigit(c)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (!lexer_is_at_end(lexer) && isdigit(c)) {
            if (i < 63) {
                token.value[i++] = c;
            }
            lexer_advance(lexer);
            c = lexer_peek(lexer);
        }
        token.value[i] = '\0';
    } else if (is_operator_char(c)) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (!lexer_is_at_end(lexer) && is_operator_char(c) && i < 63) {
            token.value[i++] = c;
            lexer_advance(lexer);
            c = lexer_peek(lexer);
        }
        token.value[i] = '\0';
    } else if (c == '"') {
        token.type = TOKEN_STRING;
        lexer_advance(lexer);
        int i = 0;
        c = lexer_peek(lexer);
        while (!lexer_is_at_end(lexer) && c != '"' && i < 63) {
            token.value[i++] = c;
            lexer_advance(lexer);
            c = lexer_peek(lexer);
        }
        token.value[i] = '\0';
        if (c == '"') {
            lexer_advance(lexer);
        }
    } else {
        token.type = TOKEN_UNKNOWN;
        token.value[0] = lexer_advance(lexer);
        token.value[1] = '\0';
    }
    
    return token;
}

void parser_init(struct Parser *parser, struct Lexer *lexer) {
    if (parser == NULL || lexer == NULL) return;
    parser->lexer = lexer