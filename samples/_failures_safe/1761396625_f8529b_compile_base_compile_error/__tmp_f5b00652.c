//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64
#define MAX_INPUT_LEN 1024

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
    TOKEN_KEYWORD,
    TOKEN_UNKNOWN
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
    uint32_t line;
    uint32_t column;
};

struct TokenizerState {
    char *input;
    uint32_t position;
    uint32_t line;
    uint32_t column;
    uint32_t token_count;
};

struct ParserContext {
    struct Token tokens[MAX_TOKENS];
    uint32_t token_index;
    uint32_t token_total;
};

void initialize_tokenizer(struct TokenizerState *state, char *input) {
    if (state == NULL || input == NULL) return;
    state->input = input;
    state->position = 0;
    state->line = 1;
    state->column = 1;
    state->token_count = 0;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_delimiter_char(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == ';' || c == ',';
}

int is_keyword(const char *str) {
    static const char *keywords[] = {"if", "else", "while", "for", "return", NULL};
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0) return 1;
    }
    return 0;
}

void skip_whitespace(struct TokenizerState *state) {
    if (state == NULL || state->input == NULL) return;
    while (state->input[state->position] != '\0' && isspace(state->input[state->position])) {
        if (state->input[state->position] == '\n') {
            state->line++;
            state->column = 1;
        } else {
            state->column++;
        }
        state->position++;
    }
}

int get_next_token(struct TokenizerState *state, struct Token *token) {
    if (state == NULL || token == NULL || state->input == NULL) return 0;
    if (state->token_count >= MAX_TOKENS) return 0;
    
    skip_whitespace(state);
    
    if (state->input[state->position] == '\0') return 0;
    
    token->line = state->line;
    token->column = state->column;
    
    char current = state->input[state->position];
    
    if (isdigit(current)) {
        token->type = TOKEN_NUMBER;
        int i = 0;
        while (i < MAX_TOKEN_LEN - 1 && isdigit(state->input[state->position])) {
            token->value[i++] = state->input[state->position++];
            state->column++;
        }
        token->value[i] = '\0';
    } else if (isalpha(current) || current == '_') {
        token->type = TOKEN_IDENTIFIER;
        int i = 0;
        while (i < MAX_TOKEN_LEN - 1 && (isalnum(state->input[state->position]) || state->input[state->position] == '_')) {
            token->value[i++] = state->input[state->position++];
            state->column++;
        }
        token->value[i] = '\0';
        if (is_keyword(token->value)) {
            token->type = TOKEN_KEYWORD;
        }
    } else if (is_operator_char(current)) {
        token->type = TOKEN_OPERATOR;
        int i = 0;
        while (i < MAX_TOKEN_LEN - 1 && is_operator_char(state->input[state->position])) {
            token->value[i++] = state->input[state->position++];
            state->column++;
        }
        token->value[i] = '\0';
    } else if (is_delimiter_char(current)) {
        token->type = TOKEN_DELIMITER;
        token->value[0] = state->input[state->position++];
        token->value[1] = '\0';
        state->column++;
    } else {
        token->type = TOKEN_UNKNOWN;
        token->value[0] = state->input[state->position++];
        token->value[1] = '\0';
        state->column++;
    }
    
    state->token_count++;
    return 1;
}

void initialize_parser(struct ParserContext *context, struct Token *tokens, uint32_t count) {
    if (context == NULL || tokens == NULL) return;
    uint32_t copy_count = count < MAX_TOKENS ? count : MAX_TOKENS;
    for (uint32_t i = 0; i < copy_count; i++) {
        context->tokens[i] = tokens[i];
    }
    context->token_index = 0;
    context->token_total = copy_count;
}

int parser_accept(struct ParserContext *context, enum TokenType type) {
    if (context == NULL || context->