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

struct ParserNode {
    enum TokenType node_type;
    char *value;
    struct ParserNode *left;
    struct ParserNode *right;
};

struct TokenList {
    struct Token *tokens;
    size_t count;
    size_t capacity;
};

void init_lexer_state(struct LexerState *state, const char *input) {
    if (state == NULL || input == NULL) return;
    state->input = input;
    state->position = 0;
    state->line = 1;
    state->column = 1;
    state->input_length = strlen(input);
}

void init_token_list(struct TokenList *list) {
    if (list == NULL) return;
    list->tokens = NULL;
    list->count = 0;
    list->capacity = 0;
}

void free_token_list(struct TokenList *list) {
    if (list == NULL) return;
    for (size_t i = 0; i < list->count; i++) {
        free(list->tokens[i].value);
    }
    free(list->tokens);
    list->tokens = NULL;
    list->count = 0;
    list->capacity = 0;
}

int add_token(struct TokenList *list, struct Token token) {
    if (list == NULL) return 0;
    if (list->count >= list->capacity) {
        size_t new_capacity = list->capacity == 0 ? 16 : list->capacity * 2;
        if (new_capacity <= list->capacity) return 0;
        struct Token *new_tokens = realloc(list->tokens, new_capacity * sizeof(struct Token));
        if (new_tokens == NULL) return 0;
        list->tokens = new_tokens;
        list->capacity = new_capacity;
    }
    list->tokens[list->count] = token;
    list->count++;
    return 1;
}

int is_keyword(const char *str) {
    static const char *keywords[] = {"if", "else", "while", "for", "return", "int", "float", "char", NULL};
    for (size_t i = 0; keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0) return 1;
    }
    return 0;
}

struct Token create_token(enum TokenType type, const char *value, size_t line, size_t column) {
    struct Token token;
    token.type = type;
    token.value = NULL;
    token.line = line;
    token.column = column;
    if (value != NULL) {
        token.value = malloc(strlen(value) + 1);
        if (token.value != NULL) {
            strcpy(token.value, value);
        }
    }
    return token;
}

int tokenize(struct LexerState *state, struct TokenList *tokens) {
    if (state == NULL || tokens == NULL) return 0;
    while (state->position < state->input_length) {
        char current = state->input[state->position];
        if (isspace(current)) {
            if (current == '\n') {
                state->line++;
                state->column = 1;
            } else {
                state->column++;
            }
            state->position++;
            continue;
        }
        if (isalpha(current) || current == '_') {
            size_t start = state->position;
            while (state->position < state->input_length && (isalnum(state->input[state->position]) || state->input[state->position] == '_')) {
                state->position++;
                state->column++;
            }
            size_t length = state->position - start;
            if (length > 256) length = 256;
            char *ident = malloc(length + 1);
            if (ident == NULL) return 0;
            strncpy(ident, state->input + start, length);
            ident[length] = '\0';
            enum TokenType type = is_keyword(ident) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
            struct Token token = create_token(type, ident, state->line, state->column - length);
            free(ident);
            if (!add_token(tokens, token)) return 0;
        } else if (isdigit(current)) {
            size_t start = state->position;
            while (state->position < state->input_length && isdigit(state->input[state->position])) {
                state->position++;
                state->column++;
            }
            size_t length = state->position - start;
            if (length > 256) length = 256;
            char *num = malloc(length + 1);
            if (num == NULL) return 0;
            strncpy(num, state->input + start, length);
            num[length] = '\0';
            struct Token token = create_token(TOKEN_NUMBER, num, state