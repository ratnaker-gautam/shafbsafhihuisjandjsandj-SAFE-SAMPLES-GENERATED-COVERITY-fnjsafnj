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
};

struct ParserState {
    const char *input;
    int position;
    int length;
};

void init_parser(struct ParserState *state, const char *input) {
    state->input = input;
    state->position = 0;
    state->length = strlen(input);
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

int is_valid_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

void skip_whitespace(struct ParserState *state) {
    while (state->position < state->length && isspace(state->input[state->position])) {
        state->position++;
    }
}

struct Token get_next_token(struct ParserState *state) {
    struct Token token;
    memset(&token, 0, sizeof(token));
    
    skip_whitespace(state);
    
    if (state->position >= state->length) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char current = state->input[state->position];
    
    if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (state->position < state->length && i < 31 && isdigit(state->input[state->position])) {
            token.value[i++] = state->input[state->position++];
        }
        token.value[i] = '\0';
    } else if (isalpha(current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (state->position < state->length && i < 31 && is_valid_identifier_char(state->input[state->position])) {
            token.value[i++] = state->input[state->position++];
        }
        token.value[i] = '\0';
    } else if (is_operator(current)) {
        token.type = TOKEN_OPERATOR;
        token.value[0] = current;
        token.value[1] = '\0';
        state->position++;
    } else {
        token.type = TOKEN_INVALID;
        token.value[0] = current;
        token.value[1] = '\0';
        state->position++;
    }
    
    return token;
}

struct Token peek_token(struct ParserState *state) {
    struct ParserState saved_state = *state;
    struct Token token = get_next_token(state);
    *state = saved_state;
    return token;
}

void parse_expression(struct ParserState *state);

void parse_primary(struct ParserState *state) {
    struct Token token = get_next_token(state);
    
    if (token.type == TOKEN_NUMBER) {
        printf("Number: %s\n", token.value);
    } else if (token.type == TOKEN_IDENTIFIER) {
        printf("Identifier: %s\n", token.value);
    } else if (token.type == TOKEN_OPERATOR && token.value[0] == '(') {
        parse_expression(state);
        token = get_next_token(state);
        if (token.type != TOKEN_OPERATOR || token.value[0] != ')') {
            printf("Error: Expected ')'\n");
        }
    } else {
        printf("Error: Unexpected token: %s\n", token.value);
    }
}

void parse_term(struct ParserState *state) {
    parse_primary(state);
    
    struct Token token = peek_token(state);
    while (token.type == TOKEN_OPERATOR && (token.value[0] == '*' || token.value[0] == '/')) {
        token = get_next_token(state);
        printf("Operator: %s\n", token.value);
        parse_primary(state);
        token = peek_token(state);
    }
}

void parse_expression(struct ParserState *state) {
    parse_term(state);
    
    struct Token token = peek_token(state);
    while (token.type == TOKEN_OPERATOR && (token.value[0] == '+' || token.value[0] == '-')) {
        token = get_next_token(state);
        printf("Operator: %s\n", token.value);
        parse_term(state);
        token = peek_token(state);
    }
}

int main() {
    char input[256];
    
    printf("Enter an arithmetic expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    struct ParserState state;
    init_parser(&state, input);
    
    printf("Parsing expression: %s\n", input);
    printf("Tokens:\n");
    
    parse_expression(&state);
    
    struct Token token = get_next_token(&state);
    if (token.type != TOKEN_EOF) {
        printf("Error: Unexpected token at end: %s\n", token.value);
    }
    
    return 0;
}