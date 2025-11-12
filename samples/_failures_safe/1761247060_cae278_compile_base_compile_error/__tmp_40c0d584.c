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
    size_t position;
    size_t length;
    int line;
    int column;
    struct Token current_token;
};

struct Parser {
    struct Lexer *lexer;
    int error_count;
};

void lexer_init(struct Lexer *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->length = strlen(input);
    lexer->line = 1;
    lexer->column = 1;
    memset(&lexer->current_token, 0, sizeof(struct Token));
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
    return strchr("+-*/=<>!&|", c) != NULL;
}

struct Token lexer_next_token(struct Lexer *lexer) {
    if (lexer == NULL) {
        struct Token invalid = {TOKEN_UNKNOWN, "", 0, 0};
        return invalid;
    }
    
    lexer_skip_whitespace(lexer);
    
    if (lexer_is_eof(lexer)) {
        struct Token eof_token = {TOKEN_EOF, "", lexer->line, lexer->column};
        return eof_token;
    }
    
    char c = lexer_peek(lexer);
    struct Token token;
    token.line = lexer->line;
    token.column = lexer->column;
    memset(token.value, 0, sizeof(token.value));
    
    if (isalpha(c) || c == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (!lexer_is_eof(lexer) && (isalnum(c) || c == '_') && i < 63) {
            token.value[i++] = lexer_advance(lexer);
            c = lexer_peek(lexer);
        }
        token.value[i] = '\0';
    } else if (isdigit(c)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (!lexer_is_eof(lexer) && (isdigit(c) || c == '.') && i < 63) {
            token.value[i++] = lexer_advance(lexer);
            c = lexer_peek(lexer);
        }
        token.value[i] = '\0';
    } else if (c == '"') {
        token.type = TOKEN_STRING;
        lexer_advance(lexer);
        int i = 0;
        while (!lexer_is_eof(lexer) && lexer_peek(lexer) != '"' && i < 63) {
            token.value[i++] = lexer_advance(lexer);
        }
        token.value[i] = '\0';
        if (!lexer_is_eof(lexer) && lexer_peek(lexer) == '"') {
            lexer_advance(lexer);
        }
    } else if (is_operator_char(c)) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (!lexer_is_eof(lexer) && is_operator_char(c) && i < 63) {
            token.value[i++] = lexer_advance(lexer);
            c = lexer_peek(lexer);
        }
        token.value[i] = '\0';
    } else {
        token.type = TOKEN_UNKNOWN;
        token.value[0] = lexer_advance(lexer);
        token.value[1] = '\0';
    }
    
    lexer->current_token = token;
    return token;
}

void parser_init(struct Parser *parser, struct Lexer *lexer) {
    if (parser == NULL || lexer == NULL) return;
    parser->lexer = lexer;
    parser->error_count = 0;
}

void parser_parse_expression(struct Parser *parser)