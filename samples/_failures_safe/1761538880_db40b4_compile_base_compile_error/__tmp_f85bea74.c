//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Token {
    char *value;
    int type;
    size_t line;
    size_t column;
};

struct TokenList {
    struct Token *tokens;
    size_t count;
    size_t capacity;
};

struct ParserState {
    const char *input;
    size_t position;
    size_t line;
    size_t column;
    struct TokenList tokens;
};

enum TokenType {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_ERROR
};

void token_list_init(struct TokenList *list) {
    list->tokens = NULL;
    list->count = 0;
    list->capacity = 0;
}

int token_list_grow(struct TokenList *list) {
    size_t new_capacity = list->capacity == 0 ? 8 : list->capacity * 2;
    struct Token *new_tokens = realloc(list->tokens, new_capacity * sizeof(struct Token));
    if (new_tokens == NULL) return 0;
    list->tokens = new_tokens;
    list->capacity = new_capacity;
    return 1;
}

int token_list_add(struct TokenList *list, const char *value, int type, size_t line, size_t column) {
    if (list->count >= list->capacity) {
        if (!token_list_grow(list)) return 0;
    }
    list->tokens[list->count].value = malloc(strlen(value) + 1);
    if (list->tokens[list->count].value == NULL) return 0;
    strcpy(list->tokens[list->count].value, value);
    list->tokens[list->count].type = type;
    list->tokens[list->count].line = line;
    list->tokens[list->count].column = column;
    list->count++;
    return 1;
}

void token_list_cleanup(struct TokenList *list) {
    for (size_t i = 0; i < list->count; i++) {
        free(list->tokens[i].value);
    }
    free(list->tokens);
    list->tokens = NULL;
    list->count = 0;
    list->capacity = 0;
}

void parser_state_init(struct ParserState *state, const char *input) {
    state->input = input;
    state->position = 0;
    state->line = 1;
    state->column = 1;
    token_list_init(&state->tokens);
}

void parser_state_cleanup(struct ParserState *state) {
    token_list_cleanup(&state->tokens);
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

char parser_peek(struct ParserState *state) {
    return state->input[state->position];
}

char parser_advance(struct ParserState *state) {
    char c = state->input[state->position];
    if (c == '\0') return c;
    state->position++;
    if (c == '\n') {
        state->line++;
        state->column = 1;
    } else {
        state->column++;
    }
    return c;
}

void parser_skip_whitespace(struct ParserState *state) {
    while (isspace(parser_peek(state))) {
        parser_advance(state);
    }
}

int parse_identifier(struct ParserState *state) {
    size_t start = state->position;
    size_t line = state->line;
    size_t column = state->column;
    while (isalnum(parser_peek(state)) || parser_peek(state) == '_') {
        parser_advance(state);
    }
    size_t length = state->position - start;
    if (length == 0) return 0;
    char *value = malloc(length + 1);
    if (value == NULL) return 0;
    strncpy(value, state->input + start, length);
    value[length] = '\0';
    int result = token_list_add(&state->tokens, value, TOKEN_IDENTIFIER, line, column);
    free(value);
    return result;
}

int parse_number(struct ParserState *state) {
    size_t start = state->position;
    size_t line = state->line;
    size_t column = state->column;
    while (isdigit(parser_peek(state))) {
        parser_advance(state);
    }
    size_t length = state->position - start;
    if (length == 0) return 0;
    char *value = malloc(length + 1);
    if (value == NULL) return 0;
    strncpy(value, state->input + start, length);
    value[length] = '\0';
    int result = token_list_add(&state->tokens, value, TOKEN_NUMBER, line, column);
    free(value);
    return result;
}

int parse_operator(struct ParserState *state) {
    size_t line = state->line;
    size_t column = state->column;
    char op[2] = { parser_advance(state), '\0' };
    return token_list_add(&state->tokens, op, TOKEN_OPERATOR, line, column);
}

int parse_string(struct ParserState *