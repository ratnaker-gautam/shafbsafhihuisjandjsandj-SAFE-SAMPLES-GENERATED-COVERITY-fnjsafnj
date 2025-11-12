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

struct ParserResult {
    struct Token tokens[MAX_TOKENS];
    uint32_t token_count;
    int valid;
    char error_msg[128];
};

static int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

static int is_delimiter_char(char c) {
    return strchr("(),;{}[]", c) != NULL;
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
    struct Token token = {TOKEN_UNKNOWN, "", state->line, state->column};
    if (state == NULL || state->input == NULL || state->token_count >= MAX_TOKENS) {
        token.type = TOKEN_EOF;
        return token;
    }
    
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
        while ((isalnum(state->input[state->position]) || state->input[state->position] == '_') && 
               value_len < MAX_TOKEN_LEN - 1) {
            token.value[value_len++] = state->input[state->position++];
            state->column++;
        }
    } else if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        while (isdigit(state->input[state->position]) && value_len < MAX_TOKEN_LEN - 1) {
            token.value[value_len++] = state->input[state->position++];
            state->column++;
        }
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        state->position++;
        state->column++;
        while (state->input[state->position] != '"' && state->input[state->position] != '\0' && 
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
        while (is_operator_char(state->input[state->position]) && value_len < MAX_TOKEN_LEN - 1) {
            token.value[value_len++] = state->input[state->position++];
            state->column++;
        }
    } else if (is_delimiter_char(current)) {
        token.type = TOKEN_DELIMITER;
        token.value[value_len++] = state->input[state->position++];
        state->column++;
    } else {
        token.type = TOKEN_UNKNOWN;
        token.value[value_len++] = state->input[state->position++];
        state->column++;
    }
    
    token.value[value_len] = '\0';
    state->token_count++;
    return token;
}

static void tokenize_input(struct TokenizerState *state, struct ParserResult *result) {
    if (state == NULL || result == NULL) return;
    
    result->token_count = 0;
    result->valid = 1;
    strcpy(result->error_msg, "");
    
    while (result->token_count < MAX_TOKENS) {
        struct Token token = get_next_token(state);
        
        if (token.type == TOKEN_EOF) {
            break;
        }
        
        if (token.type == TOKEN_UNKNOWN) {
            result->valid = 0;
            snprintf(result->error_msg, sizeof(result->error_msg