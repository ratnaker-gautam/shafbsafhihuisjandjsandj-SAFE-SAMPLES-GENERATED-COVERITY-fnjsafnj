//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <ctype.h>
#include <string.h>

enum TokenType {
    TOK_NUMBER,
    TOK_IDENTIFIER,
    TOK_OPERATOR,
    TOK_EOF,
    TOK_INVALID
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

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void init_parser(struct ParserState *state, const char *input) {
    state->input = input;
    state->position = 0;
    state->line = 1;
    state->column = 1;
    state->current_token.type = TOK_INVALID;
    memset(state->current_token.value, 0, sizeof(state->current_token.value));
}

void skip_whitespace(struct ParserState *state) {
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
    skip_whitespace(state);
    
    if (state->input[state->position] == '\0') {
        struct Token tok = {TOK_EOF, "", state->line, state->column};
        return tok;
    }
    
    char c = state->input[state->position];
    int start_col = state->column;
    
    if (isdigit(c)) {
        struct Token tok = {TOK_NUMBER, "", state->line, state->column};
        int i = 0;
        while (state->input[state->position] != '\0' && 
               isdigit(state->input[state->position]) && i < 31) {
            tok.value[i++] = state->input[state->position++];
            state->column++;
        }
        tok.value[i] = '\0';
        return tok;
    }
    
    if (isalpha(c)) {
        struct Token tok = {TOK_IDENTIFIER, "", state->line, state->column};
        int i = 0;
        while (state->input[state->position] != '\0' && 
               (isalnum(state->input[state->position]) || state->input[state->position] == '_') && i < 31) {
            tok.value[i++] = state->input[state->position++];
            state->column++;
        }
        tok.value[i] = '\0';
        return tok;
    }
    
    if (is_operator_char(c)) {
        struct Token tok = {TOK_OPERATOR, "", state->line, state->column};
        int i = 0;
        while (state->input[state->position] != '\0' && 
               is_operator_char(state->input[state->position]) && i < 31) {
            tok.value[i++] = state->input[state->position++];
            state->column++;
        }
        tok.value[i] = '\0';
        return tok;
    }
    
    struct Token tok = {TOK_INVALID, "", state->line, state->column};
    tok.value[0] = c;
    tok.value[1] = '\0';
    state->position++;
    state->column++;
    return tok;
}

void consume_token(struct ParserState *state) {
    state->current_token = get_next_token(state);
}

void parse_expression(struct ParserState *state);

void parse_primary(struct ParserState *state) {
    if (state->current_token.type == TOK_NUMBER) {
        printf("Number: %s\n", state->current_token.value);
        consume_token(state);
    } else if (state->current_token.type == TOK_IDENTIFIER) {
        printf("Identifier: %s\n", state->current_token.value);
        consume_token(state);
    } else {
        printf("Error: Expected number or identifier at line %d, column %d\n", 
               state->current_token.line, state->current_token.column);
    }
}

void parse_expression(struct ParserState *state) {
    parse_primary(state);
    
    if (state->current_token.type == TOK_OPERATOR) {
        printf("Operator: %s\n", state->current_token.value);
        consume_token(state);
        parse_expression(state);
    }
}

int parse_input(struct ParserState *state) {
    consume_token(state);
    
    while (state->current_token.type != TOK_EOF) {
        if (state->current_token.type == TOK_INVALID) {
            printf("Error: Invalid token '%s' at line %d, column %d\n", 
                   state->current_token.value, state->current_token.line, state->current_token.column);
            return 0;
        }
        parse_expression(state);
        
        if (state->current_token.type == TOK_EOF) {
            break;
        }
    }
    
    return 1;
}

int main(void) {
    char input[1024];
    printf("Enter expression: ");
    if (f