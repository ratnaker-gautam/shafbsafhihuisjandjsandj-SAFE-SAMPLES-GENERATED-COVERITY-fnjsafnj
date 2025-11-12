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

int is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
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
    
    while (isdigit(tokenizer->input[tokenizer->position]) && buf_pos < MAX_TOKEN_LEN - 1) {
        buffer[buf_pos++] = tokenizer->input[tokenizer->position++];
        tokenizer->column++;
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
    
    if (isalpha(tokenizer->input[tokenizer->position]) || tokenizer->input[tokenizer->position] == '_') {
        buffer[buf_pos++] = tokenizer->input[tokenizer->position++];
        tokenizer->column++;
        
        while ((isalnum(tokenizer->input[tokenizer->position]) || tokenizer->input[tokenizer->position] == '_') && 
               buf_pos < MAX_TOKEN_LEN - 1) {
            buffer[buf_pos++] = tokenizer->input[tokenizer->position++];
            tokenizer->column++;
        }
        
        buffer[buf_pos] = '\0';
        return add_token(tokenizer, TOKEN_IDENTIFIER, buffer);
    }
    return 0;
}

int parse_operator(struct Tokenizer *tokenizer) {
    char buffer[3] = {0};
    int buf_pos = 0;
    
    if (is_operator_char(tokenizer->input[tokenizer->position])) {
        buffer[buf_pos++] = tokenizer->input[tokenizer->position++];
        tokenizer->column++;
        
        if (is_operator_char(tokenizer->input[tokenizer->position]) && buf_pos < 2) {
            buffer[buf_pos++] = tokenizer->input[tokenizer->position++];
            tokenizer->column++;
        }
        
        buffer[buf_pos] = '\0';
        return add_token(tokenizer, TOKEN_OPERATOR, buffer);
    }
    return 0;
}

int parse_string(struct Tokenizer *tokenizer) {
    char buffer[MAX_TOKEN_LEN] = {0};
    int buf_pos = 0;
    
    if (tokenizer->input[tokenizer->position] == '"') {
        buffer[buf_pos++] = tokenizer->input[tokenizer->position++];
        tokenizer->column++;
        
        while (tokenizer->input[tokenizer->position] != '"' && 
               tokenizer->input[tokenizer->position] != '\0' && 
               buf_pos < MAX_TOKEN_LEN - 1) {
            buffer[buf_pos++] = tokenizer->input[tokenizer->position++];
            tokenizer->column++;
        }
        
        if (tokenizer->input[tokenizer->position] == '"') {
            buffer[buf_pos++] = tokenizer->input[tokenizer->position++];
            tokenizer->column++;
            buffer[buf_pos] = '\0';
            return add_token(tokenizer, TOKEN_STRING, buffer);
        }
    }
    return 0;
}

void skip_whitespace(struct Tokenizer *tokenizer) {
    while (is_whitespace(tokenizer->input[tokenizer->position])) {
        if (tokenizer->input[tokenizer->position] == '\n') {
            tokenizer->line++;
            tokenizer->column = 1;
        } else {
            tokenizer->column