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
    int input_length;
};

struct Parser {
    struct Lexer *lexer;
    struct Token current_token;
    int error_count;
};

void lexer_init(struct Lexer *lexer, const char *input) {
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->input_length = (int)strlen(input);
}

int lexer_is_at_end(struct Lexer *lexer) {
    return lexer->position >= lexer->input_length;
}

char lexer_peek(struct Lexer *lexer) {
    if (lexer_is_at_end(lexer)) {
        return '\0';
    }
    return lexer->input[lexer->position];
}

char lexer_advance(struct Lexer *lexer) {
    if (lexer_is_at_end(lexer)) {
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

void lexer_skip_whitespace(struct Lexer *lexer) {
    while (!lexer_is_at_end(lexer) && isspace(lexer_peek(lexer))) {
        lexer_advance(lexer);
    }
}

struct Token lexer_next_token(struct Lexer *lexer) {
    struct Token token = {TOKEN_UNKNOWN, "", lexer->line, lexer->column};
    
    lexer_skip_whitespace(lexer);
    
    if (lexer_is_at_end(lexer)) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char c = lexer_peek(lexer);
    int start_pos = lexer->position;
    int start_col = lexer->column;
    
    if (isalpha(c) || c == '_') {
        int i = 0;
        while (!lexer_is_at_end(lexer) && (isalnum(lexer_peek(lexer)) || lexer_peek(lexer) == '_')) {
            if (i < 63) {
                token.value[i++] = lexer_advance(lexer);
            } else {
                lexer_advance(lexer);
            }
        }
        token.value[i] = '\0';
        token.type = TOKEN_IDENTIFIER;
    } else if (isdigit(c)) {
        int i = 0;
        while (!lexer_is_at_end(lexer) && isdigit(lexer_peek(lexer))) {
            if (i < 63) {
                token.value[i++] = lexer_advance(lexer);
            } else {
                lexer_advance(lexer);
            }
        }
        token.value[i] = '\0';
        token.type = TOKEN_NUMBER;
    } else if (c == '"') {
        lexer_advance(lexer);
        int i = 0;
        while (!lexer_is_at_end(lexer) && lexer_peek(lexer) != '"') {
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
        token.type = TOKEN_STRING;
    } else if (strchr("+-*/=<>!&|", c) != NULL) {
        token.value[0] = lexer_advance(lexer);
        token.value[1] = '\0';
        token.type = TOKEN_OPERATOR;
    } else {
        token.value[0] = lexer_advance(lexer);
        token.value[1] = '\0';
        token.type = TOKEN_UNKNOWN;
    }
    
    token.line = lexer->line;
    token.column = start_col;
    
    return token;
}

void parser_init(struct Parser *parser, struct Lexer *lexer) {
    parser->lexer = lexer;
    parser->current_token = lexer_next_token(lexer);
    parser->error_count = 0;
}

void parser_advance(struct Parser *parser) {
    parser->current_token = lexer_next_token(parser->lexer);
}

void parser_expect_identifier(struct Parser *parser) {
    if (parser->current_token.type != TOKEN_IDENTIFIER) {
        printf("Error at line %d, column %d: Expected identifier, got '%s'\n",
               parser->current_token.line, parser->current_token.column,
               parser->current_token.value);
        parser->error_count++;
    }
    parser_advance(parser);