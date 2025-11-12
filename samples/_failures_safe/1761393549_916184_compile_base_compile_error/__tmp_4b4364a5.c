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

int is_delimiter_char(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || 
           c == '[' || c == ']' || c == ';' || c == ',' || c == ':';
}

int is_keyword(const char *str) {
    static const char *keywords[] = {
        "if", "else", "while", "for", "return", "int", "float", "char", "void"
    };
    static const size_t keyword_count = 9;
    
    for (size_t i = 0; i < keyword_count; i++) {
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

int tokenize_number(struct TokenizerState *state) {
    if (state->position >= state->input_len || !isdigit(state->input[state->position])) return 0;
    
    char buffer[MAX_TOKEN_LEN];
    size_t buf_pos = 0;
    
    while (state->position < state->input_len && 
           (isdigit(state->input[state->position]) || state->input[state->position] == '.')) {
        if (buf_pos >= MAX_TOKEN_LEN - 1) break;
        buffer[buf_pos++] = state->input[state->position++];
        state->column++;
    }
    buffer[buf_pos] = '\0';
    
    add_token(state, TOKEN_NUMBER, buffer);
    return 1;
}

int tokenize_identifier(struct TokenizerState *state) {
    if (state->position >= state->input_len || !isalpha(state->input[state->position])) return 0;
    
    char buffer[MAX_TOKEN_LEN];
    size_t buf_pos = 0;
    
    while (state->position < state->input_len && 
           (isalnum(state->input[state->position]) || state->input[state->position] == '_')) {
        if (buf_pos >= MAX_TOKEN_LEN - 1) break;
        buffer[buf_pos++] = state->input[state->position++];
        state->column++;
    }
    buffer[buf_pos] = '\0';
    
    enum TokenType type = is_keyword(buffer) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
    add_token(state, type, buffer);
    return 1;
}

int tokenize_operator(struct TokenizerState *state) {
    if (state->position >= state->input_len || !is_operator_char(state->input[state->position])) return 0;
    
    char buffer[3] = {0};
    size