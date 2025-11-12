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
    
    size_t len = strlen(value);
    if (len >= MAX_TOKEN_LEN) {
        len = MAX_TOKEN_LEN - 1;
    }
    strncpy(token->value, value, len);
    token->value[len] = '\0';
    
    tokenizer->token_count++;
}

char peek_char(struct Tokenizer *tokenizer) {
    if (tokenizer->input[tokenizer->position] == '\0') {
        return '\0';
    }
    return tokenizer->input[tokenizer->position];
}

char next_char(struct Tokenizer *tokenizer) {
    char c = peek_char(tokenizer);
    if (c != '\0') {
        tokenizer->position++;
        if (c == '\n') {
            tokenizer->line++;
            tokenizer->column = 1;
        } else {
            tokenizer->column++;
        }
    }
    return c;
}

void skip_whitespace(struct Tokenizer *tokenizer) {
    while (isspace(peek_char(tokenizer))) {
        next_char(tokenizer);
    }
}

void read_number(struct Tokenizer *tokenizer) {
    char buffer[MAX_TOKEN_LEN];
    int index = 0;
    
    while (isdigit(peek_char(tokenizer)) && index < MAX_TOKEN_LEN - 1) {
        buffer[index++] = next_char(tokenizer);
    }
    
    if (peek_char(tokenizer) == '.' && index < MAX_TOKEN_LEN - 1) {
        buffer[index++] = next_char(tokenizer);
        while (isdigit(peek_char(tokenizer)) && index < MAX_TOKEN_LEN - 1) {
            buffer[index++] = next_char(tokenizer);
        }
    }
    
    buffer[index] = '\0';
    add_token(tokenizer, TOKEN_NUMBER, buffer);
}

void read_identifier(struct Tokenizer *tokenizer) {
    char buffer[MAX_TOKEN_LEN];
    int index = 0;
    
    while (isalnum(peek_char(tokenizer)) && index < MAX_TOKEN_LEN - 1) {
        buffer[index++] = next_char(tokenizer);
    }
    
    buffer[index] = '\0';
    add_token(tokenizer, TOKEN_IDENTIFIER, buffer);
}

void read_operator(struct Tokenizer *tokenizer) {
    char buffer[3] = {0};
    buffer[0] = next_char(tokenizer);
    
    if (is_operator_char(peek_char(tokenizer))) {
        buffer[1] = next_char(tokenizer);
    }
    
    add_token(tokenizer, TOKEN_OPERATOR, buffer);
}

void read_string(struct Tokenizer *tokenizer) {
    char buffer[MAX_TOKEN_LEN];
    int index = 0;
    
    char quote = next_char(tokenizer);
    
    while (peek_char(tokenizer) != quote && peek_char(tokenizer) != '\0' && index < MAX_TOKEN_LEN - 1) {
        buffer[index++] = next_char(tokenizer);
    }
    
    if (peek_char(tokenizer) == quote) {
        next_char(tokenizer);
    }
    
    buffer[index] = '\0';
    add_token(tokenizer, TOKEN_STRING, buffer);
}

int tokenize(struct Tokenizer *tokenizer) {
    while (peek_char(tokenizer) != '\0') {
        skip_whitespace(tokenizer);
        
        char current = peek_char(tokenizer);
        if (current == '\0') break;
        
        if (isdigit(current)) {
            read_number(tokenizer);
        } else if (isalpha(current)) {
            read_identifier(tokenizer);
        } else if (is_operator_char(current)) {
            read_operator(tokenizer);
        } else if (current == '"' || current == '\'') {
            read_string(tokenizer);
        } else {
            char unknown[2] = {next_char(tokenizer), '\0'};
            add_token(tokenizer, TOKEN_UNKNOWN, unknown);
        }
    }
    
    add_token(tokenizer, TOKEN_EOF, "");
    return tokenizer->token