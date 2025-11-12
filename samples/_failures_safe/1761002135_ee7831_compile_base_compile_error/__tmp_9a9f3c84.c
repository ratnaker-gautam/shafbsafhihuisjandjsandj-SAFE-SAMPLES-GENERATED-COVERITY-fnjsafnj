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

void skip_whitespace(struct Tokenizer *tokenizer) {
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

int parse_number(struct Tokenizer *tokenizer) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    while (isdigit(tokenizer->input[tokenizer->position]) && i < MAX_TOKEN_LEN - 1) {
        buffer[i++] = tokenizer->input[tokenizer->position++];
        tokenizer->column++;
    }
    buffer[i] = '\0';
    return add_token(tokenizer, TOKEN_NUMBER, buffer);
}

int parse_identifier(struct Tokenizer *tokenizer) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    while (is_valid_identifier_char(tokenizer->input[tokenizer->position]) && i < MAX_TOKEN_LEN - 1) {
        buffer[i++] = tokenizer->input[tokenizer->position++];
        tokenizer->column++;
    }
    buffer[i] = '\0';
    return add_token(tokenizer, TOKEN_IDENTIFIER, buffer);
}

int parse_operator(struct Tokenizer *tokenizer) {
    char buffer[3] = {0};
    buffer[0] = tokenizer->input[tokenizer->position++];
    tokenizer->column++;
    if (is_operator_char(tokenizer->input[tokenizer->position])) {
        buffer[1] = tokenizer->input[tokenizer->position++];
        tokenizer->column++;
    }
    return add_token(tokenizer, TOKEN_OPERATOR, buffer);
}

int parse_string(struct Tokenizer *tokenizer) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    tokenizer->position++;
    tokenizer->column++;
    while (tokenizer->input[tokenizer->position] != '\0' && tokenizer->input[tokenizer->position] != '"' && i < MAX_TOKEN_LEN - 1) {
        buffer[i++] = tokenizer->input[tokenizer->position++];
        tokenizer->column++;
    }
    if (tokenizer->input[tokenizer->position] != '"') {
        return 0;
    }
    buffer[i] = '\0';
    tokenizer->position++;
    tokenizer->column++;
    return add_token(tokenizer, TOKEN_STRING, buffer);
}

int tokenize(struct Tokenizer *tokenizer) {
    while (tokenizer->input[tokenizer->position] != '\0' && tokenizer->token_count < MAX_TOKENS) {
        skip_whitespace(tokenizer);
        if (tokenizer->input[tokenizer->position] == '\0') {
            break;
        }
        char current = tokenizer->input[tokenizer->position];
        if (isdigit(current)) {
            if (!parse_number(tokenizer)) {
                return 0;
            }
        } else if (is_valid_identifier_start(current)) {
            if (!parse_identifier(tokenizer)) {
                return 0;
            }
        } else if (is_operator_char(current)) {
            if (!parse_operator(tokenizer)) {
                return 0;
            }
        } else if (current == '"') {
            if (!parse_string(tokenizer)) {
                return 0;
            }
        }