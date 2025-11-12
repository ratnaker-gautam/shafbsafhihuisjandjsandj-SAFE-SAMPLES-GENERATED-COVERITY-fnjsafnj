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
    const char *keywords[] = {"if", "else", "while", "for", "return", "int", "float", "char", "void"};
    int num_keywords = sizeof(keywords) / sizeof(keywords[0]);
    
    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int is_delimiter(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == ';' || c == ',' || c == '[';
}

void init_tokenizer(struct TokenizerState *state, char *input) {
    state->input = input;
    state->position = 0;
    state->line = 1;
    state->column = 1;
    state->token_count = 0;
}

void skip_whitespace(struct TokenizerState *state) {
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
    if (state->token_count >= MAX_TOKENS) {
        return 0;
    }
    
    skip_whitespace(state);
    
    if (state->input[state->position] == '\0') {
        return 0;
    }
    
    token->line = state->line;
    token->column = state->column;
    
    char current = state->input[state->position];
    
    if (isalpha(current) || current == '_') {
        int len = 0;
        while ((isalnum(state->input[state->position]) || state->input[state->position] == '_') && len < MAX_TOKEN_LEN - 1) {
            token->value[len++] = state->input[state->position++];
            state->column++;
        }
        token->value[len] = '\0';
        
        if (is_keyword(token->value)) {
            token->type = TOKEN_KEYWORD;
        } else {
            token->type = TOKEN_IDENTIFIER;
        }
    }
    else if (isdigit(current)) {
        int len = 0;
        while (isdigit(state->input[state->position]) && len < MAX_TOKEN_LEN - 1) {
            token->value[len++] = state->input[state->position++];
            state->column++;
        }
        token->value[len] = '\0';
        token->type = TOKEN_NUMBER;
    }
    else if (is_operator(current)) {
        token->value[0] = state->input[state->position++];
        token->value[1] = '\0';
        state->column++;
        token->type = TOKEN_OPERATOR;
    }
    else if (is_delimiter(current)) {
        token->value[0] = state->input[state->position++];
        token->value[1] = '\0';
        state->column++;
        token->type = TOKEN_DELIMITER;
    }
    else {
        token->value[0] = state->input[state->position++];
        token->value[1] = '\0';
        state->column++;
        token->type = TOKEN_UNKNOWN;
    }
    
    state->token_count++;
    return 1;
}

void init_parser(struct ParserState *parser, struct Token *tokens, int count) {
    parser->tokens = tokens;
    parser->current_token = 0;
    parser->token_count = count;
}

struct Token *peek_token(struct ParserState *parser) {
    if (parser->current_token < parser->token_count) {
        return &parser->tokens[parser->current_token];
    }
    return NULL;
}

struct Token *consume_token(struct ParserState *parser) {
    if (parser->current_token < parser->token_count) {
        return &parser->tokens[parser->current_token++];
    }
    return NULL;
}

void parse_expression(struct ParserState *parser) {
    struct Token *token = consume_token(parser);
    if (!token) return;
    
    if (token->type == TOKEN_NUMBER || token->type == TOKEN_IDENT