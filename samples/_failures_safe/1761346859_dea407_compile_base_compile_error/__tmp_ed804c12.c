//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOK_NUMBER, TOK_IDENT, TOK_OPERATOR, TOK_EOF, TOK_ERROR };

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
    struct Token current;
};

void init_parser(struct ParserState *state, const char *input) {
    state->input = input;
    state->position = 0;
    state->line = 1;
    state->column = 1;
    state->current.type = TOK_ERROR;
    memset(state->current.value, 0, sizeof(state->current.value));
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '(' || c == ')';
}

void next_token(struct ParserState *state) {
    if (state->input == NULL) {
        state->current.type = TOK_ERROR;
        return;
    }

    while (state->input[state->position] != '\0' && isspace(state->input[state->position])) {
        if (state->input[state->position] == '\n') {
            state->line++;
            state->column = 1;
        } else {
            state->column++;
        }
        state->position++;
    }

    if (state->input[state->position] == '\0') {
        state->current.type = TOK_EOF;
        memset(state->current.value, 0, sizeof(state->current.value));
        return;
    }

    state->current.line = state->line;
    state->current.column = state->column;

    char c = state->input[state->position];
    
    if (isdigit(c)) {
        state->current.type = TOK_NUMBER;
        int i = 0;
        while (state->input[state->position] != '\0' && isdigit(state->input[state->position])) {
            if (i < 31) {
                state->current.value[i++] = state->input[state->position];
            }
            state->position++;
            state->column++;
        }
        state->current.value[i] = '\0';
    } else if (isalpha(c)) {
        state->current.type = TOK_IDENT;
        int i = 0;
        while (state->input[state->position] != '\0' && (isalnum(state->input[state->position]) || state->input[state->position] == '_')) {
            if (i < 31) {
                state->current.value[i++] = state->input[state->position];
            }
            state->position++;
            state->column++;
        }
        state->current.value[i] = '\0';
    } else if (is_operator_char(c)) {
        state->current.type = TOK_OPERATOR;
        state->current.value[0] = c;
        state->current.value[1] = '\0';
        state->position++;
        state->column++;
    } else {
        state->current.type = TOK_ERROR;
        state->current.value[0] = c;
        state->current.value[1] = '\0';
        state->position++;
        state->column++;
    }
}

int expect_token(struct ParserState *state, enum TokenType type, const char *value) {
    if (state->current.type == type && (value == NULL || strcmp(state->current.value, value) == 0)) {
        next_token(state);
        return 1;
    }
    return 0;
}

void parse_expression(struct ParserState *state);

void parse_primary(struct ParserState *state) {
    if (state->current.type == TOK_NUMBER) {
        printf("Number: %s\n", state->current.value);
        next_token(state);
    } else if (state->current.type == TOK_IDENT) {
        printf("Identifier: %s\n", state->current.value);
        next_token(state);
    } else if (expect_token(state, TOK_OPERATOR, "(")) {
        parse_expression(state);
        if (!expect_token(state, TOK_OPERATOR, ")")) {
            printf("Error: Expected ')' at line %d, column %d\n", state->current.line, state->current.column);
        }
    } else {
        printf("Error: Unexpected token '%s' at line %d, column %d\n", state->current.value, state->current.line, state->current.column);
        next_token(state);
    }
}

void parse_term(struct ParserState *state) {
    parse_primary(state);
    while (state->current.type == TOK_OPERATOR && (strcmp(state->current.value, "*") == 0 || strcmp(state->current.value, "/") == 0)) {
        char op[2];
        strncpy(op, state->current.value, sizeof(op));
        op[1] = '\0';
        next_token(state);
        printf("Operator: %s\n", op);
        parse_primary(state);
    }
}

void parse_expression(struct ParserState *state) {
    parse_term(state);
    while (state->current.type == TOK_OPERATOR && (strcmp(state->current.value, "+") == 0 || strcmp(state->current.value, "-") == 0)) {
        char op[2];
        strnc