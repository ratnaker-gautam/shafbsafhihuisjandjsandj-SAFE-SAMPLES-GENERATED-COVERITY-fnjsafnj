//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

enum TokenType {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
    TOKEN_KEYWORD,
    TOKEN_UNKNOWN
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
    int line;
    int column;
};

struct TokenizerState {
    char *input;
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

int is_keyword(const char *str) {
    static const char *keywords[] = {"if", "else", "while", "for", "return", "int", "float", "char", NULL};
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int is_operator(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int is_delimiter(char c) {
    return strchr("(){}[],;", c) != NULL;
}

void init_tokenizer(struct TokenizerState *state, char *input) {
    if (state == NULL || input == NULL) return;
    state->input = input;
    state->position = 0;
    state->line = 1;
    state->column = 1;
    state->token_count = 0;
}

void init_parser(struct ParserState *state, struct Token *tokens, int count) {
    if (state == NULL || tokens == NULL || count < 0) return;
    state->tokens = tokens;
    state->current_token = 0;
    state->token_count = count;
}

int get_next_token(struct TokenizerState *state, struct Token *token) {
    if (state == NULL || token == NULL || state->input == NULL) return 0;
    if (state->token_count >= MAX_TOKENS) return 0;
    
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
        
        if (isalpha(current) || current == '_') {
            int len = 0;
            while ((isalnum(state->input[state->position]) || state->input[state->position] == '_') && len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = state->input[state->position++];
                state->column++;
            }
            token->value[len] = '\0';
            token->type = is_keyword(token->value) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
            state->token_count++;
            return 1;
        }
        
        if (isdigit(current)) {
            int len = 0;
            while (isdigit(state->input[state->position]) && len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = state->input[state->position++];
                state->column++;
            }
            token->value[len] = '\0';
            token->type = TOKEN_NUMBER;
            state->token_count++;
            return 1;
        }
        
        if (is_operator(current)) {
            int len = 0;
            while (is_operator(state->input[state->position]) && len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = state->input[state->position++];
                state->column++;
            }
            token->value[len] = '\0';
            token->type = TOKEN_OPERATOR;
            state->token_count++;
            return 1;
        }
        
        if (is_delimiter(current)) {
            token->value[0] = state->input[state->position++];
            token->value[1] = '\0';
            token->type = TOKEN_DELIMITER;
            state->column++;
            state->token_count++;
            return 1;
        }
        
        token->value[0] = state->input[state->position++];
        token->value[1] = '\0';
        token->type = TOKEN_UNKNOWN;
        state->column++;
        state->token_count++;
        return 1;
    }
    
    return 0;
}

void parse_expression(struct ParserState *state) {
    if (state == NULL || state->current_token >= state->token_count) return;
    
    struct Token *current = &state->tokens[state->current_token];
    
    if (current->type == TOKEN_IDENTIFIER || current->type == TOKEN_NUMBER) {
        state->current_token++;
        if (state->current_token < state->token_count && state->tokens[state->current_token].type == TOKEN_OPERATOR) {
            state->current_token++;
            parse_expression(state);
        }
    }
}

void parse_statement(struct ParserState *state) {
    if (state == NULL