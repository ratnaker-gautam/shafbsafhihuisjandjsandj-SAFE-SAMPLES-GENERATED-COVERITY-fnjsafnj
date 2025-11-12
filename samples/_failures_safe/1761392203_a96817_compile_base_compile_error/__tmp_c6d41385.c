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
    return c == '+' || c == '-' || c == '*' || c == '/' || 
           c == '=' || c == '<' || c == '>' || c == '!';
}

void skip_whitespace(struct Tokenizer *tokenizer) {
    if (tokenizer == NULL) return;
    
    while (tokenizer->position < tokenizer->input_len && 
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

int tokenize_number(struct Tokenizer *tokenizer) {
    if (tokenizer == NULL || tokenizer->token_count >= MAX_TOKENS) return 0;
    
    size_t start = tokenizer->position;
    while (tokenizer->position < tokenizer->input_len && 
           isdigit(tokenizer->input[tokenizer->position])) {
        tokenizer->position++;
        tokenizer->column++;
    }
    
    if (tokenizer->position - start >= MAX_TOKEN_LEN) return 0;
    
    struct Token *token = &tokenizer->tokens[tokenizer->token_count];
    token->type = TOKEN_NUMBER;
    token->line = tokenizer->line;
    token->column = tokenizer->column - (tokenizer->position - start);
    
    strncpy(token->value, &tokenizer->input[start], tokenizer->position - start);
    token->value[tokenizer->position - start] = '\0';
    
    tokenizer->token_count++;
    return 1;
}

int tokenize_identifier(struct Tokenizer *tokenizer) {
    if (tokenizer == NULL || tokenizer->token_count >= MAX_TOKENS) return 0;
    
    size_t start = tokenizer->position;
    while (tokenizer->position < tokenizer->input_len && 
           (isalnum(tokenizer->input[tokenizer->position]) || 
            tokenizer->input[tokenizer->position] == '_')) {
        tokenizer->position++;
        tokenizer->column++;
    }
    
    if (tokenizer->position - start >= MAX_TOKEN_LEN) return 0;
    
    struct Token *token = &tokenizer->tokens[tokenizer->token_count];
    token->type = TOKEN_IDENTIFIER;
    token->line = tokenizer->line;
    token->column = tokenizer->column - (tokenizer->position - start);
    
    strncpy(token->value, &tokenizer->input[start], tokenizer->position - start);
    token->value[tokenizer->position - start] = '\0';
    
    tokenizer->token_count++;
    return 1;
}

int tokenize_string(struct Tokenizer *tokenizer) {
    if (tokenizer == NULL || tokenizer->token_count >= MAX_TOKENS) return 0;
    
    if (tokenizer->input[tokenizer->position] != '"') return 0;
    
    size_t start = tokenizer->position;
    tokenizer->position++;
    tokenizer->column++;
    
    while (tokenizer->position < tokenizer->input_len && 
           tokenizer->input[tokenizer->position] != '"') {
        if (tokenizer->input[tokenizer->position] == '\n') {
            tokenizer->line++;
            tokenizer->column = 1;
        } else {
            tokenizer->column++;
        }
        tokenizer->position++;
    }
    
    if (tokenizer->position >= tokenizer->input_len) return 0;
    
    tokenizer->position++;
    tokenizer->column++;
    
    if (tokenizer->position - start - 2 >= MAX_TOKEN_LEN) return 0;
    
    struct Token *token = &tokenizer->tokens[tokenizer->token_count];
    token->type = TOKEN_STRING;