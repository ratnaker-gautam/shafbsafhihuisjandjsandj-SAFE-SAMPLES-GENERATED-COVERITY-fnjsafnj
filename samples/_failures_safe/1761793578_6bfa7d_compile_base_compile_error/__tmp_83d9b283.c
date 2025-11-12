//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

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

struct ParserState {
    struct Token *tokens;
    uint32_t token_index;
    uint32_t token_count;
};

void initialize_tokenizer(struct TokenizerState *state, char *input) {
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
    return strchr("(){}[],;", c) != NULL;
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

int get_next_token(struct TokenizerState *state, struct Token *token) {
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
    token->line = state->line;
    token->column = state->column;
    
    if (isalpha(current) || current == '_') {
        token->type = TOKEN_IDENTIFIER;
        int i = 0;
        while ((isalnum(current) || current == '_') && i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = current;
            state->position++;
            state->column++;
            current = state->input[state->position];
        }
        token->value[i] = '\0';
    } else if (isdigit(current)) {
        token->type = TOKEN_NUMBER;
        int i = 0;
        while (isdigit(current) && i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = current;
            state->position++;
            state->column++;
            current = state->input[state->position];
        }
        token->value[i] = '\0';
    } else if (current == '"') {
        token->type = TOKEN_STRING;
        int i = 0;
        state->position++;
        state->column++;
        current = state->input[state->position];
        while (current != '"' && current != '\0' && i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = current;
            state->position++;
            state->column++;
            current = state->input[state->position];
        }
        token->value[i] = '\0';
        if (current == '"') {
            state->position++;
            state->column++;
        }
    } else if (is_operator_char(current)) {
        token->type = TOKEN_OPERATOR;
        int i = 0;
        while (is_operator_char(current) && i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = current;
            state->position++;
            state->column++;
            current = state->input[state->position];
        }
        token->value[i] = '\0';
    } else if (is_delimiter_char(current)) {
        token->type = TOKEN_DELIMITER;
        token->value[0] = current;
        token->value[1] = '\0';
        state->position++;
        state->column++;
    } else {
        token->type = TOKEN_UNKNOWN;
        token->value[0] = current;
        token->value[1] = '\0';
        state->position++;
        state->column++;
    }
    
    state->token_count++;
    return 1;
}

void initialize_parser(struct ParserState *parser, struct Token *tokens, uint32_t count) {
    if (parser == NULL || tokens == NULL) return;
    parser->tokens = tokens;
    parser->token_index = 0;
    parser->token_count = count;
}

struct Token *peek_token(struct ParserState *parser) {
    if (parser == NULL || parser->tokens == NULL) return NULL;
    if (parser->token_index >= parser->token_count) return NULL;
    return &parser->t