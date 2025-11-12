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
    char input[MAX_INPUT_LEN];
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

void initialize_tokenizer(struct TokenizerState *state, const char *input) {
    if (state == NULL || input == NULL) return;
    size_t len = strlen(input);
    if (len >= MAX_INPUT_LEN) len = MAX_INPUT_LEN - 1;
    strncpy(state->input, input, len);
    state->input[len] = '\0';
    state->position = 0;
    state->line = 1;
    state->column = 1;
    state->token_count = 0;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_delimiter(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == ';' || c == ',';
}

int is_keyword(const char *str) {
    const char *keywords[] = {"if", "else", "while", "for", "return", "int", "float"};
    size_t num_keywords = sizeof(keywords) / sizeof(keywords[0]);
    for (size_t i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0) return 1;
    }
    return 0;
}

void skip_whitespace(struct TokenizerState *state) {
    if (state == NULL) return;
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
    if (state == NULL || token == NULL) return 0;
    if (state->token_count >= MAX_TOKENS) return 0;
    
    skip_whitespace(state);
    
    if (state->input[state->position] == '\0') return 0;
    
    token->line = state->line;
    token->column = state->column;
    
    char current = state->input[state->position];
    
    if (isdigit(current)) {
        token->type = TOKEN_NUMBER;
        int i = 0;
        while (state->input[state->position] != '\0' && i < MAX_TOKEN_LEN - 1 && 
               (isdigit(state->input[state->position]) || state->input[state->position] == '.')) {
            token->value[i++] = state->input[state->position++];
            state->column++;
        }
        token->value[i] = '\0';
    } else if (isalpha(current) || current == '_') {
        token->type = TOKEN_IDENTIFIER;
        int i = 0;
        while (state->input[state->position] != '\0' && i < MAX_TOKEN_LEN - 1 && 
               (isalnum(state->input[state->position]) || state->input[state->position] == '_')) {
            token->value[i++] = state->input[state->position++];
            state->column++;
        }
        token->value[i] = '\0';
        if (is_keyword(token->value)) {
            token->type = TOKEN_KEYWORD;
        }
    } else if (is_operator_char(current)) {
        token->type = TOKEN_OPERATOR;
        int i = 0;
        while (state->input[state->position] != '\0' && i < MAX_TOKEN_LEN - 1 && 
               is_operator_char(state->input[state->position])) {
            token->value[i++] = state->input[state->position++];
            state->column++;
        }
        token->value[i] = '\0';
    } else if (is_delimiter(current)) {
        token->type = TOKEN_DELIMITER;
        token->value[0] = state->input[state->position++];
        token->value[1] = '\0';
        state->column++;
    } else {
        token->type = TOKEN_UNKNOWN;
        token->value[0] = state->input[state->position++];
        token->value[1] = '\0';
        state->column++;
    }
    
    state->token_count++;
    return 1;
}

void tokenize_input(struct TokenizerState *state, struct Parser *parser) {