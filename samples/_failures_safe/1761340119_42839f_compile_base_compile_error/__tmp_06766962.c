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
    size_t position;
    size_t length;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '(' || c == ')';
}

void skip_whitespace(struct ParserState *state) {
    while (state->position < state->length && isspace(state->input[state->position])) {
        state->position++;
    }
}

struct Token get_next_token(struct ParserState *state) {
    struct Token token = {TOKEN_INVALID, ""};
    skip_whitespace(state);
    
    if (state->position >= state->length) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char current = state->input[state->position];
    
    if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        size_t i = 0;
        while (state->position < state->length && i < 31 && isdigit(state->input[state->position])) {
            token.value[i++] = state->input[state->position++];
        }
        token.value[i] = '\0';
    } else if (isalpha(current)) {
        token.type = TOKEN_IDENTIFIER;
        size_t i = 0;
        while (state->position < state->length && i < 31 && 
               (isalnum(state->input[state->position]) || state->input[state->position] == '_')) {
            token.value[i++] = state->input[state->position++];
        }
        token.value[i] = '\0';
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        size_t i = 0;
        while (state->position < state->length && i < 31 && is_operator_char(state->input[state->position])) {
            token.value[i++] = state->input[state->position++];
        }
        token.value[i] = '\0';
    } else {
        token.type = TOKEN_INVALID;
        token.value[0] = state->input[state->position++];
        token.value[1] = '\0';
    }
    
    return token;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "EOF", "INVALID"
    };
    printf("Token: %-12s Value: '%s'\n", type_names[token.type], token.value);
}

void parse_expression(struct ParserState *state);

void parse_primary(struct ParserState *state) {
    struct Token token = get_next_token(state);
    
    if (token.type == TOKEN_NUMBER || token.type == TOKEN_IDENTIFIER) {
        print_token(token);
    } else if (token.type == TOKEN_OPERATOR && token.value[0] == '(') {
        printf("Token: OPERATOR     Value: '('\n");
        parse_expression(state);
        token = get_next_token(state);
        if (token.type == TOKEN_OPERATOR && token.value[0] == ')') {
            printf("Token: OPERATOR     Value: ')'\n");
        } else {
            printf("Error: Expected closing parenthesis\n");
        }
    } else {
        printf("Error: Unexpected token in primary expression\n");
        print_token(token);
    }
}

void parse_term(struct ParserState *state) {
    parse_primary(state);
    
    struct Token token = get_next_token(state);
    if (token.type == TOKEN_OPERATOR && (strcmp(token.value, "*") == 0 || strcmp(token.value, "/") == 0)) {
        print_token(token);
        parse_term(state);
    } else {
        if (strlen(token.value) > 0) {
            state->position -= strlen(token.value);
        }
    }
}

void parse_expression(struct ParserState *state) {
    parse_term(state);
    
    struct Token token = get_next_token(state);
    if (token.type == TOKEN_OPERATOR && (strcmp(token.value, "+") == 0 || strcmp(token.value, "-") == 0)) {
        print_token(token);
        parse_expression(state);
    } else {
        if (strlen(token.value) > 0) {
            state->position -= strlen(token.value);
        }
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
    
    struct ParserState state = {
        .input = input,
        .position = 0,
        .length = len
    };
    
    printf("\nToken stream:\n");