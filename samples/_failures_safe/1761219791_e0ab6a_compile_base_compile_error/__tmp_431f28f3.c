//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <ctype.h>
#include <string.h>

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '(' || c == ')';
}

void skip_whitespace(struct ParserState *state) {
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
    skip_whitespace(state);
    
    if (state->input[state->position] == '\0') {
        struct Token token = {TOKEN_EOF, "", state->line, state->column};
        return token;
    }
    
    char current = state->input[state->position];
    int start_pos = state->position;
    int start_col = state->column;
    
    if (isdigit(current)) {
        struct Token token = {TOKEN_NUMBER, "", state->line, start_col};
        int i = 0;
        while (isdigit(state->input[state->position]) && i < 31) {
            token.value[i++] = state->input[state->position++];
            state->column++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    if (isalpha(current) || current == '_') {
        struct Token token = {TOKEN_IDENTIFIER, "", state->line, start_col};
        int i = 0;
        while ((isalnum(state->input[state->position]) || state->input[state->position] == '_') && i < 31) {
            token.value[i++] = state->input[state->position++];
            state->column++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    if (is_operator_char(current)) {
        struct Token token = {TOKEN_OPERATOR, "", state->line, start_col};
        token.value[0] = state->input[state->position++];
        token.value[1] = '\0';
        state->column++;
        return token;
    }
    
    struct Token token = {TOKEN_INVALID, "", state->line, start_col};
    token.value[0] = state->input[state->position++];
    token.value[1] = '\0';
    state->column++;
    return token;
}

void advance_token(struct ParserState *state) {
    state->current_token = get_next_token(state);
}

int expect_token(struct ParserState *state, enum TokenType expected_type) {
    return state->current_token.type == expected_type;
}

void parse_expression(struct ParserState *state);

void parse_factor(struct ParserState *state) {
    if (expect_token(state, TOKEN_NUMBER)) {
        printf("Number: %s\n", state->current_token.value);
        advance_token(state);
    } else if (expect_token(state, TOKEN_IDENTIFIER)) {
        printf("Identifier: %s\n", state->current_token.value);
        advance_token(state);
    } else if (expect_token(state, TOKEN_OPERATOR) && state->current_token.value[0] == '(') {
        advance_token(state);
        parse_expression(state);
        if (!expect_token(state, TOKEN_OPERATOR) || state->current_token.value[0] != ')') {
            printf("Error: Expected ')' at line %d, column %d\n", state->current_token.line, state->current_token.column);
            return;
        }
        advance_token(state);
    } else {
        printf("Error: Unexpected token '%s' at line %d, column %d\n", state->current_token.value, state->current_token.line, state->current_token.column);
        advance_token(state);
    }
}

void parse_term(struct ParserState *state) {
    parse_factor(state);
    
    while (expect_token(state, TOKEN_OPERATOR) && 
          (state->current_token.value[0] == '*' || state->current_token.value[0] == '/')) {
        printf("Operator: %s\n", state->current_token.value);
        advance_token(state);
        parse_factor(state);
    }
}

void parse_expression(struct ParserState *state) {
    parse_term(state);
    
    while (expect_token(state, TOKEN_OPERATOR) && 
          (state->current_token.value[0] == '+' || state->current_token.value[0] == '-')) {
        printf("Operator: %s\n", state->current_token.value);
        advance_token(state);
        parse_term(state);
    }
}

void parse_assignment(struct ParserState *state) {
    if (!expect_token(state, TOKEN_IDENTIFIER)) {
        printf("Error: Expected identifier at line %d, column