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

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '(' || c == ')';
}

void skip_whitespace(struct ParserState *state) {
    while (state->position < state->length && isspace(state->input[state->position])) {
        state->position++;
    }
}

struct Token get_next_token(struct ParserState *state) {
    struct Token token = {TOKEN_INVALID, ""};
    
    if (state->position >= state->length) {
        token.type = TOKEN_EOF;
        return token;
    }
    
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
    } else if (isalpha(current)) {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (state->position < state->length && i < 31 && (isalnum(state->input[state->position]) || state->input[state->position] == '_')) {
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
    
    return token;
}

void parse_expression(struct ParserState *state);

void parse_primary(struct ParserState *state) {
    struct Token token = get_next_token(state);
    
    if (token.type == TOKEN_NUMBER) {
        printf("NUMBER: %s\n", token.value);
    } else if (token.type == TOKEN_IDENTIFIER) {
        printf("IDENTIFIER: %s\n", token.value);
    } else if (token.type == TOKEN_OPERATOR && token.value[0] == '(') {
        printf("BEGIN GROUP\n");
        parse_expression(state);
        token = get_next_token(state);
        if (token.type != TOKEN_OPERATOR || token.value[0] != ')') {
            printf("ERROR: Expected closing parenthesis\n");
        } else {
            printf("END GROUP\n");
        }
    } else {
        printf("ERROR: Unexpected token: %s\n", token.value);
    }
}

void parse_term(struct ParserState *state) {
    parse_primary(state);
    
    struct Token token = get_next_token(state);
    if (token.type == TOKEN_OPERATOR && (token.value[0] == '*' || token.value[0] == '/')) {
        printf("OPERATOR: %s\n", token.value);
        parse_term(state);
    } else {
        state->position -= strlen(token.value);
    }
}

void parse_expression(struct ParserState *state) {
    parse_term(state);
    
    struct Token token = get_next_token(state);
    if (token.type == TOKEN_OPERATOR && (token.value[0] == '+' || token.value[0] == '-')) {
        printf("OPERATOR: %s\n", token.value);
        parse_expression(state);
    } else {
        state->position -= strlen(token.value);
    }
}

void parse_assignment(struct ParserState *state) {
    struct Token token = get_next_token(state);
    
    if (token.type == TOKEN_IDENTIFIER) {
        printf("ASSIGN TO: %s\n", token.value);
        
        token = get_next_token(state);
        if (token.type == TOKEN_OPERATOR && token.value[0] == '=') {
            printf("ASSIGNMENT OPERATOR\n");
            parse_expression(state);
        } else {
            printf("ERROR: Expected assignment operator\n");
        }
    } else {
        printf("ERROR: Expected identifier for assignment\n");
    }
}

int main() {
    char input_buffer[1024];
    
    printf("Enter an expression to parse (e.g., 'x = 5 + 3 * (2 - 1)'): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (strlen(input_buffer) == 0)