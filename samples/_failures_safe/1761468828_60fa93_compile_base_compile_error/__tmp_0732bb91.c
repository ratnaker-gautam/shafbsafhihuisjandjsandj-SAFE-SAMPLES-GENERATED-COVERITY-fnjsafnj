//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_INVALID
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

int is_valid_identifier_start(char c) {
    return isalpha(c) || c == '_';
}

int is_valid_identifier_char(char c) {
    return isalnum(c) || c == '_';
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

int add_token(struct Tokenizer *tokenizer, enum TokenType type, const char *value) {
    if (tokenizer->token_count >= MAX_TOKENS) {
        return 0;
    }
    struct Token *token = &tokenizer->tokens[tokenizer->token_count];
    token->type = type;
    token->line = tokenizer->line;
    token->column = tokenizer->column;
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
    tokenizer->token_count++;
    return 1;
}

int parse_number(struct Tokenizer *tokenizer) {
    char buffer[MAX_TOKEN_LEN] = {0};
    int buf_pos = 0;
    int has_dot = 0;
    
    while (tokenizer->input[tokenizer->position] != '\0' && buf_pos < MAX_TOKEN_LEN - 1) {
        char c = tokenizer->input[tokenizer->position];
        if (isdigit(c)) {
            buffer[buf_pos++] = c;
            tokenizer->position++;
            tokenizer->column++;
        } else if (c == '.' && !has_dot) {
            buffer[buf_pos++] = c;
            tokenizer->position++;
            tokenizer->column++;
            has_dot = 1;
        } else {
            break;
        }
    }
    
    if (buf_pos > 0) {
        buffer[buf_pos] = '\0';
        return add_token(tokenizer, TOKEN_NUMBER, buffer);
    }
    return 0;
}

int parse_identifier(struct Tokenizer *tokenizer) {
    char buffer[MAX_TOKEN_LEN] = {0};
    int buf_pos = 0;
    
    while (tokenizer->input[tokenizer->position] != '\0' && buf_pos < MAX_TOKEN_LEN - 1) {
        char c = tokenizer->input[tokenizer->position];
        if (is_valid_identifier_char(c)) {
            buffer[buf_pos++] = c;
            tokenizer->position++;
            tokenizer->column++;
        } else {
            break;
        }
    }
    
    if (buf_pos > 0) {
        buffer[buf_pos] = '\0';
        return add_token(tokenizer, TOKEN_IDENTIFIER, buffer);
    }
    return 0;
}

int parse_operator(struct Tokenizer *tokenizer) {
    char buffer[3] = {0};
    int buf_pos = 0;
    
    while (tokenizer->input[tokenizer->position] != '\0' && buf_pos < 2) {
        char c = tokenizer->input[tokenizer->position];
        if (is_operator_char(c)) {
            buffer[buf_pos++] = c;
            tokenizer->position++;
            tokenizer->column++;
        } else {
            break;
        }
    }
    
    if (buf_pos > 0) {
        buffer[buf_pos] = '\0';
        return add_token(tokenizer, TOKEN_OPERATOR, buffer);
    }
    return 0;
}

int parse_string(struct Tokenizer *tokenizer) {
    if (tokenizer->input[tokenizer->position] != '"') {
        return 0;
    }
    
    char buffer[MAX_TOKEN_LEN] = {0};
    int buf_pos = 0;
    tokenizer->position++;
    tokenizer->column++;
    
    while (tokenizer->input[tokenizer->position] != '\0' && buf_pos < MAX_TOKEN_LEN - 1) {
        char c = tokenizer->input[tokenizer->position];
        if (c == '"') {