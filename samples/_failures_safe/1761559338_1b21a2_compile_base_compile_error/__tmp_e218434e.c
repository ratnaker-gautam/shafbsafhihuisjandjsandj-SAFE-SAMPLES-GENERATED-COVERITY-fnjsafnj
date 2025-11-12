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

void init_parser(struct ParserState *state, const char *input) {
    state->input = input;
    state->position = 0;
    state->line = 1;
    state->column = 1;
    state->current_token.type = TOKEN_INVALID;
    state->current_token.value[0] = '\0';
    state->current_token.line = 1;
    state->current_token.column = 1;
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
        struct Token token;
        token.type = TOKEN_EOF;
        token.value[0] = '\0';
        token.line = state->line;
        token.column = state->column;
        return token;
    }
    
    char current = state->input[state->position];
    int start_col = state->column;
    
    if (isdigit(current)) {
        struct Token token;
        token.type = TOKEN_NUMBER;
        token.line = state->line;
        token.column = state->column;
        
        int i = 0;
        while (state->input[state->position] != '\0' && 
               isdigit(state->input[state->position]) && i < 31) {
            token.value[i++] = state->input[state->position++];
            state->column++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    if (isalpha(current)) {
        struct Token token;
        token.type = TOKEN_IDENTIFIER;
        token.line = state->line;
        token.column = state->column;
        
        int i = 0;
        while (state->input[state->position] != '\0' && 
               (isalnum(state->input[state->position]) || 
                state->input[state->position] == '_') && i < 31) {
            token.value[i++] = state->input[state->position++];
            state->column++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    if (is_operator_char(current)) {
        struct Token token;
        token.type = TOKEN_OPERATOR;
        token.line = state->line;
        token.column = state->column;
        
        int i = 0;
        while (state->input[state->position] != '\0' && 
               is_operator_char(state->input[state->position]) && i < 31) {
            token.value[i++] = state->input[state->position++];
            state->column++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    struct Token token;
    token.type = TOKEN_INVALID;
    token.value[0] = state->input[state->position++];
    token.value[1] = '\0';
    token.line = state->line;
    token.column = state->column;
    state->column++;
    return token;
}

void consume_token(struct ParserState *state) {
    state->current_token = get_next_token(state);
}

int parse_expression(struct ParserState *state);

int parse_factor(struct ParserState *state) {
    if (state->current_token.type == TOKEN_NUMBER) {
        int value = 0;
        for (int i = 0; state->current_token.value[i] != '\0'; i++) {
            if (value > 214748364) {
                return 0;
            }
            value = value * 10 + (state->current_token.value[i] - '0');
        }
        consume_token(state);
        return value;
    } else if (state->current_token.type == TOKEN_IDENTIFIER) {
        printf("Variable '%s' at line %d, column %d\n", 
               state->current_token.value, state->current_token.line, state->current_token.column);
        consume_token(state);
        return 0;
    } else if (state->current_token.value[0] == '(') {
        consume_token(state);
        int result = parse_expression(state);
        if (state->current_token.value[0] != ')') {
            return 0;
        }
        consume_token(state);
        return result;
    }
    return 0;
}

int parse_term(struct ParserState *state) {
    int result =