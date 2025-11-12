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

void add_token(struct Tokenizer *tokenizer, enum TokenType type, const char *value) {
    if (tokenizer->token_count >= MAX_TOKENS) return;
    
    struct Token *token = &tokenizer->tokens[tokenizer->token_count];
    token->type = type;
    token->line = tokenizer->line;
    token->column = tokenizer->column;
    
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
    
    tokenizer->token_count++;
}

char peek(struct Tokenizer *tokenizer) {
    return tokenizer->input[tokenizer->position];
}

char advance(struct Tokenizer *tokenizer) {
    char c = tokenizer->input[tokenizer->position];
    if (c == '\0') return c;
    
    tokenizer->position++;
    if (c == '\n') {
        tokenizer->line++;
        tokenizer->column = 1;
    } else {
        tokenizer->column++;
    }
    return c;
}

void skip_whitespace(struct Tokenizer *tokenizer) {
    while (isspace((unsigned char)peek(tokenizer))) {
        advance(tokenizer);
    }
}

void read_number(struct Tokenizer *tokenizer) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    
    while (isdigit((unsigned char)peek(tokenizer)) && i < MAX_TOKEN_LEN - 1) {
        buffer[i++] = advance(tokenizer);
    }
    buffer[i] = '\0';
    add_token(tokenizer, TOKEN_NUMBER, buffer);
}

void read_identifier(struct Tokenizer *tokenizer) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    
    while ((isalnum((unsigned char)peek(tokenizer)) || peek(tokenizer) == '_') && i < MAX_TOKEN_LEN - 1) {
        buffer[i++] = advance(tokenizer);
    }
    buffer[i] = '\0';
    add_token(tokenizer, TOKEN_IDENTIFIER, buffer);
}

void read_operator(struct Tokenizer *tokenizer) {
    char buffer[3] = {0};
    buffer[0] = advance(tokenizer);
    
    if (is_operator_char(peek(tokenizer))) {
        buffer[1] = advance(tokenizer);
    }
    
    add_token(tokenizer, TOKEN_OPERATOR, buffer);
}

void read_string(struct Tokenizer *tokenizer) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    
    advance(tokenizer);
    while (peek(tokenizer) != '"' && peek(tokenizer) != '\0' && i < MAX_TOKEN_LEN - 1) {
        buffer[i++] = advance(tokenizer);
    }
    
    if (peek(tokenizer) == '"') {
        advance(tokenizer);
        buffer[i] = '\0';
        add_token(tokenizer, TOKEN_STRING, buffer);
    } else {
        add_token(tokenizer, TOKEN_ERROR, "Unterminated string");
    }
}

void tokenize(struct Tokenizer *tokenizer) {
    while (peek(tokenizer) != '\0' && tokenizer->token_count < MAX_TOKENS) {
        skip_whitespace(tokenizer);
        
        char c = peek(tokenizer);
        if (c == '\0') break;
        
        if (isdigit((unsigned char)c)) {
            read_number(tokenizer);
        } else if (isalpha((unsigned char)c) || c == '_') {
            read_identifier(tokenizer);
        } else if (is_operator_char(c)) {
            read_operator(tokenizer);
        } else if (c == '"') {
            read_string(tokenizer);
        } else {
            char error[2] = {c, '\0'};
            add_token(tokenizer, TOKEN_ERROR, error);
            advance(tokenizer);
        }
    }
    
    add_token(tokenizer, TOKEN_EOF, "");
}

const char* token_type_name(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_ERROR: return "ERROR";