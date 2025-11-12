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
    TOKEN_ERROR
};

struct Token {
    enum TokenType type;
    char value[32];
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

void token_init(struct Token *token, enum TokenType type, const char *value, int line, int column) {
    token->type = type;
    strncpy(token->value, value, sizeof(token->value) - 1);
    token->value[sizeof(token->value) - 1] = '\0';
    token->line = line;
    token->column = column;
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

void lexer_skip_whitespace(struct Lexer *lexer) {
    while (!lexer_is_at_end(lexer)) {
        char c = lexer_peek(lexer);
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            lexer_advance(lexer);
        } else {
            break;
        }
    }
}

struct Token lexer_next_token(struct Lexer *lexer) {
    lexer_skip_whitespace(lexer);
    
    if (lexer_is_at_end(lexer)) {
        struct Token token;
        token_init(&token, TOKEN_EOF, "", lexer->line, lexer->column);
        return token;
    }
    
    char c = lexer_peek(lexer);
    int start_line = lexer->line;
    int start_column = lexer->column;
    
    if (isalpha(c) || c == '_') {
        char buffer[32] = {0};
        int i = 0;
        while (!lexer_is_at_end(lexer) && (isalnum(c) || c == '_')) {
            if (i < (int)sizeof(buffer) - 1) {
                buffer[i++] = c;
            }
            lexer_advance(lexer);
            c = lexer_peek(lexer);
        }
        buffer[i] = '\0';
        struct Token token;
        token_init(&token, TOKEN_IDENTIFIER, buffer, start_line, start_column);
        return token;
    }
    
    if (isdigit(c)) {
        char buffer[32] = {0};
        int i = 0;
        while (!lexer_is_at_end(lexer) && isdigit(c)) {
            if (i < (int)sizeof(buffer) - 1) {
                buffer[i++] = c;
            }
            lexer_advance(lexer);
            c = lexer_peek(lexer);
        }
        buffer[i] = '\0';
        struct Token token;
        token_init(&token, TOKEN_NUMBER, buffer, start_line, start_column);
        return token;
    }
    
    if (c == '"') {
        char buffer[32] = {0};
        int i = 0;
        lexer_advance(lexer);
        c = lexer_peek(lexer);
        while (!lexer_is_at_end(lexer) && c != '"') {
            if (i < (int)sizeof(buffer) - 1) {
                buffer[i++] = c;
            }
            lexer_advance(lexer);
            c = lexer_peek(lexer);
        }
        if (c == '"') {
            lexer_advance(lexer);
        }
        buffer[i] = '\0';
        struct Token token;
        token_init(&token, TOKEN_STRING, buffer, start_line, start_column);
        return token;
    }
    
    if (is_operator_char(c)) {
        char buffer[3] = {0};
        buffer[0] = c;
        lexer_advance(lexer);
        c = lexer_peek(lexer);
        if (is_operator_char(c) && strlen(buffer) < sizeof(buffer) -