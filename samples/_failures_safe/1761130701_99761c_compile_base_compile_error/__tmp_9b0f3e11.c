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

int is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

int read_next_token(struct Tokenizer *tokenizer) {
    if (tokenizer == NULL || tokenizer->input == NULL) return 0;
    if (tokenizer->token_count >= MAX_TOKENS) return 0;
    
    while (tokenizer->input[tokenizer->position] != '\0' && 
           is_whitespace(tokenizer->input[tokenizer->position])) {
        if (tokenizer->input[tokenizer->position] == '\n') {
            tokenizer->line++;
            tokenizer->column = 1;
        } else {
            tokenizer->column++;
        }
        tokenizer->position++;
    }
    
    if (tokenizer->input[tokenizer->position] == '\0') {
        struct Token *token = &tokenizer->tokens[tokenizer->token_count];
        token->type = TOKEN_EOF;
        strncpy(token->value, "", MAX_TOKEN_LEN);
        token->line = tokenizer->line;
        token->column = tokenizer->column;
        tokenizer->token_count++;
        return 1;
    }
    
    struct Token *token = &tokenizer->tokens[tokenizer->token_count];
    token->line = tokenizer->line;
    token->column = tokenizer->column;
    
    char current = tokenizer->input[tokenizer->position];
    
    if (isdigit(current)) {
        token->type = TOKEN_NUMBER;
        int i = 0;
        while (isdigit(tokenizer->input[tokenizer->position]) && i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = tokenizer->input[tokenizer->position++];
            tokenizer->column++;
        }
        token->value[i] = '\0';
    } else if (isalpha(current) || current == '_') {
        token->type = TOKEN_IDENTIFIER;
        int i = 0;
        while ((isalnum(tokenizer->input[tokenizer->position]) || 
                tokenizer->input[tokenizer->position] == '_') && 
               i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = tokenizer->input[tokenizer->position++];
            tokenizer->column++;
        }
        token->value[i] = '\0';
    } else if (is_operator_char(current)) {
        token->type = TOKEN_OPERATOR;
        int i = 0;
        while (is_operator_char(tokenizer->input[tokenizer->position]) && 
               i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = tokenizer->input[tokenizer->position++];
            tokenizer->column++;
        }
        token->value[i] = '\0';
    } else if (current == '"') {
        token->type = TOKEN_STRING;
        int i = 0;
        token->value[i++] = tokenizer->input[tokenizer->position++];
        tokenizer->column++;
        
        while (tokenizer->input[tokenizer->position] != '"' && 
               tokenizer->input[tokenizer->position] != '\0' && 
               i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = tokenizer->input[tokenizer->position++];
            tokenizer->column++;
        }
        
        if (tokenizer->input[tokenizer->position] == '"') {
            token->value[i++] = tokenizer->input[tokenizer->position++];
            tokenizer->column++;
        }
        token->value[i] = '\0';
    } else {
        token->type = TOKEN_INVALID;
        token->value[0] = tokenizer->input[tokenizer->position++];
        token->value[1] = '\0';
        tokenizer->column++;
    }
    
    tokenizer->token_count++;
    return 1;
}

void tokenize_input(struct Tokenizer *tokenizer) {
    if (tokenizer == NULL) return;
    while (tokenizer->input[tokenizer->position] != '\0' && 
           tokenizer->token_count < MAX_TOKENS) {
        if (!read_next_token(tokenizer)) {
            break