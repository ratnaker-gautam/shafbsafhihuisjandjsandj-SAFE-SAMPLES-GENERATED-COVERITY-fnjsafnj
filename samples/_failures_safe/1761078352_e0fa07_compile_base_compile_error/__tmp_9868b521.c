//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
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
    if (state == NULL || input == NULL) return;
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
    if (state == NULL) return;
    while (state->position < state->length && isspace(state->input[state->position])) {
        state->position++;
    }
}

struct Token get_next_token(struct ParserState *state) {
    struct Token token = {TOKEN_INVALID, ""};
    if (state == NULL) return token;
    
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

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "EOF", "INVALID"
    };
    if (token.type >= TOKEN_NUMBER && token.type <= TOKEN_INVALID) {
        printf("Token(%s, '%s')\n", type_names[token.type], token.value);
    } else {
        printf("Token(INVALID, '%s')\n", token.value);
    }
}

void parse_expression(struct ParserState *state, struct Token *current_token);

void parse_primary(struct ParserState *state, struct Token *current_token) {
    if (state == NULL || current_token == NULL) return;
    
    if (current_token->type == TOKEN_NUMBER || current_token->type == TOKEN_IDENTIFIER) {
        print_token(*current_token);
        *current_token = get_next_token(state);
    } else if (current_token->type == TOKEN_OPERATOR && current_token->value[0] == '(') {
        printf("Token(OPERATOR, '(')\n");
        *current_token = get_next_token(state);
        parse_expression(state, current_token);
        if (current_token->type == TOKEN_OPERATOR && current_token->value[0] == ')') {
            printf("Token(OPERATOR, ')')\n");
            *current_token = get_next_token(state);
        } else {
            printf("Error: Expected ')'\n");
        }
    } else {
        printf("Error: Expected primary expression\n");
    }
}

void parse_term(struct ParserState *state, struct Token *current_token) {
    if (state == NULL || current_token == NULL) return;
    
    parse_primary(state, current_token);
    
    while (current_token->type == TOKEN_OPERATOR && 
           (current_token->value[0] == '*' || current_token->value[0] == '/')) {
        print_token(*current_token);
        *current_token = get_next_token(state);
        parse_primary(state, current_token);
    }
}

void parse_expression(struct ParserState *state, struct Token *current_token) {
    if (state == NULL || current_token == NULL) return;
    
    parse_term(state, current_token);
    
    while (current_token->type == TOKEN_OPERATOR && 
           (current_token->value[0] == '+' || current_token->value[0] == '-')) {
        print_token(*current_token);
        *current_token = get_next_token(state);
        parse_term(state, current_token);
    }
}

void parse_assignment(struct ParserState *state, struct Token *current_token) {
    if (state == NULL || current_token == NULL) return;
    
    if (current