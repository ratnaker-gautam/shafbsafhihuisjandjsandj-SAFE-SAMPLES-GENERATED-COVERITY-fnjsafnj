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
    TOKEN_UNKNOWN
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

void initialize_tokenizer(struct Tokenizer *tokenizer, const char *input) {
    tokenizer->input = input;
    tokenizer->position = 0;
    tokenizer->line = 1;
    tokenizer->column = 1;
    tokenizer->token_count = 0;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void add_token(struct Tokenizer *tokenizer, enum TokenType type, const char *value) {
    if (tokenizer->token_count >= MAX_TOKENS) {
        return;
    }
    struct Token *token = &tokenizer->tokens[tokenizer->token_count];
    token->type = type;
    token->line = tokenizer->line;
    token->column = tokenizer->column;
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
    tokenizer->token_count++;
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

int parse_number(struct Tokenizer *tokenizer) {
    int start = tokenizer->position;
    while (isdigit(tokenizer->input[tokenizer->position])) {
        tokenizer->position++;
        tokenizer->column++;
    }
    int length = tokenizer->position - start;
    if (length > 0) {
        char number[MAX_TOKEN_LEN];
        if (length >= MAX_TOKEN_LEN) {
            length = MAX_TOKEN_LEN - 1;
        }
        strncpy(number, &tokenizer->input[start], length);
        number[length] = '\0';
        add_token(tokenizer, TOKEN_NUMBER, number);
        return 1;
    }
    return 0;
}

int parse_identifier(struct Tokenizer *tokenizer) {
    int start = tokenizer->position;
    if (isalpha(tokenizer->input[tokenizer->position]) || tokenizer->input[tokenizer->position] == '_') {
        tokenizer->position++;
        tokenizer->column++;
        while (isalnum(tokenizer->input[tokenizer->position]) || tokenizer->input[tokenizer->position] == '_') {
            tokenizer->position++;
            tokenizer->column++;
        }
        int length = tokenizer->position - start;
        if (length >= MAX_TOKEN_LEN) {
            length = MAX_TOKEN_LEN - 1;
        }
        char identifier[MAX_TOKEN_LEN];
        strncpy(identifier, &tokenizer->input[start], length);
        identifier[length] = '\0';
        add_token(tokenizer, TOKEN_IDENTIFIER, identifier);
        return 1;
    }
    return 0;
}

int parse_operator(struct Tokenizer *tokenizer) {
    if (is_operator_char(tokenizer->input[tokenizer->position])) {
        char op[2] = {tokenizer->input[tokenizer->position], '\0'};
        tokenizer->position++;
        tokenizer->column++;
        add_token(tokenizer, TOKEN_OPERATOR, op);
        return 1;
    }
    return 0;
}

int parse_string(struct Tokenizer *tokenizer) {
    if (tokenizer->input[tokenizer->position] == '"') {
        int start = tokenizer->position;
        tokenizer->position++;
        tokenizer->column++;
        while (tokenizer->input[tokenizer->position] != '\0' && tokenizer->input[tokenizer->position] != '"') {
            if (tokenizer->input[tokenizer->position] == '\n') {
                tokenizer->line++;
                tokenizer->column = 1;
            } else {
                tokenizer->column++;
            }
            tokenizer->position++;
        }
        if (tokenizer->input[tokenizer->position] == '"') {
            int length = tokenizer->position - start - 1;
            if (length >= MAX_TOKEN_LEN) {
                length = MAX_TOKEN_LEN - 1;
            }
            char string[MAX_TOKEN_LEN];
            strncpy(string, &tokenizer->input[start + 1], length);
            string[length] = '\0';
            add_token(tokenizer, TOKEN_STRING, string);
            tokenizer->position++;
            tokenizer->column++;
            return 1;
        }
    }
    return