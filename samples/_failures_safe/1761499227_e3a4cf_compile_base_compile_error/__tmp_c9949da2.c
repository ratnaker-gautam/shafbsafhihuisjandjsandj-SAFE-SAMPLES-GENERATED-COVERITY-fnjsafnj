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

struct TokenList {
    struct Token *tokens;
    size_t count;
    size_t capacity;
};

const char *KEYWORDS[] = {
    "if", "else", "while", "for", "return", "int", "char", "void"
};
const size_t NUM_KEYWORDS = 8;

int is_keyword(const char *str) {
    for (size_t i = 0; i < NUM_KEYWORDS; i++) {
        if (strcmp(str, KEYWORDS[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void init_lexer_state(struct LexerState *state, const char *input) {
    state->input = input;
    state->position = 0;
    state->line = 1;
    state->column = 1;
    state->input_length = strlen(input);
}

void init_token_list(struct TokenList *list) {
    list->tokens = NULL;
    list->count = 0;
    list->capacity = 0;
}

int add_token(struct TokenList *list, enum TokenType type, const char *value, size_t line, size_t column) {
    if (list->count >= list->capacity) {
        size_t new_capacity = list->capacity == 0 ? 16 : list->capacity * 2;
        struct Token *new_tokens = realloc(list->tokens, new_capacity * sizeof(struct Token));
        if (new_tokens == NULL) return 0;
        list->tokens = new_tokens;
        list->capacity = new_capacity;
    }
    
    list->tokens[list->count].type = type;
    list->tokens[list->count].value = malloc(strlen(value) + 1);
    if (list->tokens[list->count].value == NULL) return 0;
    strcpy(list->tokens[list->count].value, value);
    list->tokens[list->count].line = line;
    list->tokens[list->count].column = column;
    list->count++;
    return 1;
}

void free_token_list(struct TokenList *list) {
    for (size_t i = 0; i < list->count; i++) {
        free(list->tokens[i].value);
    }
    free(list->tokens);
    list->tokens = NULL;
    list->count = 0;
    list->capacity = 0;
}

char peek_char(const struct LexerState *state) {
    if (state->position >= state->input_length) return '\0';
    return state->input[state->position];
}

char next_char(struct LexerState *state) {
    if (state->position >= state->input_length) return '\0';
    char c = state->input[state->position++];
    if (c == '\n') {
        state->line++;
        state->column = 1;
    } else {
        state->column++;
    }
    return c;
}

void skip_whitespace(struct LexerState *state) {
    while (state->position < state->input_length && isspace(peek_char(state))) {
        next_char(state);
    }
}

int parse_identifier(struct LexerState *state, struct TokenList *tokens) {
    size_t start = state->position;
    size_t line = state->line;
    size_t column = state->column;
    
    while (state->position < state->input_length && (isalnum(peek_char(state)) || peek_char(state) == '_')) {
        next_char(state);
    }
    
    size_t length = state->position - start;
    if (length == 0) return 0;
    
    char *value = malloc(length + 1);
    if (value == NULL) return 0;
    strncpy(value, state->input + start, length);
    value[length] = '\0';
    
    enum TokenType type = is_keyword(value) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
    int result = add_token(tokens, type, value, line, column);
    free(value);
    return result;
}

int parse_number(struct LexerState *state, struct TokenList *tokens) {
    size_t start = state->position;
    size_t line = state->line;
    size_t column = state->column;
    
    while (state->position < state->input_length && isdigit(peek_char(state))) {
        next_char(state);
    }
    
    size_t length = state->position - start;
    if (length == 0) return 0;
    
    char *value = malloc(length + 1);
    if (value == NULL) return 0;
    strncpy(value, state->input + start, length);
    value[length] = '\0';
    
    int result = add_token(tokens, TOKEN_NUMBER, value