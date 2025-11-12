//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Token {
    char *value;
    int type;
    int line;
    int column;
};

struct TokenList {
    struct Token *tokens;
    size_t count;
    size_t capacity;
};

enum TokenType {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_UNKNOWN
};

struct LexerState {
    const char *input;
    size_t position;
    size_t line;
    size_t column;
};

void token_list_init(struct TokenList *list) {
    list->tokens = NULL;
    list->count = 0;
    list->capacity = 0;
}

int token_list_grow(struct TokenList *list) {
    size_t new_capacity = list->capacity == 0 ? 16 : list->capacity * 2;
    struct Token *new_tokens = realloc(list->tokens, new_capacity * sizeof(struct Token));
    if (!new_tokens) return 0;
    list->tokens = new_tokens;
    list->capacity = new_capacity;
    return 1;
}

void token_list_free(struct TokenList *list) {
    for (size_t i = 0; i < list->count; i++) {
        free(list->tokens[i].value);
    }
    free(list->tokens);
    list->tokens = NULL;
    list->count = 0;
    list->capacity = 0;
}

int token_list_add(struct TokenList *list, const char *value, int type, int line, int column) {
    if (list->count >= list->capacity) {
        if (!token_list_grow(list)) return 0;
    }
    list->tokens[list->count].value = malloc(strlen(value) + 1);
    if (!list->tokens[list->count].value) return 0;
    strcpy(list->tokens[list->count].value, value);
    list->tokens[list->count].type = type;
    list->tokens[list->count].line = line;
    list->tokens[list->count].column = column;
    list->count++;
    return 1;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

void lexer_init(struct LexerState *state, const char *input) {
    state->input = input;
    state->position = 0;
    state->line = 1;
    state->column = 1;
}

char lexer_peek(struct LexerState *state) {
    return state->input[state->position];
}

char lexer_advance(struct LexerState *state) {
    char c = state->input[state->position];
    if (c == '\n') {
        state->line++;
        state->column = 1;
    } else {
        state->column++;
    }
    state->position++;
    return c;
}

void lexer_skip_whitespace(struct LexerState *state) {
    while (isspace(lexer_peek(state))) {
        lexer_advance(state);
    }
}

int lexer_parse_string(struct LexerState *state, struct TokenList *tokens) {
    int start_line = state->line;
    int start_column = state->column;
    char buffer[256];
    size_t buf_pos = 0;
    lexer_advance(state);
    while (lexer_peek(state) != '"' && lexer_peek(state) != '\0') {
        if (buf_pos >= sizeof(buffer) - 1) return 0;
        buffer[buf_pos++] = lexer_advance(state);
    }
    if (lexer_peek(state) != '"') return 0;
    lexer_advance(state);
    buffer[buf_pos] = '\0';
    return token_list_add(tokens, buffer, TOKEN_STRING, start_line, start_column);
}

int lexer_parse_number(struct LexerState *state, struct TokenList *tokens) {
    int start_line = state->line;
    int start_column = state->column;
    char buffer[64];
    size_t buf_pos = 0;
    while (isdigit(lexer_peek(state))) {
        if (buf_pos >= sizeof(buffer) - 1) return 0;
        buffer[buf_pos++] = lexer_advance(state);
    }
    buffer[buf_pos] = '\0';
    return token_list_add(tokens, buffer, TOKEN_NUMBER, start_line, start_column);
}

int lexer_parse_identifier(struct LexerState *state, struct TokenList *tokens) {
    int start_line = state->line;
    int start_column = state->column;
    char buffer[128];
    size_t buf_pos = 0;
    while (isalnum(lexer_peek(state)) || lexer_peek(state) == '_') {
        if (buf_pos >= sizeof(buffer) - 1) return 0;
        buffer[buf_pos++] = lexer_advance(state);
    }
    buffer[buf_pos] = '\0';
    return token_list_add(tokens, buffer, TOKEN_IDENTIFIER, start_line, start_column);
}

int lexer_parse_operator(struct Lex