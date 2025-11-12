//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

typedef struct {
    const char* input;
    size_t pos;
    size_t len;
    Token tokens[MAX_TOKENS];
    size_t token_count;
} ParserState;

void init_parser(ParserState* state, const char* input) {
    if (state == NULL || input == NULL) return;
    state->input = input;
    state->pos = 0;
    state->len = strlen(input);
    state->token_count = 0;
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

int add_token(ParserState* state, TokenType type, const char* value) {
    if (state == NULL || value == NULL || state->token_count >= MAX_TOKENS) return 0;
    
    size_t len = strlen(value);
    if (len >= MAX_TOKEN_LEN) return 0;
    
    Token* token = &state->tokens[state->token_count];
    token->type = type;
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
    state->token_count++;
    return 1;
}

int parse_number(ParserState* state) {
    if (state == NULL || state->pos >= state->len) return 0;
    
    char buffer[MAX_TOKEN_LEN] = {0};
    size_t buf_pos = 0;
    
    while (state->pos < state->len && isdigit(state->input[state->pos])) {
        if (buf_pos >= MAX_TOKEN_LEN - 1) return 0;
        buffer[buf_pos++] = state->input[state->pos++];
    }
    
    if (buf_pos > 0) {
        buffer[buf_pos] = '\0';
        return add_token(state, TOKEN_NUMBER, buffer);
    }
    
    return 0;
}

int parse_identifier(ParserState* state) {
    if (state == NULL || state->pos >= state->len) return 0;
    
    char buffer[MAX_TOKEN_LEN] = {0};
    size_t buf_pos = 0;
    
    if (isalpha(state->input[state->pos]) || state->input[state->pos] == '_') {
        while (state->pos < state->len && 
               (isalnum(state->input[state->pos]) || state->input[state->pos] == '_')) {
            if (buf_pos >= MAX_TOKEN_LEN - 1) return 0;
            buffer[buf_pos++] = state->input[state->pos++];
        }
        
        if (buf_pos > 0) {
            buffer[buf_pos] = '\0';
            return add_token(state, TOKEN_IDENTIFIER, buffer);
        }
    }
    
    return 0;
}

int parse_operator(ParserState* state) {
    if (state == NULL || state->pos >= state->len) return 0;
    
    char buffer[2] = {0};
    
    if (is_operator(state->input[state->pos])) {
        buffer[0] = state->input[state->pos++];
        return add_token(state, TOKEN_OPERATOR, buffer);
    }
    
    return 0;
}

int parse_string(ParserState* state) {
    if (state == NULL || state->pos >= state->len) return 0;
    
    if (state->input[state->pos] == '"') {
        char buffer[MAX_TOKEN_LEN] = {0};
        size_t buf_pos = 0;
        
        state->pos++;
        
        while (state->pos < state->len && state->input[state->pos] != '"') {
            if (buf_pos >= MAX_TOKEN_LEN - 1) return 0;
            buffer[buf_pos++] = state->input[state->pos++];
        }
        
        if (state->pos < state->len && state->input[state->pos] == '"') {
            state->pos++;
            buffer[buf_pos] = '\0';
            return add_token(state, TOKEN_STRING, buffer);
        }
    }
    
    return 0;
}

int tokenize(ParserState* state) {
    if (state == NULL) return 0;
    
    while (state->pos < state->len && state->token_count < MAX_TOKENS) {
        if (is_whitespace(state->input[state->pos])) {
            state->pos++;
            continue;
        }
        
        if (parse_number(state)) continue;
        if (parse_identifier(state)) continue;
        if (parse_operator(state)) continue;
        if (parse_string(state)) continue;
        
        char unknown[2] = {state->input[state->pos++], '\0'};
        add_token(state, TOKEN_UNKNOWN, unknown