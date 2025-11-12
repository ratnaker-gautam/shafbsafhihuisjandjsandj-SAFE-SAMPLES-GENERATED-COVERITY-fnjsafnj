//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

enum TokenType {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
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

struct LexerState {
    const char *input;
    int position;
    int line;
    int column;
    int error_count;
};

struct ParserState {
    struct Token *tokens;
    int token_count;
    int current_token;
    int error_count;
};

void lexer_init(struct LexerState *state, const char *input) {
    state->input = input;
    state->position = 0;
    state->line = 1;
    state->column = 1;
    state->error_count = 0;
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int get_token(struct LexerState *state, struct Token *token) {
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
            token->type = TOKEN_IDENTIFIER;
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
            } else {
                state->error_count++;
            }
            token->value[len] = '\0';
            token->type = TOKEN_STRING;
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
        
        if (strchr("(),;{}[]", current) != NULL) {
            token->value[0] = current;
            token->value[1] = '\0';
            token->type = TOKEN_DELIMITER;
            state->position++;
            state->column++;
            return 1;
        }
        
        token->value[0] = current;
        token->value[1] = '\0';
        token->type = TOKEN_UNKNOWN;
        state->position++;
        state->column++;
        state->error_count++;
        return 1;
    }
    
    token->type = TOKEN_EOF;
    token->value[0] = '\0';
    return 0;
}

int tokenize(struct LexerState *lexer, struct Token *tokens, int max_tokens) {
    int count = 0;
    struct Token token;
    
    while (count < max_tokens && get_token(lexer, &token)) {
        if (token.type == TOKEN_EOF) {
            break;
        }
        tokens[count] = token;
        count++;
    }
    
    if (count < max_tokens) {
        tokens[count].type = TOKEN_EOF;
        tokens[count].value[0] = '\0';
        count++;
    }
    
    return count;
}

void parser_init(struct ParserState *parser, struct Token *tokens, int token_count) {
    parser->tokens = tokens;
    parser->token_count = token_count;
    parser->current_token = 0;
    parser->error_count = 0;
}

int parser_accept(struct ParserState *parser, enum TokenType type) {
    if (parser->current_token < parser->token_count && parser->tokens[parser->current