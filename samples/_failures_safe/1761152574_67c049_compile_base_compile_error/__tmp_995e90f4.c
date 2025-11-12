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
    size_t length;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

int parse_number(struct ParserState *state, struct Token *token) {
    size_t len = 0;
    
    while (state->pos < state->length && isdigit(state->input[state->pos])) {
        if (len >= sizeof(token->value) - 1) {
            return 0;
        }
        token->value[len++] = state->input[state->pos++];
    }
    
    if (len == 0) {
        return 0;
    }
    
    token->value[len] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_identifier(struct ParserState *state, struct Token *token) {
    size_t len = 0;
    
    if (state->pos >= state->length || !isalpha(state->input[state->pos])) {
        return 0;
    }
    
    while (state->pos < state->length && 
           (isalnum(state->input[state->pos]) || state->input[state->pos] == '_')) {
        if (len >= sizeof(token->value) - 1) {
            return 0;
        }
        token->value[len++] = state->input[state->pos++];
    }
    
    token->value[len] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_operator(struct ParserState *state, struct Token *token) {
    if (state->pos >= state->length || !is_operator_char(state->input[state->pos])) {
        return 0;
    }
    
    token->value[0] = state->input[state->pos++];
    token->value[1] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

void skip_whitespace(struct ParserState *state) {
    while (state->pos < state->length && isspace(state->input[state->pos])) {
        state->pos++;
    }
}

int get_next_token(struct ParserState *state, struct Token *token) {
    skip_whitespace(state);
    
    if (state->pos >= state->length) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }
    
    if (parse_number(state, token)) return 1;
    if (parse_identifier(state, token)) return 1;
    if (parse_operator(state, token)) return 1;
    
    token->type = TOKEN_INVALID;
    token->value[0] = state->input[state->pos++];
    token->value[1] = '\0';
    return 1;
}

void print_token(struct Token *token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "EOF", "INVALID"
    };
    
    printf("Token: %-12s Value: '%s'\n", type_names[token->type], token->value);
}

void parse_expression(struct ParserState *state);

void parse_factor(struct ParserState *state) {
    struct Token token;
    
    if (!get_next_token(state, &token)) {
        return;
    }
    
    print_token(&token);
    
    if (token.type == TOKEN_NUMBER || token.type == TOKEN_IDENTIFIER) {
        return;
    } else if (token.type == TOKEN_OPERATOR && token.value[0] == '-') {
        parse_factor(state);
    } else {
        printf("Syntax error: unexpected token in factor\n");
    }
}

void parse_term(struct ParserState *state) {
    parse_factor(state);
    
    struct Token token;
    if (!get_next_token(state, &token)) {
        return;
    }
    
    if (token.type == TOKEN_OPERATOR && 
        (token.value[0] == '*' || token.value[0] == '/')) {
        print_token(&token);
        parse_factor(state);
    } else {
        if (state->pos > 0) {
            state->pos--;
        }
    }
}

void parse_expression(struct ParserState *state) {
    parse_term(state);
    
    struct Token token;
    if (!get_next_token(state, &token)) {
        return;
    }
    
    if (token.type == TOKEN_OPERATOR && 
        (token.value[0] == '+' || token.value[0] == '-')) {
        print_token(&token);
        parse_term(state);
    } else {
        if (state->pos > 0) {
            state->pos--;
        }
    }
}

int main(void) {
    char input[256];
    
    printf("Enter an expression to parse: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] ==