//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_ERROR
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
    int line;
    int column;
};

struct Tokenizer {
    const char *input;
    int position;
    int line;
    int column;
    struct Token tokens[MAX_TOKENS];
    int token_count;
};

void init_tokenizer(struct Tokenizer *tokenizer, const char *input) {
    tokenizer->input = input;
    tokenizer->position = 0;
    tokenizer->line = 1;
    tokenizer->column = 1;
    tokenizer->token_count = 0;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(struct Tokenizer *tokenizer) {
    while (tokenizer->input[tokenizer->position] != '\0') {
        char c = tokenizer->input[tokenizer->position];
        if (c == ' ' || c == '\t') {
            tokenizer->position++;
            tokenizer->column++;
        } else if (c == '\n') {
            tokenizer->position++;
            tokenizer->line++;
            tokenizer->column = 1;
        } else {
            break;
        }
    }
}

int read_number(struct Tokenizer *tokenizer, struct Token *token) {
    int len = 0;
    while (isdigit(tokenizer->input[tokenizer->position])) {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        token->value[len++] = tokenizer->input[tokenizer->position++];
        tokenizer->column++;
    }
    token->value[len] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int read_identifier(struct Tokenizer *tokenizer, struct Token *token) {
    int len = 0;
    while (isalnum(tokenizer->input[tokenizer->position]) || tokenizer->input[tokenizer->position] == '_') {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        token->value[len++] = tokenizer->input[tokenizer->position++];
        tokenizer->column++;
    }
    token->value[len] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int read_operator(struct Tokenizer *tokenizer, struct Token *token) {
    int len = 0;
    while (is_operator_char(tokenizer->input[tokenizer->position])) {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        token->value[len++] = tokenizer->input[tokenizer->position++];
        tokenizer->column++;
    }
    token->value[len] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

int read_string(struct Tokenizer *tokenizer, struct Token *token) {
    if (tokenizer->input[tokenizer->position] != '"') return 0;
    
    tokenizer->position++;
    tokenizer->column++;
    int len = 0;
    
    while (tokenizer->input[tokenizer->position] != '"' && tokenizer->input[tokenizer->position] != '\0') {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        token->value[len++] = tokenizer->input[tokenizer->position++];
        tokenizer->column++;
    }
    
    if (tokenizer->input[tokenizer->position] != '"') return 0;
    
    tokenizer->position++;
    tokenizer->column++;
    token->value[len] = '\0';
    token->type = TOKEN_STRING;
    return 1;
}

int tokenize(struct Tokenizer *tokenizer) {
    while (tokenizer->input[tokenizer->position] != '\0' && tokenizer->token_count < MAX_TOKENS) {
        skip_whitespace(tokenizer);
        
        if (tokenizer->input[tokenizer->position] == '\0') break;
        
        struct Token token;
        token.line = tokenizer->line;
        token.column = tokenizer->column;
        
        char c = tokenizer->input[tokenizer->position];
        
        if (isdigit(c)) {
            if (!read_number(tokenizer, &token)) {
                token.type = TOKEN_ERROR;
                strncpy(token.value, "Number too long", MAX_TOKEN_LEN - 1);
                token.value[MAX_TOKEN_LEN - 1] = '\0';
            }
        } else if (isalpha(c) || c == '_') {
            if (!read_identifier(tokenizer, &token)) {
                token.type = TOKEN_ERROR;
                strncpy(token.value, "Identifier too long", MAX_TOKEN_LEN - 1);
                token.value[MAX_TOKEN_LEN - 1] = '\0';
            }
        } else if (is_operator_char(c)) {
            if (!read_operator(tokenizer, &token)) {
                token.type = TOKEN_ERROR;
                strncpy(token.value, "Operator too long", MAX_TOKEN_LEN -