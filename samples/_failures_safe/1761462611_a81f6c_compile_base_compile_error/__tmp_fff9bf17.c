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

void initialize_tokenizer(struct TokenizerState *state, char *input) {
    if (state == NULL || input == NULL) return;
    state->input = input;
    state->position = 0;
    state->line = 1;
    state->column = 1;
    state->token_count = 0;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_delimiter_char(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == ';' || c == ',';
}

int is_keyword(const char *str) {
    const char *keywords[] = {"if", "else", "while", "for", "return", NULL};
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0) return 1;
    }
    return 0;
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

int tokenize_number(struct TokenizerState *state, struct Token *token) {
    if (state == NULL || token == NULL || state->input == NULL) return 0;
    uint32_t start = state->position;
    uint32_t len = 0;
    
    while (state->input[state->position] != '\0' && isdigit(state->input[state->position])) {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        state->position++;
        state->column++;
        len++;
    }
    
    if (len > 0) {
        token->type = TOKEN_NUMBER;
        strncpy(token->value, &state->input[start], len);
        token->value[len] = '\0';
        token->line = state->line;
        token->column = state->column - len;
        return 1;
    }
    return 0;
}

int tokenize_identifier(struct TokenizerState *state, struct Token *token) {
    if (state == NULL || token == NULL || state->input == NULL) return 0;
    uint32_t start = state->position;
    uint32_t len = 0;
    
    if (state->input[state->position] != '\0' && isalpha(state->input[state->position])) {
        while (state->input[state->position] != '\0' && 
               (isalnum(state->input[state->position]) || state->input[state->position] == '_')) {
            if (len >= MAX_TOKEN_LEN - 1) return 0;
            state->position++;
            state->column++;
            len++;
        }
        
        strncpy(token->value, &state->input[start], len);
        token->value[len] = '\0';
        token->line = state->line;
        token->column = state->column - len;
        
        if (is_keyword(token->value)) {
            token->type = TOKEN_KEYWORD;
        } else {
            token->type = TOKEN_IDENTIFIER;
        }
        return 1;
    }
    return 0;
}

int tokenize_operator(struct TokenizerState *state, struct Token *token) {
    if (state == NULL || token == NULL || state->input == NULL) return 0;
    if (is_operator_char(state->input[state->position])) {
        token->type = TOKEN_OPERATOR;
        token->value[0] = state->input[state->position];
        token->value[1] = '\0';
        token->line = state->line;
        token->column = state->column;
        state->position++;
        state->column++;
        return 1;
    }
    return 0;
}

int tokenize_delimiter(struct TokenizerState *state, struct Token *token) {
    if (state == NULL || token == NULL || state->input == NULL) return 0;
    if (is_delimiter_char(state->input[state->position])) {
        token->type = TOKEN_DELIMITER;
        token->value[0] = state->input