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
    TOKEN_STRING,
    TOKEN_PUNCTUATION,
    TOKEN_EOF,
    TOKEN_INVALID
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

struct ParserResult {
    struct Token tokens[MAX_TOKENS];
    uint32_t token_count;
    uint32_t valid_tokens;
    uint32_t error_count;
};

static int is_operator_char(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || 
            c == '=' || c == '<' || c == '>' || c == '!' || 
            c == '&' || c == '|');
}

static int is_punctuation_char(char c) {
    return (c == '(' || c == ')' || c == '{' || c == '}' || 
            c == '[' || c == ']' || c == ',' || c == ';' || 
            c == ':' || c == '.');
}

static void init_tokenizer(struct TokenizerState *state, char *input) {
    if (state == NULL || input == NULL) return;
    state->input = input;
    state->position = 0;
    state->line = 1;
    state->column = 1;
    state->token_count = 0;
}

static void skip_whitespace(struct TokenizerState *state) {
    if (state == NULL || state->input == NULL) return;
    
    while (state->input[state->position] != '\0') {
        char c = state->input[state->position];
        if (c == ' ' || c == '\t') {
            state->position++;
            state->column++;
        } else if (c == '\n') {
            state->position++;
            state->line++;
            state->column = 1;
        } else {
            break;
        }
    }
}

static struct Token get_next_token(struct TokenizerState *state) {
    struct Token token = {TOKEN_INVALID, "", state->line, state->column};
    if (state == NULL || state->input == NULL) return token;
    
    skip_whitespace(state);
    
    if (state->input[state->position] == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char current = state->input[state->position];
    uint32_t start_pos = state->position;
    uint32_t value_len = 0;
    
    if (isalpha(current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        while (state->input[state->position] != '\0' && 
               (isalnum(state->input[state->position]) || 
                state->input[state->position] == '_') &&
               value_len < MAX_TOKEN_LEN - 1) {
            token.value[value_len++] = state->input[state->position++];
            state->column++;
        }
    } else if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        while (state->input[state->position] != '\0' && 
               isdigit(state->input[state->position]) &&
               value_len < MAX_TOKEN_LEN - 1) {
            token.value[value_len++] = state->input[state->position++];
            state->column++;
        }
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        state->position++;
        state->column++;
        while (state->input[state->position] != '\0' && 
               state->input[state->position] != '"' &&
               value_len < MAX_TOKEN_LEN - 1) {
            token.value[value_len++] = state->input[state->position++];
            state->column++;
        }
        if (state->input[state->position] == '"') {
            state->position++;
            state->column++;
        }
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        while (state->input[state->position] != '\0' && 
               is_operator_char(state->input[state->position]) &&
               value_len < MAX_TOKEN_LEN - 1) {
            token.value[value_len++] = state->input[state->position++];
            state->column++;
        }
    } else if (is_punctuation_char(current)) {
        token.type = TOKEN_PUNCTUATION;
        token.value[value_len++] = state->input[state->position++];
        state->column++;
    } else {
        token.type = TOKEN_INVALID;
        token.value[value_len++] = state->input[state->position++];
        state->column++;
    }
    
    token.value[value_len] = '\0';
    state->token_count++;
    return token;
}

static struct ParserResult parse_input(char *input) {
    struct ParserResult result = {0};
    if (input ==