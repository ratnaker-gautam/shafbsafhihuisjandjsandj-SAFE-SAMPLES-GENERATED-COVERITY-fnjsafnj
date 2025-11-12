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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
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
    printf("%s: %s\n", type_names[token.type], token.value);
}

void parse_expression(struct ParserState *state);

void parse_primary(struct ParserState *state) {
    struct Token token = get_next_token(state);
    
    if (token.type == TOKEN_NUMBER) {
        printf("Found number: %s\n", token.value);
    } else if (token.type == TOKEN_IDENTIFIER) {
        printf("Found identifier: %s\n", token.value);
    } else {
        printf("Unexpected token in primary: ");
        print_token(token);
    }
}

void parse_term(struct ParserState *state) {
    parse_primary(state);
    
    struct Token token = get_next_token(state);
    if (token.type == TOKEN_OPERATOR && (token.value[0] == '*' || token.value[0] == '/')) {
        printf("Found operator: %s\n", token.value);
        parse_term(state);
    } else {
        state->position -= strlen(token.value);
    }
}

void parse_expression(struct ParserState *state) {
    parse_term(state);
    
    struct Token token = get_next_token(state);
    if (token.type == TOKEN_OPERATOR && (token.value[0] == '+' || token.value[0] == '-')) {
        printf("Found operator: %s\n", token.value);
        parse_expression(state);
    } else {
        state->position -= strlen(token.value);
    }
}

int main() {
    char input[256];
    
    printf("Enter an expression to parse: ");
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
    state.input = input;
    state.position = 0;
    state.length = strlen(input);
    
    printf("\nParsing expression: %s\n", input);
    printf("Token stream:\n");
    
    struct ParserState token_state = state;
    while (token_state.position < token_state.length) {
        struct Token token = get_next_token(&token_state);
        if (token.type == TOKEN_EOF) break;
        print_token(token);
    }
    
    printf("\nRecursive descent parse:\n");
    parse_expression(&state);
    
    struct Token final_token = get_next_token(&state);
    if (final_token.type != TOKEN_EOF) {
        printf("Unexpected token at end: ");
        print_token(final_token);
    } else {
        printf("Parse completed successfully\n");
    }
    
    return 0;
}