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
    TOKEN_UNKNOWN
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
    struct ASTNode *left;
    struct ASTNode *right;
};

int is_keyword(const char *str) {
    static const char *keywords[] = {"if", "else", "while", "for", "return", "int", "float", "char", "void", NULL};
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
    token.value = malloc(strlen(value) + 1);
    if (token.value != NULL) {
        strcpy(token.value, value);
    }
    token.line = line;
    token.column = column;
    return token;
}

void free_token(struct Token *token) {
    if (token != NULL && token->value != NULL) {
        free(token->value);
    }
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int read_next_token(struct LexerState *state, struct Token *token) {
    if (state == NULL || token == NULL || state->input == NULL) {
        return 0;
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
        *token = create_token(TOKEN_EOF, "", state->line, state->column);
        return 1;
    }

    char current = state->input[state->position];
    size_t start_line = state->line;
    size_t start_column = state->column;

    if (isalpha(current) || current == '_') {
        size_t start = state->position;
        while (state->position < state->input_length && (isalnum(state->input[state->position]) || state->input[state->position] == '_')) {
            state->position++;
            state->column++;
        }
        size_t length = state->position - start;
        char *value = malloc(length + 1);
        if (value == NULL) {
            return 0;
        }
        strncpy(value, state->input + start, length);
        value[length] = '\0';
        enum TokenType type = is_keyword(value) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
        *token = create_token(type, value, start_line, start_column);
        free(value);
        return 1;
    } else if (isdigit(current)) {
        size_t start = state->position;
        while (state->position < state->input_length && isdigit(state->input[state->position])) {
            state->position++;
            state->column++;
        }
        if (state->position < state->input_length && state->input[state->position] == '.') {
            state->position++;
            state->column++;
            while (state->position < state->input_length && isdigit(state->input[state->position])) {
                state->position++;
                state->column++;
            }
        }
        size_t length = state->position - start;
        char *value = malloc(length + 1);
        if (value == NULL) {
            return 0;
        }
        strncpy(value, state->input + start, length);
        value[length] = '\0';
        *token = create_token(TOKEN_NUMBER, value, start_line, start_column);
        free(value);
        return 1;
    } else if (current == '"') {
        size_t start = state->position;
        state->position++;
        state->column++;
        while (state->position < state->input_length && state->input[state->position] != '"') {
            if (state->input[state->position] == '\n') {
                state->line++;
                state->column = 1;
            } else {
                state->column++;
            }
            state->position++;
        }
        if (state->position < state->input_length && state->input[state->position] == '"') {
            state->position++;
            state->column++;
        }
        size_t length = state->position - start;
        char *value = malloc(length + 1);
        if (value == NULL) {
            return 0;
        }
        strncpy(value, state->input + start,