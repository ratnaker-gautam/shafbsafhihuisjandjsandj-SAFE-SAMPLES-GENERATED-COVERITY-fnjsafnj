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

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void add_token(struct Tokenizer *tokenizer, enum TokenType type, const char *value) {
    if (tokenizer->token_count >= MAX_TOKENS) return;
    
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
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    
    while (isdigit(tokenizer->input[tokenizer->position]) && i < MAX_TOKEN_LEN - 1) {
        buffer[i++] = tokenizer->input[tokenizer->position++];
        tokenizer->column++;
    }
    
    if (tokenizer->input[tokenizer->position] == '.' && i < MAX_TOKEN_LEN - 1) {
        buffer[i++] = tokenizer->input[tokenizer->position++];
        tokenizer->column++;
        
        while (isdigit(tokenizer->input[tokenizer->position]) && i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = tokenizer->input[tokenizer->position++];
            tokenizer->column++;
        }
    }
    
    buffer[i] = '\0';
    add_token(tokenizer, TOKEN_NUMBER, buffer);
    return 1;
}

int parse_identifier(struct Tokenizer *tokenizer) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    
    while (isalnum(tokenizer->input[tokenizer->position]) && i < MAX_TOKEN_LEN - 1) {
        buffer[i++] = tokenizer->input[tokenizer->position++];
        tokenizer->column++;
    }
    
    buffer[i] = '\0';
    add_token(tokenizer, TOKEN_IDENTIFIER, buffer);
    return 1;
}

int parse_string(struct Tokenizer *tokenizer) {
    if (tokenizer->input[tokenizer->position] != '"') return 0;
    
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    tokenizer->position++;
    tokenizer->column++;
    
    while (tokenizer->input[tokenizer->position] != '\0' && 
           tokenizer->input[tokenizer->position] != '"' && 
           i < MAX_TOKEN_LEN - 1) {
        buffer[i++] = tokenizer->input[tokenizer->position++];
        tokenizer->column++;
    }
    
    if (tokenizer->input[tokenizer->position] == '"') {
        tokenizer->position++;
        tokenizer->column++;
        buffer[i] = '\0';
        add_token(tokenizer, TOKEN_STRING, buffer);
        return 1;
    }
    
    return 0;
}

int parse_operator(struct Tokenizer *tokenizer) {
    char buffer[3] = {0};
    buffer[0] = tokenizer->input[tokenizer->position];
    tokenizer->position++;
    tokenizer->column++;
    
    if (is_operator(tokenizer->input[tokenizer->position])) {
        buffer[1] = tokenizer->input[tokenizer->position];
        tokenizer->position++;
        tokenizer->column++;
    }
    
    add_token(tokenizer, TOKEN_OPERATOR, buffer);
    return 1;
}

int tokenize(struct Tokenizer *tokenizer) {
    while (tokenizer->input[tokenizer->position] != '\0' && 
           tokenizer->token_count < MAX_TOKENS) {
        skip_whitespace(tokenizer);
        
        if (tokenizer->