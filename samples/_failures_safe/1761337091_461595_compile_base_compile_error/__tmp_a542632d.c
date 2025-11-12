//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 1000
#define MAX_TOKEN_LEN 64

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
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
    return strchr("+-*/=<>!&|", c) != NULL;
}

int is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
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
    if (c == '\n') {
        tokenizer->line++;
        tokenizer->column = 1;
    } else {
        tokenizer->column++;
    }
    tokenizer->position++;
    return c;
}

void tokenize_number(struct Tokenizer *tokenizer) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    
    while (isdigit(peek(tokenizer)) && i < MAX_TOKEN_LEN - 1) {
        buffer[i++] = advance(tokenizer);
    }
    
    if (peek(tokenizer) == '.' && i < MAX_TOKEN_LEN - 1) {
        buffer[i++] = advance(tokenizer);
        while (isdigit(peek(tokenizer)) && i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = advance(tokenizer);
        }
    }
    
    buffer[i] = '\0';
    add_token(tokenizer, TOKEN_NUMBER, buffer);
}

void tokenize_identifier(struct Tokenizer *tokenizer) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    
    while ((isalnum(peek(tokenizer)) || peek(tokenizer) == '_') && i < MAX_TOKEN_LEN - 1) {
        buffer[i++] = advance(tokenizer);
    }
    
    buffer[i] = '\0';
    add_token(tokenizer, TOKEN_IDENTIFIER, buffer);
}

void tokenize_operator(struct Tokenizer *tokenizer) {
    char buffer[3] = {0};
    buffer[0] = advance(tokenizer);
    
    if (is_operator_char(peek(tokenizer))) {
        buffer[1] = advance(tokenizer);
    }
    
    add_token(tokenizer, TOKEN_OPERATOR, buffer);
}

void tokenize_string(struct Tokenizer *tokenizer) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    
    advance(tokenizer);
    
    while (peek(tokenizer) != '"' && peek(tokenizer) != '\0' && i < MAX_TOKEN_LEN - 1) {
        buffer[i++] = advance(tokenizer);
    }
    
    if (peek(tokenizer) == '"') {
        advance(tokenizer);
    }
    
    buffer[i] = '\0';
    add_token(tokenizer, TOKEN_STRING, buffer);
}

int tokenize(struct Tokenizer *tokenizer) {
    while (peek(tokenizer) != '\0') {
        if (is_whitespace(peek(tokenizer))) {
            advance(tokenizer);
            continue;
        }
        
        if (isdigit(peek(tokenizer))) {
            tokenize_number(tokenizer);
        } else if (isalpha(peek(tokenizer)) || peek(tokenizer) == '_') {
            tokenize_identifier(tokenizer);
        } else if (is_operator_char(peek(tokenizer))) {
            tokenize_operator(tokenizer);
        } else if (peek(tokenizer) == '"') {
            tokenize_string(tokenizer);
        } else {
            advance(tokenizer);
        }
    }
    
    add_token(tokenizer, TOKEN_EOF, "");
    return tokenizer->token_count;
}

void print_tokens(struct Tokenizer *tokenizer) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF"
    };
    
    for (int i = 0; i < tokenizer->token_count; i++) {
        struct Token *token = &tokenizer->tokens[i];
        printf("Line %d, Column %d: