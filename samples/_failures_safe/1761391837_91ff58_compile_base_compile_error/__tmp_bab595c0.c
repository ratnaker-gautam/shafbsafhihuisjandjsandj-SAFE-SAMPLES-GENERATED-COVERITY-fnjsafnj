//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

enum TokenType {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
    TOKEN_KEYWORD,
    TOKEN_EOF,
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char value[64];
    uint32_t line;
    uint32_t column;
};

struct LexerState {
    const char *input;
    uint32_t position;
    uint32_t line;
    uint32_t column;
    uint32_t input_length;
};

struct ParserNode {
    enum TokenType node_type;
    char value[64];
    struct ParserNode *left;
    struct ParserNode *right;
};

struct ParseResult {
    int success;
    struct ParserNode *ast;
    char error_msg[128];
};

static const char *keywords[] = {
    "if", "else", "while", "for", "return", "int", "float", "char", "void"
};
static const size_t num_keywords = sizeof(keywords) / sizeof(keywords[0]);

void lexer_init(struct LexerState *state, const char *input) {
    if (state == NULL || input == NULL) return;
    state->input = input;
    state->position = 0;
    state->line = 1;
    state->column = 1;
    state->input_length = strlen(input);
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int is_delimiter(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || 
           c == '[' || c == ']' || c == ';' || c == ',' || c == ':';
}

int is_keyword(const char *str) {
    for (size_t i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

struct Token lexer_next_token(struct LexerState *state) {
    struct Token token = {TOKEN_INVALID, "", state->line, state->column};
    
    if (state->position >= state->input_length) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char current = state->input[state->position];
    
    while (isspace(current)) {
        if (current == '\n') {
            state->line++;
            state->column = 1;
        } else {
            state->column++;
        }
        state->position++;
        if (state->position >= state->input_length) {
            token.type = TOKEN_EOF;
            return token;
        }
        current = state->input[state->position];
    }
    
    token.line = state->line;
    token.column = state->column;
    
    if (isalpha(current) || current == '_') {
        size_t i = 0;
        while ((state->position < state->input_length) && 
               (isalnum(state->input[state->position]) || state->input[state->position] == '_') &&
               i < sizeof(token.value) - 1) {
            token.value[i++] = state->input[state->position++];
            state->column++;
        }
        token.value[i] = '\0';
        token.type = is_keyword(token.value) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
    }
    else if (isdigit(current)) {
        size_t i = 0;
        while ((state->position < state->input_length) && 
               (isdigit(state->input[state->position]) || state->input[state->position] == '.') &&
               i < sizeof(token.value) - 1) {
            token.value[i++] = state->input[state->position++];
            state->column++;
        }
        token.value[i] = '\0';
        token.type = TOKEN_NUMBER;
    }
    else if (current == '"') {
        size_t i = 0;
        state->position++;
        state->column++;
        while ((state->position < state->input_length) && 
               (state->input[state->position] != '"') &&
               i < sizeof(token.value) - 1) {
            token.value[i++] = state->input[state->position++];
            state->column++;
        }
        token.value[i] = '\0';
        if (state->position < state->input_length && state->input[state->position] == '"') {
            state->position++;
            state->column++;
            token.type = TOKEN_STRING;
        } else {
            token.type = TOKEN_INVALID;
        }
    }
    else if (is_operator_char(current)) {
        size_t i = 0;
        while ((state->position < state->input_length) && 
               is_operator_char(state->input[state->position]) &&
               i < sizeof(token.value) - 1) {
            token.value[i++] = state->input[state->position++];
            state->column++;
        }
        token.value[i] = '\0';
        token.type = TOKEN_OPERATOR;
    }
    else if (is