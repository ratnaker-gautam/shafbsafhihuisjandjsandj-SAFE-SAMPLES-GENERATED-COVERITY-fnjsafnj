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

void init_parser(struct ParserState *state, const char *input) {
    state->input = input;
    state->position = 0;
    state->length = strlen(input);
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '(' || c == ')';
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
    } else if (isalpha(current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        size_t i = 0;
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

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "EOF", "INVALID"
    };
    printf("Token: %-12s Value: '%s'\n", type_names[token.type], token.value);
}

void parse_expression(struct ParserState *state);

void parse_primary(struct ParserState *state) {
    struct Token token = get_next_token(state);
    
    if (token.type == TOKEN_NUMBER) {
        printf("Found number: %s\n", token.value);
    } else if (token.type == TOKEN_IDENTIFIER) {
        printf("Found identifier: %s\n", token.value);
    } else if (token.type == TOKEN_OPERATOR && token.value[0] == '(') {
        printf("Entering subexpression\n");
        parse_expression(state);
        token = get_next_token(state);
        if (token.type != TOKEN_OPERATOR || token.value[0] != ')') {
            printf("Error: Expected closing parenthesis\n");
        } else {
            printf("Exiting subexpression\n");
        }
    } else {
        printf("Error: Expected primary expression\n");
    }
}

void parse_expression(struct ParserState *state) {
    parse_primary(state);
    
    struct Token token = get_next_token(state);
    
    if (token.type == TOKEN_OPERATOR && (token.value[0] == '+' || token.value[0] == '-' || 
                                         token.value[0] == '*' || token.value[0] == '/')) {
        printf("Found operator: %s\n", token.value);
        parse_expression(state);
    } else {
        if (state->position > 0 && state->position <= state->length) {
            size_t token_len = strlen(token.value);
            if (state->position >= token_len) {
                state->position -= token_len;
            }
        }
    }
}

int main(void) {
    char input[256];
    
    printf("Enter expression to parse: ");
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
    
    printf("\nToken stream:\n");
    struct Token token;
    do {
        token = get_next_token(&state);
        print_token(token);
    } while (token.type != TOKEN_EOF && token.type != TOKEN_INVALID);
    
    printf("\nParse tree:\n");
    init_parser