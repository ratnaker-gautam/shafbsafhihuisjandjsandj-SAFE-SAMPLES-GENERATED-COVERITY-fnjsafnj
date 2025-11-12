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
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
    TOKEN_KEYWORD,
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

struct ParserContext {
    struct Token tokens[MAX_TOKENS];
    uint32_t token_index;
    uint32_t token_total;
};

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int is_delimiter_char(char c) {
    return strchr("(),;{}[]", c) != NULL;
}

int is_keyword(const char *str) {
    const char *keywords[] = {"if", "else", "while", "for", "return", "int", "float", "char", NULL};
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void init_tokenizer(struct TokenizerState *state, char *input) {
    state->input = input;
    state->position = 0;
    state->line = 1;
    state->column = 1;
    state->token_count = 0;
}

void skip_whitespace(struct TokenizerState *state) {
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

int read_number(struct TokenizerState *state, struct Token *token) {
    uint32_t start = state->position;
    uint32_t len = 0;
    
    while (isdigit(state->input[state->position])) {
        state->position++;
        state->column++;
        len++;
        if (len >= MAX_TOKEN_LEN - 1) {
            return 0;
        }
    }
    
    if (len > 0) {
        strncpy(token->value, &state->input[start], len);
        token->value[len] = '\0';
        token->type = TOKEN_NUMBER;
        token->line = state->line;
        token->column = state->column - len;
        return 1;
    }
    return 0;
}

int read_identifier(struct TokenizerState *state, struct Token *token) {
    uint32_t start = state->position;
    uint32_t len = 0;
    
    while (isalnum(state->input[state->position]) || state->input[state->position] == '_') {
        state->position++;
        state->column++;
        len++;
        if (len >= MAX_TOKEN_LEN - 1) {
            return 0;
        }
    }
    
    if (len > 0) {
        strncpy(token->value, &state->input[start], len);
        token->value[len] = '\0';
        token->type = is_keyword(token->value) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
        token->line = state->line;
        token->column = state->column - len;
        return 1;
    }
    return 0;
}

int read_operator(struct TokenizerState *state, struct Token *token) {
    if (is_operator_char(state->input[state->position])) {
        token->value[0] = state->input[state->position];
        token->value[1] = '\0';
        token->type = TOKEN_OPERATOR;
        token->line = state->line;
        token->column = state->column;
        state->position++;
        state->column++;
        return 1;
    }
    return 0;
}

int read_delimiter(struct TokenizerState *state, struct Token *token) {
    if (is_delimiter_char(state->input[state->position])) {
        token->value[0] = state->input[state->position];
        token->value[1] = '\0';
        token->type = TOKEN_DELIMITER;
        token->line = state->line;
        token->column = state->column;
        state->position++;
        state->column++;
        return 1;
    }
    return 0;
}

int tokenize(struct TokenizerState *state, struct Token tokens[], uint32_t max_tokens) {
    while (state->input[state->position] != '\0' && state->token_count < max_tokens) {
        skip_whitespace(state);
        
        if (state->input[state->position] == '\0') {
            break;
        }
        
        struct Token token;
        int success = 0;
        
        if (isdigit(state->input[state->position])) {
            success = read_number(state