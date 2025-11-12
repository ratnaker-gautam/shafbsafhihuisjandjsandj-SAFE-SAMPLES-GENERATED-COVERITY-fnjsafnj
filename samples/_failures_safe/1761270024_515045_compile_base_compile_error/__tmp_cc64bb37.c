//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char value[32];
    int line;
    int column;
};

struct ParserState {
    const char *input;
    int position;
    int line;
    int column;
    struct Token current_token;
};

void init_parser(struct ParserState *state, const char *input) {
    if (state == NULL || input == NULL) return;
    state->input = input;
    state->position = 0;
    state->line = 1;
    state->column = 1;
    state->current_token.type = TOKEN_INVALID;
    memset(state->current_token.value, 0, sizeof(state->current_token.value));
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(struct ParserState *state) {
    if (state == NULL || state->input == NULL) return;
    
    while (state->input[state->position] != '\0') {
        char c = state->input[state->position];
        if (c == ' ' || c == '\t') {
            state->position++;
            state->column++;
        } else if (c == '\n') {
            state->position++;
            state->line++;
            state->column = 1;
        } else {
            break;
        }
    }
}

struct Token get_next_token(struct ParserState *state) {
    if (state == NULL || state->input == NULL) {
        struct Token invalid = {TOKEN_INVALID, "", 0, 0};
        return invalid;
    }
    
    skip_whitespace(state);
    
    if (state->input[state->position] == '\0') {
        struct Token eof = {TOKEN_EOF, "", state->line, state->column};
        return eof;
    }
    
    char current = state->input[state->position];
    struct Token token;
    token.line = state->line;
    token.column = state->column;
    memset(token.value, 0, sizeof(token.value));
    
    if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (state->input[state->position] != '\0' && 
               i < (int)sizeof(token.value) - 1 && 
               isdigit(state->input[state->position])) {
            token.value[i++] = state->input[state->position++];
            state->column++;
        }
        token.value[i] = '\0';
    } else if (isalpha(current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (state->input[state->position] != '\0' && 
               i < (int)sizeof(token.value) - 1 && 
               (isalnum(state->input[state->position]) || state->input[state->position] == '_')) {
            token.value[i++] = state->input[state->position++];
            state->column++;
        }
        token.value[i] = '\0';
    } else if (is_operator(current)) {
        token.type = TOKEN_OPERATOR;
        token.value[0] = state->input[state->position++];
        token.value[1] = '\0';
        state->column++;
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        state->position++;
        state->column++;
        int i = 0;
        while (state->input[state->position] != '\0' && 
               i < (int)sizeof(token.value) - 1 && 
               state->input[state->position] != '"') {
            token.value[i++] = state->input[state->position++];
            state->column++;
        }
        if (state->input[state->position] == '"') {
            state->position++;
            state->column++;
        }
        token.value[i] = '\0';
    } else {
        token.type = TOKEN_INVALID;
        token.value[0] = state->input[state->position++];
        token.value[1] = '\0';
        state->column++;
    }
    
    return token;
}

void print_token(const struct Token *token) {
    if (token == NULL) return;
    
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "INVALID"
    };
    
    printf("Line %d, Column %d: %s", token->line, token->column, type_names[token->type]);
    
    if (token->type != TOKEN_EOF) {
        printf(" ('%s')", token->value);
    }
    printf("\n");
}

void process_tokens(const char *input) {
    if (input == NULL) return;
    
    struct ParserState state;
    init_parser(&state, input);
    
    printf("Input: \"%s\"\n", input);
    printf("Tokens:\n");
    
    while (1) {
        struct Token token = get_next_token(&state);
        print_token(&token);
        
        if (token.type == TO