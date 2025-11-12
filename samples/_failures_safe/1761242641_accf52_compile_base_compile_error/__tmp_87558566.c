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
    if (state == NULL || input == NULL) return;
    state->input = input;
    state->position = 0;
    state->line = 1;
    state->column = 1;
    state->token_count = 0;
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int read_next_token(struct TokenizerState *state, struct Token *token) {
    if (state == NULL || token == NULL || state->input == NULL || state->position < 0) {
        return 0;
    }
    
    while (state->input[state->position] != '\0') {
        char current = state->input[state->position];
        
        if (isspace(current)) {
            if (current == '\n') {
                state->line++;
                state->column = 1;
            } else {
                state->column++;
            }
            state->position++;
            continue;
        }
        
        token->line = state->line;
        token->column = state->column;
        
        if (isdigit(current)) {
            int len = 0;
            while (isdigit(state->input[state->position]) && len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = state->input[state->position++];
                state->column++;
            }
            token->value[len] = '\0';
            token->type = TOKEN_NUMBER;
            return 1;
        }
        
        if (isalpha(current) || current == '_') {
            int len = 0;
            while ((isalnum(state->input[state->position]) || state->input[state->position] == '_') && len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = state->input[state->position++];
                state->column++;
            }
            token->value[len] = '\0';
            token->type = TOKEN_IDENTIFIER;
            return 1;
        }
        
        if (is_operator_char(current)) {
            int len = 0;
            while (is_operator_char(state->input[state->position]) && len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = state->input[state->position++];
                state->column++;
            }
            token->value[len] = '\0';
            token->type = TOKEN_OPERATOR;
            return 1;
        }
        
        if (current == '"') {
            int len = 0;
            state->position++;
            state->column++;
            while (state->input[state->position] != '"' && state->input[state->position] != '\0' && len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = state->input[state->position++];
                state->column++;
            }
            if (state->input[state->position] == '"') {
                state->position++;
                state->column++;
            }
            token->value[len] = '\0';
            token->type = TOKEN_STRING;
            return 1;
        }
        
        token->value[0] = current;
        token->value[1] = '\0';
        token->type = TOKEN_UNKNOWN;
        state->position++;
        state->column++;
        return 1;
    }
    
    token->type = TOKEN_EOF;
    token->value[0] = '\0';
    return 1;
}

int tokenize(struct TokenizerState *state, struct Token *tokens, int max_tokens) {
    if (state == NULL || tokens == NULL || max_tokens <= 0) {
        return 0;
    }
    
    int count = 0;
    struct Token token;
    
    while (count < max_tokens && read_next_token(state, &token)) {
        if (token.type == TOKEN_EOF) {
            tokens[count] = token;
            count++;
            break;
        }
        tokens[count] = token;
        count++;
    }
    
    state->token_count = count;
    return count;
}

void init_parser(struct ParserState *parser, struct Token *tokens, int token_count) {
    if (parser == NULL || tokens == NULL || token_count < 0) {
        return;
    }
    parser->tokens = tokens;
    parser->current_token = 0;
    parser->token_count = token_count;
}

struct Token *peek_token(struct ParserState *parser) {
    if (parser == NULL || parser->tokens == NULL || parser->current_token >= parser->token_count) {
        return NULL;
    }
    return &parser->tokens[parser->current_token];
}

struct Token *consume_token(struct ParserState *