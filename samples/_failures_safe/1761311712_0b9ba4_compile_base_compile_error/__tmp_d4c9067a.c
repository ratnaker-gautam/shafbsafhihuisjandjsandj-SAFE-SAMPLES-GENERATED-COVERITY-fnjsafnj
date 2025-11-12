//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

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
    uint32_t line;
    uint32_t column;
};

struct Tokenizer {
    char *input;
    uint32_t position;
    uint32_t line;
    uint32_t column;
    struct Token tokens[MAX_TOKENS];
    uint32_t token_count;
};

void tokenizer_init(struct Tokenizer *tokenizer, char *input) {
    if (tokenizer == NULL || input == NULL) {
        return;
    }
    tokenizer->input = input;
    tokenizer->position = 0;
    tokenizer->line = 1;
    tokenizer->column = 1;
    tokenizer->token_count = 0;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void add_token(struct Tokenizer *tokenizer, enum TokenType type, char *value) {
    if (tokenizer == NULL || value == NULL || tokenizer->token_count >= MAX_TOKENS) {
        return;
    }
    struct Token *token = &tokenizer->tokens[tokenizer->token_count];
    token->type = type;
    size_t len = strlen(value);
    if (len >= MAX_TOKEN_LEN) {
        len = MAX_TOKEN_LEN - 1;
    }
    strncpy(token->value, value, len);
    token->value[len] = '\0';
    token->line = tokenizer->line;
    token->column = tokenizer->column;
    tokenizer->token_count++;
}

void skip_whitespace(struct Tokenizer *tokenizer) {
    if (tokenizer == NULL || tokenizer->input == NULL) {
        return;
    }
    while (tokenizer->input[tokenizer->position] != '\0' && isspace(tokenizer->input[tokenizer->position])) {
        if (tokenizer->input[tokenizer->position] == '\n') {
            tokenizer->line++;
            tokenizer->column = 1;
        } else {
            tokenizer->column++;
        }
        tokenizer->position++;
    }
}

void read_identifier(struct Tokenizer *tokenizer) {
    if (tokenizer == NULL || tokenizer->input == NULL) {
        return;
    }
    char buffer[MAX_TOKEN_LEN];
    uint32_t i = 0;
    while (tokenizer->input[tokenizer->position] != '\0' && (isalnum(tokenizer->input[tokenizer->position]) || tokenizer->input[tokenizer->position] == '_')) {
        if (i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = tokenizer->input[tokenizer->position];
        }
        tokenizer->position++;
        tokenizer->column++;
    }
    buffer[i] = '\0';
    add_token(tokenizer, TOKEN_IDENTIFIER, buffer);
}

void read_number(struct Tokenizer *tokenizer) {
    if (tokenizer == NULL || tokenizer->input == NULL) {
        return;
    }
    char buffer[MAX_TOKEN_LEN];
    uint32_t i = 0;
    while (tokenizer->input[tokenizer->position] != '\0' && isdigit(tokenizer->input[tokenizer->position])) {
        if (i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = tokenizer->input[tokenizer->position];
        }
        tokenizer->position++;
        tokenizer->column++;
    }
    buffer[i] = '\0';
    add_token(tokenizer, TOKEN_NUMBER, buffer);
}

void read_string(struct Tokenizer *tokenizer) {
    if (tokenizer == NULL || tokenizer->input == NULL || tokenizer->input[tokenizer->position] != '"') {
        return;
    }
    tokenizer->position++;
    tokenizer->column++;
    char buffer[MAX_TOKEN_LEN];
    uint32_t i = 0;
    while (tokenizer->input[tokenizer->position] != '\0' && tokenizer->input[tokenizer->position] != '"') {
        if (i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = tokenizer->input[tokenizer->position];
        }
        tokenizer->position++;
        tokenizer->column++;
    }
    if (tokenizer->input[tokenizer->position] == '"') {
        tokenizer->position++;
        tokenizer->column++;
    }
    buffer[i] = '\0';
    add_token(tokenizer, TOKEN_STRING, buffer);
}

void tokenize(struct Tokenizer *tokenizer) {
    if (tokenizer == NULL || tokenizer->input == NULL) {
        return;
    }
    while (tokenizer->input[tokenizer->position] != '\0' && tokenizer->token_count < MAX_TOKENS) {
        skip_whitespace(tokenizer);
        if (tokenizer->input[tokenizer->position] == '\0') {
            break;
        }
        char current = tokenizer->input[tokenizer->position];
        if (isalpha(current) ||