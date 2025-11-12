//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char value[32];
    int line;
    int column;
};

struct ParserState {
    const char *input;
    int position;
    int line;
    int column;
    struct Token current_token;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void init_parser(struct ParserState *state, const char *input) {
    if (state == NULL || input == NULL) return;
    state->input = input;
    state->position = 0;
    state->line = 1;
    state->column = 1;
    state->current_token.type = TOKEN_INVALID;
    memset(state->current_token.value, 0, sizeof(state->current_token.value));
}

void skip_whitespace(struct ParserState *state) {
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

struct Token get_next_token(struct ParserState *state) {
    struct Token token;
    memset(&token, 0, sizeof(token));
    token.type = TOKEN_INVALID;
    
    if (state == NULL || state->input == NULL) {
        return token;
    }
    
    skip_whitespace(state);
    
    if (state->input[state->position] == '\0') {
        token.type = TOKEN_EOF;
        token.line = state->line;
        token.column = state->column;
        return token;
    }
    
    char current = state->input[state->position];
    int start_col = state->column;
    
    if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        token.line = state->line;
        token.column = state->column;
        int i = 0;
        
        while (state->input[state->position] != '\0' && 
               isdigit(state->input[state->position]) && 
               i < 31) {
            token.value[i++] = state->input[state->position++];
            state->column++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    if (isalpha(current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        token.line = state->line;
        token.column = state->column;
        int i = 0;
        
        while (state->input[state->position] != '\0' && 
               (isalnum(state->input[state->position]) || 
                state->input[state->position] == '_') && 
               i < 31) {
            token.value[i++] = state->input[state->position++];
            state->column++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        token.line = state->line;
        token.column = state->column;
        token.value[0] = state->input[state->position++];
        token.value[1] = '\0';
        state->column++;
        return token;
    }
    
    if (current == '"') {
        token.type = TOKEN_STRING;
        token.line = state->line;
        token.column = state->column;
        int i = 0;
        state->position++;
        state->column++;
        
        while (state->input[state->position] != '\0' && 
               state->input[state->position] != '"' && 
               i < 31) {
            token.value[i++] = state->input[state->position++];
            state->column++;
        }
        
        if (state->input[state->position] == '"') {
            state->position++;
            state->column++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    token.type = TOKEN_INVALID;
    token.value[0] = state->input[state->position++];
    token.value[1] = '\0';
    token.line = state->line;
    token.column = state->column;
    state->column++;
    return token;
}

void process_token(struct Token token) {
    switch (token.type) {
        case TOKEN_NUMBER:
            printf("NUMBER: %s (line %d, column %d)\n", token.value, token.line, token.column);
            break;
        case TOKEN_IDENTIFIER:
            printf("IDENTIFIER: %s (line %d, column %d)\n", token.value, token.line, token.column);
            break;
        case TOKEN_OPERATOR:
            printf("OPERATOR: %s (line %d, column %d)\n", token.value, token.line, token.column);
            break;