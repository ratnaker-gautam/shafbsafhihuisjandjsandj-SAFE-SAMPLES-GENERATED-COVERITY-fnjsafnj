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
    TOKEN_EOF
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

int is_operator(char c) {
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
    
    if (!isalpha(tokenizer->input[tokenizer->position]) && tokenizer->input[tokenizer->position] != '_') {
        return 0;
    }
    
    while (isalnum(tokenizer->input[tokenizer->position]) || tokenizer->input[tokenizer->position] == '_') {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        token->value[len++] = tokenizer->input[tokenizer->position++];
        tokenizer->column++;
    }
    
    token->value[len] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int read_string(struct Tokenizer *tokenizer, struct Token *token) {
    int len = 0;
    
    if (tokenizer->input[tokenizer->position] != '"') return 0;
    
    tokenizer->position++;
    tokenizer->column++;
    
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

int read_operator(struct Tokenizer *tokenizer, struct Token *token) {
    if (!is_operator(tokenizer->input[tokenizer->position])) return 0;
    
    token->value[0] = tokenizer->input[tokenizer->position++];
    token->value[1] = '\0';
    tokenizer->column++;
    token->type = TOKEN_OPERATOR;
    return 1;
}

int get_next_token(struct Tokenizer *tokenizer, struct Token *token) {
    skip_whitespace(tokenizer);
    
    token->line = tokenizer->line;
    token->column = tokenizer->column;
    
    if (tokenizer->input[tokenizer->position] == '\0') {
        token->type = TOKEN_EOF;
        strcpy(token->value, "EOF");
        return 1;
    }
    
    if (isdigit(tokenizer->input[tokenizer->position])) {
        return read_number(tokenizer, token);
    }
    
    if (isalpha(tokenizer->input[tokenizer->position]) || tokenizer->input[tokenizer->position] == '_') {
        return read_identifier(tokenizer, token);
    }
    
    if (tokenizer->input[tokenizer->position] == '"') {
        return read_string(tokenizer, token);
    }
    
    if (is_operator(tokenizer->input[tokenizer->position])) {
        return read_operator(tokenizer, token);
    }
    
    return 0;
}

int tokenize(struct Tokenizer *tokenizer) {
    struct Token token;
    int result;
    
    while (tokenizer->token_count < MAX_TOKENS - 1) {
        result = get_next_token(tokenizer, &token);
        if