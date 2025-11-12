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
    char *value;
    size_t line;
    size_t column;
};

struct LexerState {
    const char *input;
    size_t position;
    size_t line;
    size_t column;
    size_t input_length;
};

struct ParserState {
    struct Token *tokens;
    size_t token_count;
    size_t current_token;
};

struct ASTNode {
    char *type;
    char *value;
    struct ASTNode **children;
    size_t child_count;
};

int is_keyword(const char *str) {
    static const char *keywords[] = {"if", "else", "while", "for", "return", "int", "char", "void", NULL};
    for (size_t i = 0; keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

struct Token create_token(enum TokenType type, const char *value, size_t line, size_t column) {
    struct Token token;
    token.type = type;
    token.line = line;
    token.column = column;
    token.value = malloc(strlen(value) + 1);
    if (token.value != NULL) {
        strcpy(token.value, value);
    }
    return token;
}

void free_token(struct Token *token) {
    if (token != NULL && token->value != NULL) {
        free(token->value);
    }
}

int lexer_advance(struct LexerState *state) {
    if (state->position >= state->input_length) {
        return 0;
    }
    if (state->input[state->position] == '\n') {
        state->line++;
        state->column = 1;
    } else {
        state->column++;
    }
    state->position++;
    return 1;
}

char lexer_peek(const struct LexerState *state) {
    if (state->position >= state->input_length) {
        return '\0';
    }
    return state->input[state->position];
}

struct Token lexer_next_token(struct LexerState *state) {
    while (state->position < state->input_length && isspace(lexer_peek(state))) {
        lexer_advance(state);
    }
    if (state->position >= state->input_length) {
        return create_token(TOKEN_EOF, "", state->line, state->column);
    }
    char current = lexer_peek(state);
    size_t start_line = state->line;
    size_t start_column = state->column;
    if (isalpha(current) || current == '_') {
        char buffer[256];
        size_t i = 0;
        while ((state->position < state->input_length) && (isalnum(current) || current == '_') && i < 255) {
            buffer[i++] = current;
            lexer_advance(state);
            current = lexer_peek(state);
        }
        buffer[i] = '\0';
        if (is_keyword(buffer)) {
            return create_token(TOKEN_KEYWORD, buffer, start_line, start_column);
        } else {
            return create_token(TOKEN_IDENTIFIER, buffer, start_line, start_column);
        }
    } else if (isdigit(current)) {
        char buffer[256];
        size_t i = 0;
        while ((state->position < state->input_length) && (isdigit(current) || current == '.') && i < 255) {
            buffer[i++] = current;
            lexer_advance(state);
            current = lexer_peek(state);
        }
        buffer[i] = '\0';
        return create_token(TOKEN_NUMBER, buffer, start_line, start_column);
    } else if (current == '"') {
        char buffer[256];
        size_t i = 0;
        lexer_advance(state);
        current = lexer_peek(state);
        while ((state->position < state->input_length) && (current != '"') && i < 255) {
            buffer[i++] = current;
            lexer_advance(state);
            current = lexer_peek(state);
        }
        buffer[i] = '\0';
        if (current == '"') {
            lexer_advance(state);
        }
        return create_token(TOKEN_STRING, buffer, start_line, start_column);
    } else if (strchr("+-*/=<>!&|", current) != NULL) {
        char buffer[3] = {current, '\0', '\0'};
        lexer_advance(state);
        char next = lexer_peek(state);
        if (strchr("=+-", next) != NULL) {
            buffer[1] = next;
            buffer[2] = '\0';
            lexer_advance(state);
        }
        return create_token(TOKEN_OPERATOR, buffer, start_line, start_column);
    } else if (strchr("(),;{}[]", current) != NULL) {
        char buffer[2] = {current, '\0'};
        lexer_advance(state);