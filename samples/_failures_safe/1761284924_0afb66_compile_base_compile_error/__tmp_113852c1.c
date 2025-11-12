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
    int line;
    int column;
};

struct TokenizerState {
    char *input;
    int position;
    int line;
    int column;
    int token_count;
};

struct ParserContext {
    struct Token tokens[MAX_TOKENS];
    int token_index;
    int token_total;
};

int is_keyword(const char *str) {
    static const char *keywords[] = {"if", "else", "while", "for", "return", NULL};
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_delimiter(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == ';' || c == ',';
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
    if (state == NULL || token == NULL || state->input == NULL) return 0;
    int start = state->position;
    int len = 0;
    while (state->input[state->position] != '\0' && isdigit(state->input[state->position])) {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        state->position++;
        state->column++;
        len++;
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
    if (state == NULL || token == NULL || state->input == NULL) return 0;
    int start = state->position;
    int len = 0;
    while (state->input[state->position] != '\0' && (isalnum(state->input[state->position]) || state->input[state->position] == '_')) {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        state->position++;
        state->column++;
        len++;
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

int get_next_token(struct TokenizerState *state, struct Token *token) {
    if (state == NULL || token == NULL || state->input == NULL) return 0;
    skip_whitespace(state);
    if (state->input[state->position] == '\0') return 0;
    
    char current = state->input[state->position];
    
    if (isdigit(current)) {
        return read_number(state, token);
    } else if (isalpha(current) || current == '_') {
        return read_identifier(state, token);
    } else if (is_operator(current)) {
        token->value[0] = current;
        token->value[1] = '\0';
        token->type = TOKEN_OPERATOR;
        token->line = state->line;
        token->column = state->column;
        state->position++;
        state->column++;
        return 1;
    } else if (is_delimiter(current)) {
        token->value[0] = current;
        token->value[1] = '\0';
        token->type = TOKEN_DELIMITER;
        token->line = state->line;
        token->column = state->column;
        state->position++;
        state->column++;
        return 1;
    } else