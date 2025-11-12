//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
    int position;
};

struct ParserState {
    const char *input;
    int position;
    int length;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

struct Token get_next_token(struct ParserState *state) {
    struct Token token = {TOKEN_INVALID, "", state->position};
    
    if (state->position >= state->length) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    while (state->position < state->length && isspace(state->input[state->position])) {
        state->position++;
    }
    
    if (state->position >= state->length) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char current = state->input[state->position];
    
    if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (state->position < state->length && i < 31 && 
               isdigit(state->input[state->position])) {
            token.value[i++] = state->input[state->position++];
        }
        token.value[i] = '\0';
    } else if (isalpha(current)) {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (state->position < state->length && i < 31 && 
               (isalnum(state->input[state->position]) || state->input[state->position] == '_')) {
            token.value[i++] = state->input[state->position++];
        }
        token.value[i] = '\0';
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        token.value[0] = state->input[state->position++];
        token.value[1] = '\0';
    } else {
        token.type = TOKEN_INVALID;
        token.value[0] = state->input[state->position++];
        token.value[1] = '\0';
    }
    
    token.position = state->position;
    return token;
}

void parse_expression(struct ParserState *state);
void parse_term(struct ParserState *state);
void parse_factor(struct ParserState *state);

void parse_factor(struct ParserState *state) {
    struct Token token = get_next_token(state);
    
    if (token.type == TOKEN_NUMBER) {
        printf("Number: %s\n", token.value);
    } else if (token.type == TOKEN_IDENTIFIER) {
        printf("Identifier: %s\n", token.value);
    } else if (token.type == TOKEN_OPERATOR && token.value[0] == '(') {
        parse_expression(state);
        token = get_next_token(state);
        if (token.type != TOKEN_OPERATOR || token.value[0] != ')') {
            printf("Error: Expected closing parenthesis at position %d\n", token.position);
        }
    } else {
        printf("Error: Unexpected token '%s' at position %d\n", token.value, token.position);
    }
}

void parse_term(struct ParserState *state) {
    parse_factor(state);
    
    struct Token token = get_next_token(state);
    if (token.type == TOKEN_OPERATOR && (token.value[0] == '*' || token.value[0] == '/')) {
        printf("Operator: %s\n", token.value);
        parse_term(state);
    } else {
        state->position = token.position;
    }
}

void parse_expression(struct ParserState *state) {
    parse_term(state);
    
    struct Token token = get_next_token(state);
    if (token.type == TOKEN_OPERATOR && (token.value[0] == '+' || token.value[0] == '-')) {
        printf("Operator: %s\n", token.value);
        parse_expression(state);
    } else {
        state->position = token.position;
    }
}

int main(void) {
    char input[256];
    
    printf("Enter an arithmetic expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    if (len >= sizeof(input) - 1) {
        printf("Input too long\n");
        return 1;
    }
    
    struct ParserState state;
    state.input = input;
    state.position = 0;
    state.length = (int)len;
    
    printf("Parsing expression: %s\n", input);
    parse_expression(&state);
    
    struct Token final_token = get_next_token(&state);
    if (final_token.type != TOKEN_EOF) {
        printf("Error: Unexpected token '%s' at end of expression\n", final_token.value);
        return 1;
    }
    
    printf("Parsing completed successfully\n");
    return 0;
}