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
    TOKEN_DELIMITER,
    TOKEN_KEYWORD,
    TOKEN_UNKNOWN
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
    uint32_t line;
    uint32_t column;
};

struct Tokenizer {
    struct Token tokens[MAX_TOKENS];
    size_t token_count;
    char input[MAX_INPUT_LEN];
    size_t input_len;
    size_t position;
    uint32_t line;
    uint32_t column;
};

struct Parser {
    struct Tokenizer tokenizer;
    size_t current_token;
};

void tokenizer_init(struct Tokenizer *tokenizer, const char *input) {
    if (tokenizer == NULL || input == NULL) return;
    
    size_t len = strlen(input);
    if (len >= MAX_INPUT_LEN) len = MAX_INPUT_LEN - 1;
    
    strncpy(tokenizer->input, input, len);
    tokenizer->input[len] = '\0';
    tokenizer->input_len = len;
    tokenizer->position = 0;
    tokenizer->token_count = 0;
    tokenizer->line = 1;
    tokenizer->column = 1;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_delimiter_char(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == ';' || c == ',';
}

int is_keyword(const char *str) {
    const char *keywords[] = {"if", "else", "while", "for", "return", NULL};
    
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
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

void skip_whitespace(struct Tokenizer *tokenizer) {
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

void tokenize_number(struct Tokenizer *tokenizer) {
    size_t start = tokenizer->position;
    
    while (tokenizer->position < tokenizer->input_len && 
           isdigit(tokenizer->input[tokenizer->position])) {
        tokenizer->position++;
        tokenizer->column++;
    }
    
    if (tokenizer->position - start > 0) {
        char number[MAX_TOKEN_LEN];
        size_t len = tokenizer->position - start;
        if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
        
        strncpy(number, &tokenizer->input[start], len);
        number[len] = '\0';
        add_token(tokenizer, TOKEN_NUMBER, number);
    }
}

void tokenize_identifier(struct Tokenizer *tokenizer) {
    size_t start = tokenizer->position;
    
    while (tokenizer->position < tokenizer->input_len && 
           (isalnum(tokenizer->input[tokenizer->position]) || 
            tokenizer->input[tokenizer->position] == '_')) {
        tokenizer->position++;
        tokenizer->column++;
    }
    
    if (tokenizer->position - start > 0) {
        char identifier[MAX_TOKEN_LEN];
        size_t len = tokenizer->position - start;
        if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
        
        strncpy(identifier, &tokenizer->input[start], len);
        identifier[len] = '\0';
        
        if (is_keyword(identifier)) {
            add_token(tokenizer, TOKEN_KEYWORD, identifier);
        } else {
            add_token(tokenizer, TOKEN_IDENTIFIER, identifier);
        }
    }
}

int tokenize(struct Tokenizer *tokenizer) {
    if (tokenizer == NULL) return 0;
    
    while (tokenizer->position < tokenizer->input_len) {
        skip_whitespace(tokenizer);
        if (tokenizer->position >= tokenizer->input_len) break;
        
        char current = tokenizer->input[tokenizer->