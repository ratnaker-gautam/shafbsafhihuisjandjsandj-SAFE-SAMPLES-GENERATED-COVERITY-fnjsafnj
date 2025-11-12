//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64
#define MAX_INPUT_LEN 1024

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_PAREN,
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
    char *input;
    int position;
    int line;
    int column;
    struct Token tokens[MAX_TOKENS];
    int token_count;
};

void tokenizer_init(struct Tokenizer *tokenizer, char *input) {
    if (tokenizer == NULL || input == NULL) return;
    tokenizer->input = input;
    tokenizer->position = 0;
    tokenizer->line = 1;
    tokenizer->column = 1;
    tokenizer->token_count = 0;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void tokenizer_skip_whitespace(struct Tokenizer *tokenizer) {
    if (tokenizer == NULL) return;
    while (tokenizer->input[tokenizer->position] != '\0' && 
           isspace(tokenizer->input[tokenizer->position])) {
        if (tokenizer->input[tokenizer->position] == '\n') {
            tokenizer->line++;
            tokenizer->column = 1;
        } else {
            tokenizer->column++;
        }
        tokenizer->position++;
    }
}

int tokenizer_read_number(struct Tokenizer *tokenizer, struct Token *token) {
    if (tokenizer == NULL || token == NULL) return 0;
    int start = tokenizer->position;
    int len = 0;
    
    while (isdigit(tokenizer->input[tokenizer->position])) {
        tokenizer->position++;
        tokenizer->column++;
        len++;
        if (len >= MAX_TOKEN_LEN - 1) break;
    }
    
    if (len > 0) {
        strncpy(token->value, &tokenizer->input[start], len);
        token->value[len] = '\0';
        token->type = TOKEN_NUMBER;
        token->line = tokenizer->line;
        token->column = tokenizer->column - len;
        return 1;
    }
    return 0;
}

int tokenizer_read_identifier(struct Tokenizer *tokenizer, struct Token *token) {
    if (tokenizer == NULL || token == NULL) return 0;
    int start = tokenizer->position;
    int len = 0;
    
    while (isalnum(tokenizer->input[tokenizer->position]) || 
           tokenizer->input[tokenizer->position] == '_') {
        tokenizer->position++;
        tokenizer->column++;
        len++;
        if (len >= MAX_TOKEN_LEN - 1) break;
    }
    
    if (len > 0) {
        strncpy(token->value, &tokenizer->input[start], len);
        token->value[len] = '\0';
        token->type = TOKEN_IDENTIFIER;
        token->line = tokenizer->line;
        token->column = tokenizer->column - len;
        return 1;
    }
    return 0;
}

int tokenizer_read_operator(struct Tokenizer *tokenizer, struct Token *token) {
    if (tokenizer == NULL || token == NULL) return 0;
    if (is_operator_char(tokenizer->input[tokenizer->position])) {
        token->value[0] = tokenizer->input[tokenizer->position];
        token->value[1] = '\0';
        token->type = TOKEN_OPERATOR;
        token->line = tokenizer->line;
        token->column = tokenizer->column;
        tokenizer->position++;
        tokenizer->column++;
        return 1;
    }
    return 0;
}

int tokenizer_read_paren(struct Tokenizer *tokenizer, struct Token *token) {
    if (tokenizer == NULL || token == NULL) return 0;
    char c = tokenizer->input[tokenizer->position];
    if (c == '(' || c == ')') {
        token->value[0] = c;
        token->value[1] = '\0';
        token->type = TOKEN_PAREN;
        token->line = tokenizer->line;
        token->column = tokenizer->column;
        tokenizer->position++;
        tokenizer->column++;
        return 1;
    }
    return 0;
}

int tokenizer_next_token(struct Tokenizer *tokenizer, struct Token *token) {
    if (tokenizer == NULL || token == NULL) return 0;
    if (tokenizer->input[tokenizer->position] == '\0') {
        token->type = TOKEN_EOF;
        strcpy(token->value, "");
        token->line = tokenizer->line;
        token->column = tokenizer->column;
        return 1;
    }
    
    tokenizer_skip_whitespace(tokenizer);
    
    if (tokenizer_read_number(tokenizer, token)) return 1;
    if (tokenizer_read_identifier(tokenizer, token)) return 1;
    if (tokenizer_read_operator(token