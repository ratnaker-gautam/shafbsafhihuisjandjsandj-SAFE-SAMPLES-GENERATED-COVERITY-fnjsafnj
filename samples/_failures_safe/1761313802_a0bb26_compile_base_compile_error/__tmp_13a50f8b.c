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
    TOKEN_UNKNOWN
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

struct ParserState {
    struct Token *tokens;
    size_t token_count;
    size_t current_token;
};

struct ASTNode {
    char *type;
    char *value;
    struct ASTNode *left;
    struct ASTNode *right;
};

int is_keyword(const char *str) {
    static const char *keywords[] = {"if", "else", "while", "for", "return", "int", "float", "char", "void", NULL};
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

struct Token create_token(enum TokenType type, const char *value, size_t line, size_t column) {
    struct Token token;
    token.type = type;
    token.line = line;
    token.column = column;
    token.value = malloc(strlen(value) + 1);
    if (token.value != NULL) {
        strcpy(token.value, value);
    }
    return token;
}

void free_token(struct Token *token) {
    if (token != NULL && token->value != NULL) {
        free(token->value);
    }
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

struct LexerState init_lexer(const char *input) {
    struct LexerState state;
    state.input = input;
    state.position = 0;
    state.line = 1;
    state.column = 1;
    state.input_length = strlen(input);
    return state;
}

char lexer_peek(struct LexerState *state) {
    if (state->position >= state->input_length) {
        return '\0';
    }
    return state->input[state->position];
}

char lexer_advance(struct LexerState *state) {
    if (state->position >= state->input_length) {
        return '\0';
    }
    char c = state->input[state->position++];
    if (c == '\n') {
        state->line++;
        state->column = 1;
    } else {
        state->column++;
    }
    return c;
}

void lexer_skip_whitespace(struct LexerState *state) {
    while (state->position < state->input_length && isspace(lexer_peek(state))) {
        lexer_advance(state);
    }
}

struct Token lexer_next_token(struct LexerState *state) {
    lexer_skip_whitespace(state);
    size_t start_line = state->line;
    size_t start_column = state->column;
    
    if (state->position >= state->input_length) {
        return create_token(TOKEN_EOF, "", start_line, start_column);
    }
    
    char c = lexer_peek(state);
    
    if (isalpha(c) || c == '_') {
        char buffer[256];
        size_t i = 0;
        while ((state->position < state->input_length) && 
               (isalnum(lexer_peek(state)) || lexer_peek(state) == '_') && i < 255) {
            buffer[i++] = lexer_advance(state);
        }
        buffer[i] = '\0';
        
        if (is_keyword(buffer)) {
            return create_token(TOKEN_KEYWORD, buffer, start_line, start_column);
        } else {
            return create_token(TOKEN_IDENTIFIER, buffer, start_line, start_column);
        }
    }
    
    if (isdigit(c)) {
        char buffer[256];
        size_t i = 0;
        while ((state->position < state->input_length) && 
               (isdigit(lexer_peek(state)) || lexer_peek(state) == '.') && i < 255) {
            buffer[i++] = lexer_advance(state);
        }
        buffer[i] = '\0';
        return create_token(TOKEN_NUMBER, buffer, start_line, start_column);
    }
    
    if (c == '"') {
        char buffer[256];
        size_t i = 0;
        lexer_advance(state);
        while ((state->position < state->input_length) && 
               (lexer_peek(state) != '"') && i < 255) {
            buffer[i++] = lexer_advance(state);
        }
        if (lexer_peek(state) == '"') {
            lexer_advance(state);
        }
        buffer[i] = '\0';
        return create_token(TOKEN_STRING, buffer, start_line, start_column);
    }
    
    if (is_operator_char(c)) {
        char buffer[3] = {0};
        buffer[0] = lexer_advance(state);
        if (is_operator_char