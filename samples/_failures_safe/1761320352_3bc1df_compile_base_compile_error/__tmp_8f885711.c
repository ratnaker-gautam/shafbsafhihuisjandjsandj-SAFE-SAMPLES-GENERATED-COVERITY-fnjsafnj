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
    uint32_t error_count;
};

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int is_delimiter_char(char c) {
    return strchr("(){}[],;:", c) != NULL;
}

void init_tokenizer(struct TokenizerState *state, char *input) {
    if (state == NULL || input == NULL) return;
    state->input = input;
    state->position = 0;
    state->line = 1;
    state->column = 1;
    state->token_count = 0;
}

void skip_whitespace(struct TokenizerState *state) {
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

int read_string(struct TokenizerState *state, struct Token *token) {
    if (state == NULL || token == NULL || state->input == NULL) return 0;
    
    if (state->input[state->position] != '"') return 0;
    
    token->type = TOKEN_STRING;
    token->line = state->line;
    token->column = state->column;
    
    uint32_t len = 0;
    state->position++;
    state->column++;
    
    while (state->input[state->position] != '\0' && 
           state->input[state->position] != '"' && 
           len < MAX_TOKEN_LEN - 1) {
        token->value[len++] = state->input[state->position++];
        state->column++;
    }
    
    if (state->input[state->position] == '"') {
        state->position++;
        state->column++;
    }
    
    token->value[len] = '\0';
    return 1;
}

int read_number(struct TokenizerState *state, struct Token *token) {
    if (state == NULL || token == NULL || state->input == NULL) return 0;
    
    if (!isdigit(state->input[state->position])) return 0;
    
    token->type = TOKEN_NUMBER;
    token->line = state->line;
    token->column = state->column;
    
    uint32_t len = 0;
    while (state->input[state->position] != '\0' && 
           isdigit(state->input[state->position]) && 
           len < MAX_TOKEN_LEN - 1) {
        token->value[len++] = state->input[state->position++];
        state->column++;
    }
    
    token->value[len] = '\0';
    return 1;
}

int read_identifier(struct TokenizerState *state, struct Token *token) {
    if (state == NULL || token == NULL || state->input == NULL) return 0;
    
    if (!isalpha(state->input[state->position]) && 
        state->input[state->position] != '_') return 0;
    
    token->type = TOKEN_IDENTIFIER;
    token->line = state->line;
    token->column = state->column;
    
    uint32_t len = 0;
    while (state->input[state->position] != '\0' && 
           (isalnum(state->input[state->position]) || 
            state->input[state->position] == '_') && 
           len < MAX_TOKEN_LEN - 1) {
        token->value[len++] = state->input[state->position++];
        state->column++;
    }
    
    token->value[len] = '\0';
    return 1;
}

int read_operator(struct TokenizerState *state, struct Token *token) {
    if (state == NULL || token == NULL || state->input == NULL) return 0;
    
    if (!is_operator_char(state->input[state->position])) return 0;
    
    token->type = TOKEN_OPERATOR;
    token->line = state->line;
    token->column = state->column;
    
    uint32_t len = 0;
    while (state->input[state->position] != '\0' && 
           is_operator_char(state->input[state->position]) && 
           len < MAX_TOKEN_LEN - 1) {
        token->value[len++] = state->input