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
    TOKEN_PUNCTUATION,
    TOKEN_KEYWORD,
    TOKEN_EOF,
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char *value;
    size_t line;
    size_t column;
};

struct LexerState {
    const char *input;
    size_t position;
    size_t line;
    size_t column;
    size_t input_length;
};

struct ParserNode {
    enum TokenType node_type;
    char *value;
    struct ParserNode *left;
    struct ParserNode *right;
};

struct Tokenizer {
    struct LexerState state;
    struct Token *tokens;
    size_t token_count;
    size_t token_capacity;
};

void init_lexer_state(struct LexerState *state, const char *input) {
    state->input = input;
    state->position = 0;
    state->line = 1;
    state->column = 1;
    state->input_length = strlen(input);
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int is_punctuation_char(char c) {
    return strchr("(),;{}[]", c) != NULL;
}

int is_keyword(const char *str) {
    const char *keywords[] = {"if", "else", "while", "for", "return", "int", "char", "void"};
    size_t num_keywords = sizeof(keywords) / sizeof(keywords[0]);
    for (size_t i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void add_token(struct Tokenizer *tokenizer, enum TokenType type, const char *value, size_t line, size_t column) {
    if (tokenizer->token_count >= tokenizer->token_capacity) {
        size_t new_capacity = tokenizer->token_capacity * 2;
        if (new_capacity == 0) new_capacity = 16;
        struct Token *new_tokens = realloc(tokenizer->tokens, new_capacity * sizeof(struct Token));
        if (!new_tokens) return;
        tokenizer->tokens = new_tokens;
        tokenizer->token_capacity = new_capacity;
    }
    
    struct Token *token = &tokenizer->tokens[tokenizer->token_count];
    token->type = type;
    token->line = line;
    token->column = column;
    
    if (value) {
        token->value = malloc(strlen(value) + 1);
        if (token->value) {
            strcpy(token->value, value);
        }
    } else {
        token->value = NULL;
    }
    
    tokenizer->token_count++;
}

void skip_whitespace(struct LexerState *state) {
    while (state->position < state->input_length && isspace(state->input[state->position])) {
        if (state->input[state->position] == '\n') {
            state->line++;
            state->column = 1;
        } else {
            state->column++;
        }
        state->position++;
    }
}

void tokenize_identifier(struct Tokenizer *tokenizer) {
    struct LexerState *state = &tokenizer->state;
    size_t start = state->position;
    
    while (state->position < state->input_length && 
           (isalnum(state->input[state->position]) || state->input[state->position] == '_')) {
        state->position++;
        state->column++;
    }
    
    size_t length = state->position - start;
    if (length > 0) {
        char *ident = malloc(length + 1);
        if (ident) {
            strncpy(ident, state->input + start, length);
            ident[length] = '\0';
            
            enum TokenType type = is_keyword(ident) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
            add_token(tokenizer, type, ident, state->line, state->column - length);
            free(ident);
        }
    }
}

void tokenize_number(struct Tokenizer *tokenizer) {
    struct LexerState *state = &tokenizer->state;
    size_t start = state->position;
    
    while (state->position < state->input_length && isdigit(state->input[state->position])) {
        state->position++;
        state->column++;
    }
    
    size_t length = state->position - start;
    if (length > 0) {
        char *num = malloc(length + 1);
        if (num) {
            strncpy(num, state->input + start, length);
            num[length] = '\0';
            add_token(tokenizer, TOKEN_NUMBER, num, state->line, state->column - length);
            free(num);
        }
    }
}

void tokenize_string(struct Tokenizer *tokenizer) {
    struct LexerState *state = &tokenizer->state;
    size_t start = state->position;
    state->position++;
    state->column++;
    
    while (state->position < state->input_length && state->input[state->position] != '"') {
        if (