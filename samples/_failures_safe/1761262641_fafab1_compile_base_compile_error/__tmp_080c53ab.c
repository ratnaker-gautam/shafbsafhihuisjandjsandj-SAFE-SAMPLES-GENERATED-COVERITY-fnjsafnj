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
    
    struct Token token;
    token.line = state->line;
    token.column = state->column;
    token.value[0] = '\0';
    
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
        token.value[0] = state->input[state->position++];
        token.value[1] = '\0';
        state->column++;
    } else {
        token.type = TOKEN_INVALID;
        token.value[0] = state->input[state->position++];
        token.value[1] = '\0';
        state->column++;
    }
    
    return token;
}

void consume_token(struct ParserState *state, enum TokenType expected_type) {
    if (state->current_token.type == expected_type) {
        state->current_token = get_next_token(state);
    }
}

int parse_expression(struct ParserState *state);

int parse_factor(struct ParserState *state) {
    if (state->current_token.type == TOKEN_NUMBER) {
        int value;
        if (sscanf(state->current_token.value, "%d", &value) == 1) {
            consume_token(state, TOKEN_NUMBER);
            return value;
        }
    } else if (state->current_token.type == TOKEN_IDENTIFIER) {
        printf("Variable '%s' at line %d, column %d\n", state->current_token.value, state->current_token.line, state->current_token.column);
        consume_token(state, TOKEN_IDENTIFIER);
        return 0;
    } else if (state->current_token.type == TOKEN_OPERATOR && state->current_token.value[0] == '(') {
        consume_token(state, TOKEN_OPERATOR);
        int result = parse_expression(state);
        if (state->current_token.type == TOKEN_OPERATOR && state->current_token.value[0] == ')') {
            consume_token(state, TOKEN_OPERATOR);
            return result;
        }
    }
    return 0;
}

int parse_term(struct ParserState *state) {
    int result = parse_factor(state);
    
    while (state->current_token.type == TOKEN_OPERATOR && 
           (state->current_token.value[0] == '*' || state->current_token.value[0] == '/')) {
        char op = state->current_token.value[0];
        consume_token(state, TOKEN_OPERATOR);
        int right = parse_factor(state);
        
        if (op == '*') {
            result *= right;
        } else if (op == '/' && right != 0) {
            result /= right;
        }
    }
    
    return result;
}

int parse_expression(struct ParserState *state) {
    int result = parse_term(state);
    
    while (state->current_token.type == TOKEN_OPERATOR && 
           (state->current_token.value[0] == '+' || state->current_token.value[0] == '-')) {
        char op = state->current_token.value[0];
        consume_token(state, TOKEN_OPERATOR);
        int right = parse_term(state);
        
        if (op == '+') {
            result += right;
        } else {
            result -= right;
        }
    }
    
    return result;
}

int main() {
    char input[256];