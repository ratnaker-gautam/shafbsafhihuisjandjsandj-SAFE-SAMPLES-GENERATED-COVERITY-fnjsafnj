//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_UNKNOWN
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
    int line;
    int column;
};

struct TokenizerState {
    const char *input;
    int position;
    int line;
    int column;
    int token_count;
};

struct ParserState {
    struct Token *tokens;
    int current_token;
    int token_count;
};

void init_tokenizer(struct TokenizerState *state, const char *input) {
    state->input = input;
    state->position = 0;
    state->line = 1;
    state->column = 1;
    state->token_count = 0;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(struct TokenizerState *state) {
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

int tokenize_number(struct TokenizerState *state, struct Token *token) {
    int start = state->position;
    int len = 0;
    
    while (isdigit(state->input[state->position])) {
        state->position++;
        state->column++;
        len++;
        if (len >= MAX_TOKEN_LEN - 1) break;
    }
    
    if (len > 0) {
        token->type = TOKEN_NUMBER;
        strncpy(token->value, &state->input[start], len);
        token->value[len] = '\0';
        token->line = state->line;
        token->column = state->column - len;
        return 1;
    }
    return 0;
}

int tokenize_identifier(struct TokenizerState *state, struct Token *token) {
    int start = state->position;
    int len = 0;
    
    if (isalpha(state->input[state->position]) || state->input[state->position] == '_') {
        while (isalnum(state->input[state->position]) || state->input[state->position] == '_') {
            state->position++;
            state->column++;
            len++;
            if (len >= MAX_TOKEN_LEN - 1) break;
        }
        
        token->type = TOKEN_IDENTIFIER;
        strncpy(token->value, &state->input[start], len);
        token->value[len] = '\0';
        token->line = state->line;
        token->column = state->column - len;
        return 1;
    }
    return 0;
}

int tokenize_operator(struct TokenizerState *state, struct Token *token) {
    if (is_operator_char(state->input[state->position])) {
        token->type = TOKEN_OPERATOR;
        token->value[0] = state->input[state->position];
        token->value[1] = '\0';
        token->line = state->line;
        token->column = state->column;
        state->position++;
        state->column++;
        return 1;
    }
    return 0;
}

int tokenize_string(struct TokenizerState *state, struct Token *token) {
    if (state->input[state->position] == '"') {
        int start = state->position;
        int len = 0;
        state->position++;
        state->column++;
        
        while (state->input[state->position] != '"' && state->input[state->position] != '\0') {
            state->position++;
            state->column++;
            len++;
            if (len >= MAX_TOKEN_LEN - 1) break;
        }
        
        if (state->input[state->position] == '"') {
            token->type = TOKEN_STRING;
            strncpy(token->value, &state->input[start + 1], len);
            token->value[len] = '\0';
            token->line = state->line;
            token->column = state->column - len - 1;
            state->position++;
            state->column++;
            return 1;
        }
    }
    return 0;
}

int get_next_token(struct TokenizerState *state, struct Token *token) {
    skip_whitespace(state);
    
    if (state->input[state->position] == '\0') {
        token->type = TOKEN_EOF;
        strcpy(token->value, "");
        token->line = state->line;
        token->column = state->column;
        return 1;
    }
    
    if (tokenize_number(state, token)) return 1;
    if (tokenize_identifier(state, token)) return 1;
    if (tokenize_operator(state, token)) return 1;
    if (tokenize_string(state, token)) return 1;
    
    token->type = TOKEN_UNKNOWN;
    token->