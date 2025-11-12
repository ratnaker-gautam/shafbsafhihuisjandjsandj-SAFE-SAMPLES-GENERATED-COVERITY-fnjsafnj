//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64
#define MAX_INPUT_LEN 1024

enum TokenType {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_UNKNOWN
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
    uint32_t line;
    uint32_t column;
};

struct TokenizerState {
    char *input;
    uint32_t position;
    uint32_t line;
    uint32_t column;
    uint32_t token_count;
};

struct Parser {
    struct Token tokens[MAX_TOKENS];
    uint32_t current_token;
    uint32_t token_count;
};

void init_tokenizer(struct TokenizerState *state, char *input) {
    if (state == NULL || input == NULL) return;
    state->input = input;
    state->position = 0;
    state->line = 1;
    state->column = 1;
    state->token_count = 0;
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int is_delimiter_char(char c) {
    return strchr("(),;{}[]", c) != NULL;
}

void skip_whitespace(struct TokenizerState *state) {
    if (state == NULL || state->input == NULL) return;
    while (state->input[state->position] != '\0' && isspace(state->input[state->position])) {
        if (state->input[state->position] == '\n') {
            state->line++;
            state->column = 1;
        } else {
            state->column++;
        }
        state->position++;
    }
}

int read_token(struct TokenizerState *state, struct Token *token) {
    if (state == NULL || token == NULL || state->input == NULL) return 0;
    
    skip_whitespace(state);
    
    if (state->input[state->position] == '\0') {
        token->type = TOKEN_EOF;
        strcpy(token->value, "");
        token->line = state->line;
        token->column = state->column;
        return 1;
    }
    
    char current = state->input[state->position];
    uint32_t start_pos = state->position;
    uint32_t value_len = 0;
    
    if (isalpha(current) || current == '_') {
        token->type = TOKEN_IDENTIFIER;
        while ((state->input[state->position] != '\0') && 
               (isalnum(state->input[state->position]) || state->input[state->position] == '_') &&
               value_len < MAX_TOKEN_LEN - 1) {
            token->value[value_len++] = state->input[state->position++];
            state->column++;
        }
    } else if (isdigit(current)) {
        token->type = TOKEN_NUMBER;
        while ((state->input[state->position] != '\0') && 
               (isdigit(state->input[state->position]) || state->input[state->position] == '.') &&
               value_len < MAX_TOKEN_LEN - 1) {
            token->value[value_len++] = state->input[state->position++];
            state->column++;
        }
    } else if (current == '"') {
        token->type = TOKEN_STRING;
        state->position++;
        state->column++;
        while (state->input[state->position] != '\0' && 
               state->input[state->position] != '"' && 
               value_len < MAX_TOKEN_LEN - 1) {
            token->value[value_len++] = state->input[state->position++];
            state->column++;
        }
        if (state->input[state->position] == '"') {
            state->position++;
            state->column++;
        }
    } else if (is_operator_char(current)) {
        token->type = TOKEN_OPERATOR;
        while ((state->input[state->position] != '\0') && 
               is_operator_char(state->input[state->position]) &&
               value_len < MAX_TOKEN_LEN - 1) {
            token->value[value_len++] = state->input[state->position++];
            state->column++;
        }
    } else if (is_delimiter_char(current)) {
        token->type = TOKEN_DELIMITER;
        token->value[value_len++] = state->input[state->position++];
        state->column++;
    } else {
        token->type = TOKEN_UNKNOWN;
        token->value[value_len++] = state->input[state->position++];
        state->column++;
    }
    
    token->value[value_len] = '\0';
    token->line = state->line;
    token->column = state->column - value_len;
    
    return 1;
}

int tokenize(struct TokenizerState *state, struct Token tokens[], uint32_t max_tokens) {
    if (state == NULL || tokens == NULL || max_tokens == 0) return 0;
    
    uint32_t count = 0;
    struct Token token;
    
    while (count < max_tokens - 1) {
        if (!read_token(state, &