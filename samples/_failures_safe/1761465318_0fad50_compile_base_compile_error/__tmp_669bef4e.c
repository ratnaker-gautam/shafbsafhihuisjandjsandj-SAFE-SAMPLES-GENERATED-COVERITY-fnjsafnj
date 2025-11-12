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

void init_parser(struct ParserState *state, const char *str) {
    state->input = str;
    state->pos = 0;
    state->len = strlen(str);
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '(' || c == ')';
}

struct Token get_next_token(struct ParserState *state) {
    struct Token token = {TOK_INVALID, ""};
    
    if (state->pos >= state->len) {
        token.type = TOK_EOF;
        return token;
    }
    
    while (state->pos < state->len && isspace(state->input[state->pos])) {
        state->pos++;
    }
    
    if (state->pos >= state->len) {
        token.type = TOK_EOF;
        return token;
    }
    
    char current = state->input[state->pos];
    
    if (isdigit(current)) {
        token.type = TOK_NUMBER;
        size_t i = 0;
        while (state->pos < state->len && i < 31 && isdigit(state->input[state->pos])) {
            token.value[i++] = state->input[state->pos++];
        }
        token.value[i] = '\0';
    } else if (isalpha(current)) {
        token.type = TOK_IDENT;
        size_t i = 0;
        while (state->pos < state->len && i < 31 && (isalnum(state->input[state->pos]) || state->input[state->pos] == '_')) {
            token.value[i++] = state->input[state->pos++];
        }
        token.value[i] = '\0';
    } else if (is_operator(current)) {
        token.type = TOK_OPERATOR;
        token.value[0] = state->input[state->pos++];
        token.value[1] = '\0';
    } else {
        token.type = TOK_INVALID;
        token.value[0] = state->input[state->pos++];
        token.value[1] = '\0';
    }
    
    return token;
}

void print_token(struct Token token) {
    const char *type_names[] = {"NUMBER", "IDENT", "OPERATOR", "EOF", "INVALID"};
    printf("Token(%s, '%s')\n", type_names[token.type], token.value);
}

void parse_expression(struct ParserState *state);

void parse_primary(struct ParserState *state) {
    struct Token token = get_next_token(state);
    
    if (token.type == TOK_NUMBER) {
        printf("Found number: %s\n", token.value);
    } else if (token.type == TOK_IDENT) {
        printf("Found identifier: %s\n", token.value);
    } else if (token.type == TOK_OPERATOR && token.value[0] == '(') {
        printf("Entering subexpression\n");
        parse_expression(state);
        struct Token close_paren = get_next_token(state);
        if (close_paren.type != TOK_OPERATOR || close_paren.value[0] != ')') {
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
    
    struct Token next = get_next_token(state);
    
    if (next.type == TOK_OPERATOR && (next.value[0] == '+' || next.value[0] == '-' || 
                                      next.value[0] == '*' || next.value[0] == '/')) {
        printf("Found operator: %s\n", next.value);
        parse_expression(state);
    } else {
        if (next.type != TOK_EOF && state->pos >= strlen(next.value)) {
            state->pos -= strlen(next.value);
        }
    }
}

int main(void) {
    char input[256];
    
    printf("Enter expression to parse: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
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
    printf("Token stream:\n");
    
    struct ParserState token_state;
    init_parser(&token_state, input);
    
    struct Token token;
    do {
        token = get_next_token(&token_state);
        print_token(token);
    } while (token.type != TOK_EOF && token.type != TOK_INVALID);
    
    printf("\nRecursive parse:\n");
    parse_expression(&state);