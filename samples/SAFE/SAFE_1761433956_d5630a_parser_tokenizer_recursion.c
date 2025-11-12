//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <ctype.h>
#include <string.h>

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
    size_t pos;
    size_t len;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

void skip_whitespace(struct ParserState *state) {
    while (state->pos < state->len && isspace(state->input[state->pos])) {
        state->pos++;
    }
}

struct Token get_next_token(struct ParserState *state) {
    struct Token token = {TOKEN_INVALID, ""};
    
    skip_whitespace(state);
    
    if (state->pos >= state->len) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char current = state->input[state->pos];
    
    if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        size_t i = 0;
        while (state->pos < state->len && i < 31 && isdigit(state->input[state->pos])) {
            token.value[i++] = state->input[state->pos++];
        }
        token.value[i] = '\0';
    } else if (isalpha(current)) {
        token.type = TOKEN_IDENTIFIER;
        size_t i = 0;
        while (state->pos < state->len && i < 31 && (isalnum(state->input[state->pos]) || state->input[state->pos] == '_')) {
            token.value[i++] = state->input[state->pos++];
        }
        token.value[i] = '\0';
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        token.value[0] = state->input[state->pos++];
        token.value[1] = '\0';
    } else {
        token.type = TOKEN_INVALID;
        token.value[0] = state->input[state->pos++];
        token.value[1] = '\0';
    }
    
    return token;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "EOF", "INVALID"
    };
    printf("Token: %-12s Value: %s\n", type_names[token.type], token.value);
}

void parse_expression(struct ParserState *state);

void parse_primary(struct ParserState *state) {
    struct Token token = get_next_token(state);
    
    if (token.type == TOKEN_NUMBER) {
        printf("Found number: %s\n", token.value);
    } else if (token.type == TOKEN_IDENTIFIER) {
        printf("Found identifier: %s\n", token.value);
    } else if (token.type == TOKEN_OPERATOR && token.value[0] == '(') {
        printf("Entering parentheses\n");
        parse_expression(state);
        token = get_next_token(state);
        if (token.type != TOKEN_OPERATOR || token.value[0] != ')') {
            printf("Error: Expected closing parenthesis\n");
        } else {
            printf("Exiting parentheses\n");
        }
    } else {
        printf("Error: Unexpected token in primary expression\n");
        print_token(token);
    }
}

void parse_expression(struct ParserState *state) {
    parse_primary(state);
    
    struct Token token = get_next_token(state);
    
    if (token.type == TOKEN_OPERATOR && (token.value[0] == '+' || token.value[0] == '-' || 
                                         token.value[0] == '*' || token.value[0] == '/')) {
        printf("Found operator: %s\n", token.value);
        parse_expression(state);
    } else if (token.type != TOKEN_EOF && token.type != TOKEN_INVALID) {
        state->pos -= strlen(token.value);
    }
}

int main() {
    char input[256];
    
    printf("Enter an expression to parse: ");
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
    
    struct ParserState state = {input, 0, len};
    
    printf("\nParsing expression: %s\n", input);
    printf("Token stream:\n");
    
    struct Token token;
    do {
        token = get_next_token(&state);
        print_token(token);
    } while (token.type != TOKEN_EOF && token.type != TOKEN_INVALID);
    
    state.pos = 0;
    printf("\nRecursive descent parse:\n");
    parse_expression(&state);
    
    return 0;
}