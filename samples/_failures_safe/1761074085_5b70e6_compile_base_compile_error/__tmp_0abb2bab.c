//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: parser_tokenizer
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

int parse_number(struct Tokenizer *tokenizer, struct Token *token) {
    int start = tokenizer->position;
    int has_dot = 0;
    
    while (tokenizer->input[tokenizer->position] != '\0') {
        char c = tokenizer->input[tokenizer->position];
        if (isdigit(c)) {
            tokenizer->position++;
            tokenizer->column++;
        } else if (c == '.' && !has_dot) {
            has_dot = 1;
            tokenizer->position++;
            tokenizer->column++;
        } else {
            break;
        }
    }
    
    int length = tokenizer->position - start;
    if (length > 0 && length < MAX_TOKEN_LEN) {
        strncpy(token->value, &tokenizer->input[start], length);
        token->value[length] = '\0';
        token->type = TOKEN_NUMBER;
        return 1;
    }
    return 0;
}

int parse_identifier(struct Tokenizer *tokenizer, struct Token *token) {
    int start = tokenizer->position;
    
    if (!is_valid_identifier_start(tokenizer->input[tokenizer->position])) {
        return 0;
    }
    
    tokenizer->position++;
    tokenizer->column++;
    
    while (tokenizer->input[tokenizer->position] != '\0') {
        char c = tokenizer->input[tokenizer->position];
        if (!is_valid_identifier_char(c)) {
            break;
        }
        tokenizer->position++;
        tokenizer->column++;
    }
    
    int length = tokenizer->position - start;
    if (length < MAX_TOKEN_LEN) {
        strncpy(token->value, &tokenizer->input[start], length);
        token->value[length] = '\0';
        token->type = TOKEN_IDENTIFIER;
        return 1;
    }
    return 0;
}

int parse_operator(struct Tokenizer *tokenizer, struct Token *token) {
    if (is_operator_char(tokenizer->input[tokenizer->position])) {
        token->value[0] = tokenizer->input[tokenizer->position];
        token->value[1] = '\0';
        token->type = TOKEN_OPERATOR;
        tokenizer->position++;
        tokenizer->column++;
        return 1;
    }
    return 0;
}

int parse_string(struct Tokenizer *tokenizer, struct Token *token) {
    if (tokenizer->input[tokenizer->position] != '"') {
        return 0;
    }
    
    int start = tokenizer->position;
    tokenizer->position++;
    tokenizer->column++;
    
    while (tokenizer->input[tokenizer->position] != '\0') {
        if (tokenizer->input[tokenizer->position] == '"') {
            break;
        }
        if (tokenizer->input[tokenizer->position] == '\n') {
            return 0;
        }
        tokenizer->position++;
        tokenizer->column++;
    }
    
    if (tokenizer->input[tokenizer->position] != '"') {
        return 0;
    }
    
    tokenizer->position++;
    tokenizer->column++;
    
    int length = tokenizer->position - start;
    if (length < MAX_TOKEN_LEN) {
        strncpy(token->value, &tokenizer->input[start], length);
        token->value[length] = '\0';
        token->type = TOKEN_STRING;
        return 1;
    }
    return 0;
}

int get_next_token(struct Tokenizer *tokenizer, struct Token *token) {
    skip_