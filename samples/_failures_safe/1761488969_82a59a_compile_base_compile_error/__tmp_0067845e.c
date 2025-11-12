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
    return c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ';' || c == ',';
}

int is_keyword(const char *str) {
    const char *keywords[] = {"if", "else", "while", "for", "return", "int", "float"};
    const int num_keywords = 7;
    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void add_token(struct TokenizerState *state, struct ParserContext *context, enum TokenType type, const char *value) {
    if (state == NULL || context == NULL || value == NULL) return;
    if (context->token_total >= MAX_TOKENS) return;
    
    struct Token *token = &context->tokens[context->token_total];
    token->type = type;
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
    token->line = state->line;
    token->column = state->column - strlen(value);
    context->token_total++;
    state->token_count++;
}

int tokenize(struct TokenizerState *state, struct ParserContext *context) {
    if (state == NULL || context == NULL || state->input == NULL) return 0;
    
    context->token_total = 0;
    char current_token[MAX_TOKEN_LEN];
    uint32_t token_len = 0;
    
    while (state->input[state->position] != '\0' && context->token_total < MAX_TOKENS) {
        char c = state->input[state->position];
        
        if (isspace(c)) {
            if (c == '\n') {
                state->line++;
                state->column = 1;
            } else {
                state->column++;
            }
            state->position++;
            continue;
        }
        
        if (isdigit(c)) {
            token_len = 0;
            while (isdigit(state->input[state->position]) && token_len < MAX_TOKEN_LEN - 1) {
                current_token[token_len++] = state->input[state->position++];
                state->column++;
            }
            current_token[token_len] = '\0';
            add_token(state, context, TOKEN_NUMBER, current_token);
            continue;
        }
        
        if (isalpha(c) || c == '_') {
            token_len = 0;
            while ((isalnum(state->input[state->position]) || state->input[state->position] == '_') && 
                   token_len < MAX_TOKEN_LEN - 1) {
                current_token[token_len++] = state->input[state->position++];
                state->column++;
            }
            current_token[token_len] = '\0';
            if (is_keyword(current_token)) {
                add_token(state, context, TOKEN_KEYWORD, current_token);
            } else {
                add_token(state, context, TOKEN_IDENTIFIER, current_token);
            }
            continue;
        }
        
        if (is_operator_char(c)) {
            token_len = 0;
            while (is_operator_char(state->input[state->position]) && token_len < MAX_TOKEN_LEN - 1) {
                current_token[token_len++] = state->input[state->position++];
                state->column++;
            }
            current_token[token_len] = '\0';
            add_token(state, context, TOKEN_OPERATOR, current_token);
            continue;
        }
        
        if (is_delimiter_char(c)) {
            current_token[0] = c;
            current_token[1] = '\0';
            add_token(state, context, TOKEN_DELIMITER, current_token);
            state->position++;
            state->column++;
            continue;
        }
        
        current_token[0] = c;
        current_token[1] = '\0';