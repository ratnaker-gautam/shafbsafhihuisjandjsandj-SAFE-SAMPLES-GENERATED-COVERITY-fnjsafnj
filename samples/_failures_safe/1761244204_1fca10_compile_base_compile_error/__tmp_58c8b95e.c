//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Token {
    char *value;
    int type;
    int line;
    int column;
};

struct TokenList {
    struct Token *tokens;
    size_t count;
    size_t capacity;
};

enum TokenType {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_UNKNOWN
};

struct ParserState {
    const char *input;
    size_t position;
    size_t length;
    int line;
    int column;
};

void initTokenList(struct TokenList *list) {
    list->tokens = malloc(sizeof(struct Token) * 16);
    if (list->tokens == NULL) {
        exit(EXIT_FAILURE);
    }
    list->count = 0;
    list->capacity = 16;
}

void freeTokenList(struct TokenList *list) {
    for (size_t i = 0; i < list->count; i++) {
        free(list->tokens[i].value);
    }
    free(list->tokens);
}

void addToken(struct TokenList *list, const char *value, int type, int line, int column) {
    if (list->count >= list->capacity) {
        size_t new_capacity = list->capacity * 2;
        struct Token *new_tokens = realloc(list->tokens, sizeof(struct Token) * new_capacity);
        if (new_tokens == NULL) {
            exit(EXIT_FAILURE);
        }
        list->tokens = new_tokens;
        list->capacity = new_capacity;
    }
    
    list->tokens[list->count].value = malloc(strlen(value) + 1);
    if (list->tokens[list->count].value == NULL) {
        exit(EXIT_FAILURE);
    }
    strcpy(list->tokens[list->count].value, value);
    list->tokens[list->count].type = type;
    list->tokens[list->count].line = line;
    list->tokens[list->count].column = column;
    list->count++;
}

int isOperatorChar(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void initParserState(struct ParserState *state, const char *input) {
    state->input = input;
    state->position = 0;
    state->length = strlen(input);
    state->line = 1;
    state->column = 1;
}

char peekChar(struct ParserState *state) {
    if (state->position >= state->length) {
        return '\0';
    }
    return state->input[state->position];
}

char nextChar(struct ParserState *state) {
    if (state->position >= state->length) {
        return '\0';
    }
    char c = state->input[state->position];
    state->position++;
    if (c == '\n') {
        state->line++;
        state->column = 1;
    } else {
        state->column++;
    }
    return c;
}

void skipWhitespace(struct ParserState *state) {
    while (state->position < state->length && isspace(peekChar(state))) {
        nextChar(state);
    }
}

void tokenize(struct ParserState *state, struct TokenList *tokens) {
    while (state->position < state->length) {
        skipWhitespace(state);
        char current = peekChar(state);
        if (current == '\0') break;
        
        int line = state->line;
        int column = state->column;
        
        if (isalpha(current) || current == '_') {
            char buffer[256];
            size_t i = 0;
            while ((state->position < state->length) && 
                   (isalnum(peekChar(state)) || peekChar(state) == '_') && 
                   i < sizeof(buffer) - 1) {
                buffer[i++] = nextChar(state);
            }
            buffer[i] = '\0';
            addToken(tokens, buffer, TOKEN_IDENTIFIER, line, column);
        }
        else if (isdigit(current)) {
            char buffer[256];
            size_t i = 0;
            while ((state->position < state->length) && 
                   (isdigit(peekChar(state)) || peekChar(state) == '.') && 
                   i < sizeof(buffer) - 1) {
                buffer[i++] = nextChar(state);
            }
            buffer[i] = '\0';
            addToken(tokens, buffer, TOKEN_NUMBER, line, column);
        }
        else if (current == '"') {
            nextChar(state);
            char buffer[256];
            size_t i = 0;
            while ((state->position < state->length) && 
                   (peekChar(state) != '"') && 
                   i < sizeof(buffer) - 1) {
                buffer[i++] = nextChar(state);
            }
            buffer[i] = '\0';
            if (peekChar(state) == '"') {
                nextChar(state);
            }
            addToken(tokens, buffer, TOKEN_STRING, line, column);
        }
        else if (isOperatorChar(current)) {
            char buffer[3] = {0};
            buffer[0] = nextChar(state);
            if (state->position < state->length && isOperatorChar(