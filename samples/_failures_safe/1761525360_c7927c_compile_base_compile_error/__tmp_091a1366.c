//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
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
    struct Token tokens[MAX_TOKENS];
    size_t token_count;
    size_t current_token;
    char input[MAX_INPUT_LEN];
    size_t input_len;
    size_t position;
    int line;
    int column;
};

void tokenizer_init(struct Tokenizer *tokenizer, const char *input) {
    if (tokenizer == NULL || input == NULL) return;
    
    size_t input_len = strlen(input);
    if (input_len >= MAX_INPUT_LEN) input_len = MAX_INPUT_LEN - 1;
    
    strncpy(tokenizer->input, input, input_len);
    tokenizer->input[input_len] = '\0';
    tokenizer->input_len = input_len;
    tokenizer->token_count = 0;
    tokenizer->current_token = 0;
    tokenizer->position = 0;
    tokenizer->line = 1;
    tokenizer->column = 1;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

void add_token(struct Tokenizer *tokenizer, enum TokenType type, const char *value) {
    if (tokenizer == NULL || value == NULL || tokenizer->token_count >= MAX_TOKENS) return;
    
    struct Token *token = &tokenizer->tokens[tokenizer->token_count];
    token->type = type;
    token->line = tokenizer->line;
    token->column = tokenizer->column;
    
    size_t len = strlen(value);
    if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
    strncpy(token->value, value, len);
    token->value[len] = '\0';
    
    tokenizer->token_count++;
}

char peek_char(struct Tokenizer *tokenizer) {
    if (tokenizer == NULL || tokenizer->position >= tokenizer->input_len) return '\0';
    return tokenizer->input[tokenizer->position];
}

char next_char(struct Tokenizer *tokenizer) {
    if (tokenizer == NULL || tokenizer->position >= tokenizer->input_len) return '\0';
    
    char c = tokenizer->input[tokenizer->position++];
    if (c == '\n') {
        tokenizer->line++;
        tokenizer->column = 1;
    } else {
        tokenizer->column++;
    }
    return c;
}

void skip_whitespace(struct Tokenizer *tokenizer) {
    if (tokenizer == NULL) return;
    
    while (tokenizer->position < tokenizer->input_len && 
           isspace(tokenizer->input[tokenizer->position])) {
        next_char(tokenizer);
    }
}

void tokenize_number(struct Tokenizer *tokenizer) {
    if (tokenizer == NULL) return;
    
    char buffer[MAX_TOKEN_LEN];
    size_t index = 0;
    
    while (tokenizer->position < tokenizer->input_len && 
           isdigit(peek_char(tokenizer)) && index < MAX_TOKEN_LEN - 1) {
        buffer[index++] = next_char(tokenizer);
    }
    
    if (index > 0) {
        buffer[index] = '\0';
        add_token(tokenizer, TOKEN_NUMBER, buffer);
    }
}

void tokenize_identifier(struct Tokenizer *tokenizer) {
    if (tokenizer == NULL) return;
    
    char buffer[MAX_TOKEN_LEN];
    size_t index = 0;
    
    while (tokenizer->position < tokenizer->input_len && 
           (isalnum(peek_char(tokenizer)) || peek_char(tokenizer) == '_') && 
           index < MAX_TOKEN_LEN - 1) {
        buffer[index++] = next_char(tokenizer);
    }
    
    if (index > 0) {
        buffer[index] = '\0';
        add_token(tokenizer, TOKEN_IDENTIFIER, buffer);
    }
}

void tokenize_string(struct Tokenizer *tokenizer) {
    if (tokenizer == NULL) return;
    
    char buffer[MAX_TOKEN_LEN];
    size_t index = 0;
    char quote = next_char(tokenizer);
    
    while (tokenizer->position < tokenizer->input_len && 
           peek_char(tokenizer) != quote && index < MAX_TOKEN_LEN - 1) {
        buffer[index++] = next_char(tokenizer);
    }
    
    if (index > 0) {
        buffer[index] = '\0';
        add_token(tokenizer, TOKEN_STRING, buffer);
    }
    
    if (peek_char(tokenizer) == quote) {
        next_char(tokenizer);
    }
}

void tokenize_operator(struct Tokenizer *tokenizer) {
    if (tokenizer == NULL) return;
    
    char buffer[MAX_TOKEN_LEN];
    size_t index =