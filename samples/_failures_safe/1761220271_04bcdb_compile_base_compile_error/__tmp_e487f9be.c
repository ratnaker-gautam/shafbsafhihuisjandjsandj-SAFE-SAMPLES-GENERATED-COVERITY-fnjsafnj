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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '(' || c == ')';
}

int is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n';
}

void skip_whitespace(struct ParserState *state) {
    while (state->position < state->length && 
           is_whitespace(state->input[state->position])) {
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
        while (state->position < state->length && 
               i < 31 && 
               isdigit(state->input[state->position])) {
            token.value[i++] = state->input[state->position++];
        }
        token.value[i] = '\0';
    } else if (isalpha(current)) {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (state->position < state->length && 
               i < 31 && 
               (isalnum(state->input[state->position]) || 
                state->input[state->position] == '_')) {
            token.value[i++] = state->input[state->position++];
        }
        token.value[i] = '\0';
    } else if (is_operator(current)) {
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

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "EOF", "INVALID"
    };
    printf("Token: %-12s Value: '%s'\n", type_names[token.type], token.value);
}

void parse_expression(struct ParserState *state, struct Token *current_token);

void parse_factor(struct ParserState *state, struct Token *current_token) {
    if (current_token->type == TOKEN_NUMBER || 
        current_token->type == TOKEN_IDENTIFIER) {
        print_token(*current_token);
        *current_token = get_next_token(state);
    } else if (current_token->type == TOKEN_OPERATOR && 
               current_token->value[0] == '(') {
        *current_token = get_next_token(state);
        parse_expression(state, current_token);
        if (current_token->type == TOKEN_OPERATOR && 
            current_token->value[0] == ')') {
            *current_token = get_next_token(state);
        }
    }
}

void parse_term(struct ParserState *state, struct Token *current_token) {
    parse_factor(state, current_token);
    
    while (current_token->type == TOKEN_OPERATOR && 
           (current_token->value[0] == '*' || 
            current_token->value[0] == '/')) {
        print_token(*current_token);
        *current_token = get_next_token(state);
        parse_factor(state, current_token);
    }
}

void parse_expression(struct ParserState *state, struct Token *current_token) {
    parse_term(state, current_token);
    
    while (current_token->type == TOKEN_OPERATOR && 
           (current_token->value[0] == '+' || 
            current_token->value[0] == '-')) {
        print_token(*current_token);
        *current_token = get_next_token(state);
        parse_term(state, current_token);
    }
}

int main(void) {
    char input[256];
    
    printf("Enter an arithmetic expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    struct ParserState state;
    init_parser(&state, input);
    
    printf("\nToken stream:\n");
    struct Token current_token = get_next_token(&state);
    
    while (current_token.type != TOKEN_EOF) {
        if (current_token.type == TOKEN_INVALID)