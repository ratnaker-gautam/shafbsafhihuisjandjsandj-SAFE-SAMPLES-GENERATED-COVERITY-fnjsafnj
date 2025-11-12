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
    struct ASTNode **children;
    size_t child_count;
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
    token.value = malloc(strlen(value) + 1);
    if (token.value) {
        strcpy(token.value, value);
    }
    token.line = line;
    token.column = column;
    return token;
}

void free_token(struct Token *token) {
    if (token && token->value) {
        free(token->value);
    }
}

int lexer_init(struct LexerState *state, const char *input) {
    if (!state || !input) return 0;
    state->input = input;
    state->position = 0;
    state->line = 1;
    state->column = 1;
    state->input_length = strlen(input);
    return 1;
}

struct Token lexer_next_token(struct LexerState *state) {
    if (!state || state->position >= state->input_length) {
        return create_token(TOKEN_EOF, "", state->line, state->column);
    }

    char current = state->input[state->position];
    while (isspace(current)) {
        if (current == '\n') {
            state->line++;
            state->column = 1;
        } else {
            state->column++;
        }
        state->position++;
        if (state->position >= state->input_length) {
            return create_token(TOKEN_EOF, "", state->line, state->column);
        }
        current = state->input[state->position];
    }

    size_t start_pos = state->position;
    size_t start_line = state->line;
    size_t start_col = state->column;

    if (isalpha(current) || current == '_') {
        while (state->position < state->input_length && 
               (isalnum(state->input[state->position]) || state->input[state->position] == '_')) {
            state->position++;
            state->column++;
        }
        size_t len = state->position - start_pos;
        char *value = malloc(len + 1);
        if (value) {
            strncpy(value, state->input + start_pos, len);
            value[len] = '\0';
            enum TokenType type = is_keyword(value) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
            struct Token token = create_token(type, value, start_line, start_col);
            free(value);
            return token;
        }
    } else if (isdigit(current)) {
        while (state->position < state->input_length && isdigit(state->input[state->position])) {
            state->position++;
            state->column++;
        }
        size_t len = state->position - start_pos;
        char *value = malloc(len + 1);
        if (value) {
            strncpy(value, state->input + start_pos, len);
            value[len] = '\0';
            struct Token token = create_token(TOKEN_NUMBER, value, start_line, start_col);
            free(value);
            return token;
        }
    } else if (current == '"') {
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
        size_t len = state->position - start_pos;
        char *value = malloc(len + 1);
        if (value) {
            strncpy(value, state->input + start_pos, len);
            value[len] = '\0';
            struct Token token = create_token(TOKEN_STRING, value, start_line, start_col);
            free(value);
            return token;
        }
    } else if (strchr("+-*/=<>!&|", current)) {
        state->position++;
        state->column++;
        if (state->position < state