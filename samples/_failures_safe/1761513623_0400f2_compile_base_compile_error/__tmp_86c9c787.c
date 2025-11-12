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
        if (len >= MAX_TOKEN_LEN - 1) {
            return 0;
        }
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
        if (len >= MAX_TOKEN_LEN - 1) {
            return 0;
        }
        token->value[len++] = tokenizer->input[tokenizer->position++];
        tokenizer->column++;
    }
    token->value[len] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int read_string(struct Tokenizer *tokenizer, struct Token *token) {
    int len = 0;
    tokenizer->position++;
    tokenizer->column++;
    
    while (tokenizer->input[tokenizer->position] != '\0' && tokenizer->input[tokenizer->position] != '"') {
        if (len >= MAX_TOKEN_LEN - 1) {
            return 0;
        }
        token->value[len++] = tokenizer->input[tokenizer->position++];
        tokenizer->column++;
    }
    
    if (tokenizer->input[tokenizer->position] == '"') {
        tokenizer->position++;
        tokenizer->column++;
        token->value[len] = '\0';
        token->type = TOKEN_STRING;
        return 1;
    }
    return 0;
}

int read_operator(struct Tokenizer *tokenizer, struct Token *token) {
    int len = 0;
    while (is_operator_char(tokenizer->input[tokenizer->position])) {
        if (len >= MAX_TOKEN_LEN - 1) {
            return 0;
        }
        token->value[len++] = tokenizer->input[tokenizer->position++];
        tokenizer->column++;
    }
    token->value[len] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

int get_next_token(struct Tokenizer *tokenizer, struct Token *token) {
    skip_whitespace(tokenizer);
    
    if (tokenizer->input[tokenizer->position] == '\0') {
        token->type = TOKEN_EOF;
        strcpy(token->value, "EOF");
        token->line = tokenizer->line;
        token->column = tokenizer->column;
        return 1;
    }
    
    token->line = tokenizer->line;
    token->column = tokenizer->column;
    
    char c = tokenizer->input[tokenizer->position];
    
    if (isdigit(c)) {
        return read_number(tokenizer, token);
    } else if (isalpha(c) || c == '_') {
        return read_identifier(tokenizer, token);
    } else if (c == '"') {
        return read_string(tokenizer, token);
    } else if (is_operator_char(c)) {
        return read_operator(tokenizer, token);
    } else {
        token->type = TOKEN_ERROR;
        token->value[0] = c;
        token->value[1] = '\0';
        tokenizer->position++;
        tokenizer->column++;
        return 1;
    }
}

int tokenize(struct Tokenizer *tokenizer) {
    struct Token token;
    
    while (tokenizer->token_count < MAX_TOKENS) {
        if (!