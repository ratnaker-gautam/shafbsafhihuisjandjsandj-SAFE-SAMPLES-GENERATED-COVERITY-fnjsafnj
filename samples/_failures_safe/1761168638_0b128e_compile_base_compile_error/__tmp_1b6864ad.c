//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_UNKNOWN
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
    int line;
    int column;
};

struct TokenizerState {
    const char *input;
    int position;
    int line;
    int column;
    int token_count;
};

struct ParserState {
    struct Token *tokens;
    int current_token;
    int token_count;
};

void init_tokenizer(struct TokenizerState *state, const char *input) {
    state->input = input;
    state->position = 0;
    state->line = 1;
    state->column = 1;
    state->token_count = 0;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(struct TokenizerState *state) {
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

int read_number(struct TokenizerState *state, struct Token *token) {
    int len = 0;
    int start_pos = state->position;
    int start_col = state->column;
    
    while (state->input[state->position] != '\0' && isdigit(state->input[state->position])) {
        if (len < MAX_TOKEN_LEN - 1) {
            token->value[len++] = state->input[state->position];
        }
        state->position++;
        state->column++;
    }
    
    if (len > 0) {
        token->value[len] = '\0';
        token->type = TOKEN_NUMBER;
        token->line = state->line;
        token->column = start_col;
        return 1;
    }
    
    state->position = start_pos;
    state->column = start_col;
    return 0;
}

int read_identifier(struct TokenizerState *state, struct Token *token) {
    int len = 0;
    int start_pos = state->position;
    int start_col = state->column;
    
    if (state->input[state->position] != '\0' && isalpha(state->input[state->position])) {
        while (state->input[state->position] != '\0' && 
               (isalnum(state->input[state->position]) || state->input[state->position] == '_')) {
            if (len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = state->input[state->position];
            }
            state->position++;
            state->column++;
        }
        
        if (len > 0) {
            token->value[len] = '\0';
            token->type = TOKEN_IDENTIFIER;
            token->line = state->line;
            token->column = start_col;
            return 1;
        }
    }
    
    state->position = start_pos;
    state->column = start_col;
    return 0;
}

int read_operator(struct TokenizerState *state, struct Token *token) {
    int len = 0;
    int start_pos = state->position;
    int start_col = state->column;
    
    if (state->input[state->position] != '\0' && is_operator_char(state->input[state->position])) {
        while (state->input[state->position] != '\0' && is_operator_char(state->input[state->position])) {
            if (len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = state->input[state->position];
            }
            state->position++;
            state->column++;
        }
        
        if (len > 0) {
            token->value[len] = '\0';
            token->type = TOKEN_OPERATOR;
            token->line = state->line;
            token->column = start_col;
            return 1;
        }
    }
    
    state->position = start_pos;
    state->column = start_col;
    return 0;
}

int read_string(struct TokenizerState *state, struct Token *token) {
    int len = 0;
    int start_pos = state->position;
    int start_col = state->column;
    
    if (state->input[state->position] == '"') {
        token->value[len++] = state->input[state->position];
        state->position++;
        state->column++;
        
        while (state->input[state->position] != '\0' && state->input[state->position] != '"') {
            if (len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = state->input[state->position];
            }
            state->position++;
            state->column++;
        }
        
        if (state->input[state->position] == '"') {
            if (len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = state->input[state->position];
            }
            state