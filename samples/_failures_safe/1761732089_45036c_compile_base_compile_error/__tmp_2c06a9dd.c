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

int lexer_is_at_end(struct Lexer *lexer) {
    if (lexer == NULL) return 1;
    return lexer->position >= lexer->length;
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
    while (!lexer_is_at_end(lexer) && isspace(lexer_peek(lexer))) {
        lexer_advance(lexer);
    }
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

void token_init(struct Token *token, enum TokenType type, const char *value, int line, int column) {
    if (token == NULL || value == NULL) return;
    token->type = type;
    strncpy(token->value, value, sizeof(token->value) - 1);
    token->value[sizeof(token->value) - 1] = '\0';
    token->line = line;
    token->column = column;
}

struct Token lexer_next_token(struct Lexer *lexer) {
    struct Token token;
    if (lexer == NULL) {
        token_init(&token, TOKEN_EOF, "", 0, 0);
        return token;
    }
    
    lexer_skip_whitespace(lexer);
    
    if (lexer_is_at_end(lexer)) {
        token_init(&token, TOKEN_EOF, "", lexer->line, lexer->column);
        return token;
    }
    
    char c = lexer_peek(lexer);
    int start_line = lexer->line;
    int start_column = lexer->column;
    
    if (isalpha(c) || c == '_') {
        char identifier[64] = {0};
        int i = 0;
        while (!lexer_is_at_end(lexer) && (isalnum(lexer_peek(lexer)) || lexer_peek(lexer) == '_')) {
            if (i < 63) {
                identifier[i++] = lexer_advance(lexer);
            } else {
                lexer_advance(lexer);
            }
        }
        identifier[i] = '\0';
        token_init(&token, TOKEN_IDENTIFIER, identifier, start_line, start_column);
    } else if (isdigit(c)) {
        char number[64] = {0};
        int i = 0;
        while (!lexer_is_at_end(lexer) && isdigit(lexer_peek(lexer))) {
            if (i < 63) {
                number[i++] = lexer_advance(lexer);
            } else {
                lexer_advance(lexer);
            }
        }
        number[i] = '\0';
        token_init(&token, TOKEN_NUMBER, number, start_line, start_column);
    } else if (is_operator_char(c)) {
        char op[3] = {0};
        op[0] = lexer_advance(lexer);
        if (!lexer_is_at_end(lexer) && is_operator_char(lexer_peek(lexer))) {
            op[1] = lexer_advance(lexer);
        }
        token_init(&token, TOKEN_OPERATOR, op, start_line, start_column);
    } else if (c == '"') {
        char string[64] = {0};
        int i = 0;
        lexer_advance(lexer);
        while (!lexer_is_at_end(lexer) && lexer_peek(lexer) != '"') {
            if (i < 63) {
                string[i++] = lexer_advance(lexer);
            } else {
                lexer_advance(lexer);
            }
        }
        if (!lexer