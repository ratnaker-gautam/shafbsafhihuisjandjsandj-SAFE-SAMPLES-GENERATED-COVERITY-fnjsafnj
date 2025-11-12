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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(struct ParserState *state) {
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

struct Token get_next_token(struct ParserState *state) {
    struct Token token = {TOKEN_INVALID, "", state->line, state->column};
    
    skip_whitespace(state);
    
    if (state->input[state->position] == '\0') {
        token.type = TOKEN_EOF;
        strcpy(token.value, "");
        return token;
    }
    
    char current = state->input[state->position];
    
    if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (state->input[state->position] != '\0' && isdigit(state->input[state->position]) && i < 31) {
            token.value[i++] = state->input[state->position++];
            state->column++;
        }
        token.value[i] = '\0';
    } else if (isalpha(current)) {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (state->input[state->position] != '\0' && (isalnum(state->input[state->position]) || state->input[state->position] == '_') && i < 31) {
            token.value[i++] = state->input[state->position++];
            state->column++;
        }
        token.value[i] = '\0';
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (state->input[state->position] != '\0' && is_operator_char(state->input[state->position]) && i < 31) {
            token.value[i++] = state->input[state->position++];
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

void init_parser(struct ParserState *state, const char *input) {
    state->input = input;
    state->position = 0;
    state->line = 1;
    state->column = 1;
    state->current_token = get_next_token(state);
}

void consume_token(struct ParserState *state, enum TokenType expected_type) {
    if (state->current_token.type == expected_type) {
        state->current_token = get_next_token(state);
    }
}

int parse_expression(struct ParserState *state);
int parse_term(struct ParserState *state);
int parse_factor(struct ParserState *state);

int parse_factor(struct ParserState *state) {
    if (state->current_token.type == TOKEN_NUMBER) {
        int value = 0;
        for (int i = 0; state->current_token.value[i] != '\0'; i++) {
            if (value > (2147483647 - (state->current_token.value[i] - '0')) / 10) {
                return 0;
            }
            value = value * 10 + (state->current_token.value[i] - '0');
        }
        consume_token(state, TOKEN_NUMBER);
        return value;
    } else if (state->current_token.type == TOKEN_IDENTIFIER) {
        printf("Variable '%s' at line %d, column %d\n", state->current_token.value, state->current_token.line, state->current_token.column);
        consume_token(state, TOKEN_IDENTIFIER);
        return 1;
    } else {
        printf("Unexpected token '%s' at line %d, column %d\n", state->current_token.value, state->current_token.line, state->current_token.column);
        return 0;
    }
}

int parse_term(struct ParserState *state) {
    int result = parse_factor(state);
    
    while (state->current_token.type == TOKEN_OPERATOR && 
           (strcmp(state->current_token.value, "*") == 0 || strcmp(state->current_token.value, "/") == 0)) {
        char op = state->current_token.value[0];
        consume_token(state, TOKEN_OPERATOR);
        int right = parse_factor(state);
        
        if (op == '*') {
            if (result != 0 && right > 2147483647 / result) {
                return 0;
            }
            result *= right;
        } else if