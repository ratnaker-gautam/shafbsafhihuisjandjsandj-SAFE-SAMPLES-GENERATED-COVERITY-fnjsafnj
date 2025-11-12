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
    size_t input_len;
    size_t position;
    uint32_t line;
    uint32_t column;
    struct Token tokens[MAX_TOKENS];
    size_t token_count;
};

struct ParserState {
    struct Token tokens[MAX_TOKENS];
    size_t token_count;
    size_t current_token;
};

void tokenizer_init(struct TokenizerState *state, const char *input) {
    if (state == NULL || input == NULL) return;
    
    size_t len = strlen(input);
    if (len >= MAX_INPUT_LEN) len = MAX_INPUT_LEN - 1;
    
    strncpy(state->input, input, len);
    state->input[len] = '\0';
    state->input_len = len;
    state->position = 0;
    state->line = 1;
    state->column = 1;
    state->token_count = 0;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int is_delimiter(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || 
           c == '[' || c == ']' || c == ';' || c == ',' || c == ':';
}

int is_keyword(const char *str) {
    const char *keywords[] = {"if", "else", "while", "for", "return", "int", "float"};
    size_t num_keywords = sizeof(keywords) / sizeof(keywords[0]);
    
    for (size_t i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void add_token(struct TokenizerState *state, enum TokenType type, const char *value) {
    if (state == NULL || value == NULL || state->token_count >= MAX_TOKENS) return;
    
    struct Token *token = &state->tokens[state->token_count];
    token->type = type;
    token->line = state->line;
    token->column = state->column;
    
    size_t len = strlen(value);
    if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
    strncpy(token->value, value, len);
    token->value[len] = '\0';
    
    state->token_count++;
}

void skip_whitespace(struct TokenizerState *state) {
    while (state->position < state->input_len && isspace(state->input[state->position])) {
        if (state->input[state->position] == '\n') {
            state->line++;
            state->column = 1;
        } else {
            state->column++;
        }
        state->position++;
    }
}

void tokenize_number(struct TokenizerState *state) {
    size_t start = state->position;
    while (state->position < state->input_len && isdigit(state->input[state->position])) {
        state->position++;
        state->column++;
    }
    
    if (state->position - start > 0) {
        char num_str[MAX_TOKEN_LEN];
        size_t len = state->position - start;
        if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
        strncpy(num_str, &state->input[start], len);
        num_str[len] = '\0';
        add_token(state, TOKEN_NUMBER, num_str);
    }
}

void tokenize_identifier(struct TokenizerState *state) {
    size_t start = state->position;
    while (state->position < state->input_len && 
           (isalnum(state->input[state->position]) || state->input[state->position] == '_')) {
        state->position++;
        state->column++;
    }
    
    if (state->position - start > 0) {
        char ident_str[MAX_TOKEN_LEN];
        size_t len = state->position - start;
        if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
        strncpy(ident_str, &state->input[start], len);
        ident_str[len] = '\0';
        
        enum TokenType type = is_keyword(ident_str) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
        add_token(state, type, ident_str);
    }
}

void tokenize_operator(struct TokenizerState *state) {
    size_t start = state->position;
    state->position++;
    state->column++;
    
    if (state->position < state->input_len && is_operator_char(state->input[state->position])) {
        state->position++;
        state->column++;