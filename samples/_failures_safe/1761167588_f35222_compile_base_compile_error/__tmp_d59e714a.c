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
    token.line = 0;
    token.column = 0;
    token.type = TOKEN_INVALID;
    token.value[0] = '\0';
    
    if (state == NULL || state->input == NULL) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    skip_whitespace(state);
    
    token.line = state->line;
    token.column = state->column;
    
    char current = state->input[state->position];
    
    if (current == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (isdigit(state->input[state->position]) && i < 31) {
            token.value[i++] = state->input[state->position++];
            state->column++;
        }
        token.value[i] = '\0';
    } else if (isalpha(current)) {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while ((isalnum(state->input[state->position]) || state->input[state->position] == '_') && i < 31) {
            token.value[i++] = state->input[state->position++];
            state->column++;
        }
        token.value[i] = '\0';
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (is_operator_char(state->input[state->position]) && i < 31) {
            token.value[i++] = state->input[state->position++];
            state->column++;
        }
        token.value[i] = '\0';
    } else {
        token.type = TOKEN_INVALID;
        if (state->input[state->position] != '\0') {
            token.value[0] = state->input[state->position++];
            token.value[1] = '\0';
            state->column++;
        }
    }
    
    return token;
}

void advance_token(struct ParserState *state) {
    if (state != NULL) {
        state->current_token = get_next_token(state);
    }
}

int expect_token(struct ParserState *state, enum TokenType expected_type) {
    if (state == NULL) return 0;
    return state->current_token.type == expected_type;
}

void parse_expression(struct ParserState *state);

void parse_primary(struct ParserState *state) {
    if (state == NULL) return;
    
    if (expect_token(state, TOKEN_NUMBER)) {
        printf("Number: %s\n", state->current_token.value);
        advance_token(state);
    } else if (expect_token(state, TOKEN_IDENTIFIER)) {
        printf("Identifier: %s\n", state->current_token.value);
        advance_token(state);
    } else {
        printf("Error: Expected number or identifier at line %d, column %d\n", 
               state->current_token.line, state->current_token.column);
    }
}

void parse_term(struct ParserState *state) {
    if (state == NULL) return;
    
    parse_primary(state);
    
    if (expect_token(state, TOKEN_OPERATOR) && 
        (strcmp(state->current_token.value, "*") == 0 || 
         strcmp(state->current_token.value, "/") == 0)) {
        printf("Operator: %s\n", state->current_token.value);
        advance_token(state);
        parse_term(state);
    }
}

void parse_expression(struct ParserState *state) {
    if (state == NULL) return;
    
    parse_term(state);
    
    if (expect_token(state, TOKEN_OPERATOR) && 
        (strcmp(state->current_token.value, "+") == 0 || 
         strcmp(state->current_token.value, "-") == 0)) {
        printf("Operator: %s\n", state->current_token.value);
        advance_token(state);
        parse_expression(state);
    }
}

void parse_statement(struct ParserState *state) {
    if (state == NULL) return;
    
    if (expect_token(state, TOKEN_IDENTIFIER