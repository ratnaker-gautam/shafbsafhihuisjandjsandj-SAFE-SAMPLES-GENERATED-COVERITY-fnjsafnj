//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOK_NUMBER, TOK_IDENT, TOK_OPERATOR, TOK_EOF, TOK_INVALID };

struct Token {
    enum TokenType type;
    char value[32];
};

struct ParserState {
    const char *input;
    size_t pos;
    size_t len;
};

void init_parser(struct ParserState *state, const char *input) {
    if (state == NULL || input == NULL) {
        fprintf(stderr, "Invalid parser state or input\n");
        exit(1);
    }
    state->input = input;
    state->pos = 0;
    state->len = strlen(input);
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

void skip_whitespace(struct ParserState *state) {
    while (state->pos < state->len && isspace(state->input[state->pos])) {
        state->pos++;
    }
}

struct Token parse_number(struct ParserState *state) {
    struct Token token = {TOK_NUMBER, ""};
    size_t i = 0;
    while (state->pos < state->len && i < 31 && isdigit(state->input[state->pos])) {
        token.value[i++] = state->input[state->pos++];
    }
    token.value[i] = '\0';
    return token;
}

struct Token parse_ident(struct ParserState *state) {
    struct Token token = {TOK_IDENT, ""};
    size_t i = 0;
    while (state->pos < state->len && i < 31 && (isalnum(state->input[state->pos]) || state->input[state->pos] == '_')) {
        token.value[i++] = state->input[state->pos++];
    }
    token.value[i] = '\0';
    return token;
}

struct Token parse_operator(struct ParserState *state) {
    struct Token token = {TOK_OPERATOR, ""};
    token.value[0] = state->input[state->pos++];
    token.value[1] = '\0';
    return token;
}

struct Token get_next_token(struct ParserState *state) {
    skip_whitespace(state);
    if (state->pos >= state->len) {
        struct Token token = {TOK_EOF, ""};
        return token;
    }
    char current = state->input[state->pos];
    if (isdigit(current)) {
        return parse_number(state);
    } else if (isalpha(current) || current == '_') {
        return parse_ident(state);
    } else if (is_operator(current)) {
        return parse_operator(state);
    } else {
        struct Token token = {TOK_INVALID, ""};
        token.value[0] = current;
        token.value[1] = '\0';
        state->pos++;
        return token;
    }
}

void print_token(struct Token token) {
    const char *type_names[] = {"NUMBER", "IDENT", "OPERATOR", "EOF", "INVALID"};
    printf("Token: %-10s Value: '%s'\n", type_names[token.type], token.value);
}

void parse_expression(struct ParserState *state);

void parse_primary(struct ParserState *state) {
    struct Token token = get_next_token(state);
    if (token.type == TOK_NUMBER || token.type == TOK_IDENT) {
        print_token(token);
    } else if (token.type == TOK_OPERATOR && token.value[0] == '(') {
        parse_expression(state);
        token = get_next_token(state);
        if (token.type != TOK_OPERATOR || token.value[0] != ')') {
            fprintf(stderr, "Expected closing parenthesis\n");
            exit(1);
        }
    } else {
        fprintf(stderr, "Unexpected token in primary expression\n");
        print_token(token);
        exit(1);
    }
}

void parse_expression(struct ParserState *state) {
    parse_primary(state);
    struct Token token = get_next_token(state);
    if (token.type == TOK_OPERATOR && (token.value[0] == '+' || token.value[0] == '-' || token.value[0] == '*' || token.value[0] == '/')) {
        print_token(token);
        parse_expression(state);
    } else {
        state->pos--;
    }
}

int main(void) {
    char input[256];
    printf("Enter expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Failed to read input\n");
        return 1;
    }
    input[strcspn(input, "\n")] = '\0';
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    struct ParserState state;
    init_parser(&state, input);
    printf("Parsing expression: %s\n", input);
    parse_expression(&state);
    struct Token token = get_next_token(&state);
    if (token.type != TOK_EOF) {
        fprintf(stderr, "Unexpected tokens after expression\n");
        print_token(token);
        return 1;
    }
    printf("Parsing completed successfully\n");
    return 0;
}