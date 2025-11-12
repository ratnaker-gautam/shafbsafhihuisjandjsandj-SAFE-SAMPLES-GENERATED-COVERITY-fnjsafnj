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

int is_valid_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

int add_token(struct Tokenizer *tokenizer, enum TokenType type, const char *value, int line, int column) {
    if (tokenizer->token_count >= MAX_TOKENS) {
        return 0;
    }
    
    struct Token *token = &tokenizer->tokens[tokenizer->token_count];
    token->type = type;
    token->line = line;
    token->column = column;
    
    size_t len = strlen(value);
    if (len >= MAX_TOKEN_LEN) {
        len = MAX_TOKEN_LEN - 1;
    }
    strncpy(token->value, value, len);
    token->value[len] = '\0';
    
    tokenizer->token_count++;
    return 1;
}

char peek_char(struct Tokenizer *tokenizer) {
    return tokenizer->input[tokenizer->position];
}

char next_char(struct Tokenizer *tokenizer) {
    char c = tokenizer->input[tokenizer->position];
    if (c == '\0') {
        return '\0';
    }
    
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
    while (isspace(peek_char(tokenizer))) {
        next_char(tokenizer);
    }
}

int parse_number(struct Tokenizer *tokenizer) {
    char buffer[MAX_TOKEN_LEN];
    int buffer_pos = 0;
    int start_line = tokenizer->line;
    int start_column = tokenizer->column;
    
    while (isdigit(peek_char(tokenizer)) && buffer_pos < MAX_TOKEN_LEN - 1) {
        buffer[buffer_pos++] = next_char(tokenizer);
    }
    
    if (buffer_pos == 0) {
        return 0;
    }
    
    buffer[buffer_pos] = '\0';
    return add_token(tokenizer, TOKEN_NUMBER, buffer, start_line, start_column);
}

int parse_identifier(struct Tokenizer *tokenizer) {
    char buffer[MAX_TOKEN_LEN];
    int buffer_pos = 0;
    int start_line = tokenizer->line;
    int start_column = tokenizer->column;
    
    if (!isalpha(peek_char(tokenizer))) {
        return 0;
    }
    
    while (is_valid_identifier_char(peek_char(tokenizer)) && buffer_pos < MAX_TOKEN_LEN - 1) {
        buffer[buffer_pos++] = next_char(tokenizer);
    }
    
    buffer[buffer_pos] = '\0';
    return add_token(tokenizer, TOKEN_IDENTIFIER, buffer, start_line, start_column);
}

int parse_operator(struct Tokenizer *tokenizer) {
    char buffer[3] = {0};
    int start_line = tokenizer->line;
    int start_column = tokenizer->column;
    
    char first = peek_char(tokenizer);
    if (!is_operator_char(first)) {
        return 0;
    }
    
    buffer[0] = next_char(tokenizer);
    
    char second = peek_char(tokenizer);
    if (is_operator_char(second) && 
        ((first == '<' && second == '=') || 
         (first == '>' && second == '=') ||
         (first == '=' && second == '='))) {
        buffer[1] = next_char(tokenizer);
    }
    
    return add_token(tokenizer, TOKEN_OPERATOR, buffer, start_line, start_column);
}

int parse_string(struct Tokenizer *tokenizer) {
    char buffer[MAX_TOKEN_LEN];
    int buffer_pos = 0;
    int start_line = tokenizer->line;
    int start_column = tokenizer->column;
    
    if (peek_char(tokenizer) != '"') {
        return 0;
    }
    
    next_char(tokenizer);
    
    while (peek_char(tokenizer) != '"' && peek_char(tokenizer) != '\0' && buffer_pos < MAX_TOKEN_LEN - 1) {
        buffer[buffer_pos++] = next_char(tokenizer);
    }
    
    if (peek_char(tokenizer) != '"') {
        return 0;
    }
    
    next_char(tokenizer);
    buffer[buffer_pos