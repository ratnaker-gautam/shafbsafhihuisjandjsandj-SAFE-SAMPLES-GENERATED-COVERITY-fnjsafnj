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
    int error_count;
};

void init_parser(struct ParserState *state, const char *input) {
    if (state == NULL || input == NULL) return;
    state->input = input;
    state->position = 0;
    state->line = 1;
    state->column = 1;
    state->error_count = 0;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
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
    struct Token token = {TOKEN_INVALID, "", state->line, state->column};
    if (state == NULL || state->input == NULL) return token;
    
    skip_whitespace(state);
    
    if (state->input[state->position] == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char current = state->input[state->position];
    
    if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (i < 31 && isdigit(state->input[state->position])) {
            token.value[i++] = state->input[state->position++];
            state->column++;
        }
        token.value[i] = '\0';
    } else if (isalpha(current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (i < 31 && (isalnum(state->input[state->position]) || state->input[state->position] == '_')) {
            token.value[i++] = state->input[state->position++];
            state->column++;
        }
        token.value[i] = '\0';
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (i < 31 && is_operator_char(state->input[state->position])) {
            token.value[i++] = state->input[state->position++];
            state->column++;
        }
        token.value[i] = '\0';
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        token.value[0] = '"';
        state->position++;
        state->column++;
        int i = 1;
        while (i < 31 && state->input[state->position] != '\0' && state->input[state->position] != '"') {
            token.value[i++] = state->input[state->position++];
            state->column++;
        }
        if (state->input[state->position] == '"') {
            if (i < 31) {
                token.value[i++] = '"';
            }
            state->position++;
            state->column++;
        }
        token.value[i] = '\0';
    } else {
        token.type = TOKEN_INVALID;
        token.value[0] = state->input[state->position++];
        token.value[1] = '\0';
        state->column++;
    }
    
    return token;
}

const char* token_type_to_string(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_INVALID: return "INVALID";
        default: return "UNKNOWN";
    }
}

void process_tokens(const char *input) {
    if (input == NULL) return;
    
    struct ParserState state;
    init_parser(&state, input);
    
    printf("Input: \"%s\"\n", input);
    printf("Tokens:\n");
    
    while (1) {
        struct Token token = get_next_token(&state);
        
        switch (token.type) {
            case TOKEN_NUMBER:
                printf("  NUMBER: %s (line %d, col %d)\n", token.value, token.line, token.column);
                break;
            case TOKEN_IDENTIFIER:
                printf("  IDENTIFIER: %s (line %d, col %d)\n", token.value, token.line, token.column);
                break;
            case TOKEN_OPERATOR:
                printf("