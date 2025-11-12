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
    TOKEN_PUNCTUATION,
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

struct ParserNode {
    enum { NODE_EXPRESSION, NODE_STATEMENT, NODE_DECLARATION } type;
    struct Token token;
    struct ParserNode *left;
    struct ParserNode *right;
};

struct ParserState {
    struct Token *tokens;
    size_t token_count;
    size_t current_token;
};

int is_keyword(const char *str) {
    static const char *keywords[] = {"if", "else", "while", "for", "return", "int", "char", "void", NULL};
    for (int i = 0; keywords[i] != NULL; i++) {
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
    if (value != NULL) {
        token.value = malloc(strlen(value) + 1);
        if (token.value != NULL) {
            strcpy(token.value, value);
        }
    } else {
        token.value = NULL;
    }
    return token;
}

void free_token(struct Token *token) {
    if (token != NULL && token->value != NULL) {
        free(token->value);
    }
}

int lexer_init(struct LexerState *state, const char *input) {
    if (state == NULL || input == NULL) return 0;
    state->input = input;
    state->position = 0;
    state->line = 1;
    state->column = 1;
    state->input_length = strlen(input);
    return 1;
}

struct Token lexer_next_token(struct LexerState *state) {
    if (state == NULL || state->position >= state->input_length) {
        return create_token(TOKEN_EOF, "", state->line, state->column);
    }

    while (state->position < state->input_length && isspace(state->input[state->position])) {
        if (state->input[state->position] == '\n') {
            state->line++;
            state->column = 1;
        } else {
            state->column++;
        }
        state->position++;
    }

    if (state->position >= state->input_length) {
        return create_token(TOKEN_EOF, "", state->line, state->column);
    }

    char current = state->input[state->position];
    size_t start_pos = state->position;
    size_t start_line = state->line;
    size_t start_col = state->column;

    if (isalpha(current) || current == '_') {
        while (state->position < state->input_length && (isalnum(state->input[state->position]) || state->input[state->position] == '_')) {
            state->position++;
            state->column++;
        }
        size_t len = state->position - start_pos;
        char *ident = malloc(len + 1);
        if (ident == NULL) return create_token(TOKEN_INVALID, "", start_line, start_col);
        strncpy(ident, state->input + start_pos, len);
        ident[len] = '\0';
        struct Token token = create_token(is_keyword(ident) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER, ident, start_line, start_col);
        free(ident);
        return token;
    } else if (isdigit(current)) {
        while (state->position < state->input_length && isdigit(state->input[state->position])) {
            state->position++;
            state->column++;
        }
        size_t len = state->position - start_pos;
        char *num = malloc(len + 1);
        if (num == NULL) return create_token(TOKEN_INVALID, "", start_line, start_col);
        strncpy(num, state->input + start_pos, len);
        num[len] = '\0';
        struct Token token = create_token(TOKEN_NUMBER, num, start_line, start_col);
        free(num);
        return token;
    } else if (current == '"') {
        state->position++;
        state->column++;
        start_pos = state->position;
        while (state->position < state->input_length && state->input[state->position] != '"') {
            state->position++;
            state->column++;
        }
        if (state->position >= state->input_length) {
            return create_token(TOKEN_INVALID, "", start_line, start_col);
        }
        size_t len = state->position - start_pos;
        char *str = malloc(len + 1);
        if (str == NULL) return create_token(TOKEN_INVALID, "", start_line, start_col);
        strncpy(str, state->input + start_pos, len);