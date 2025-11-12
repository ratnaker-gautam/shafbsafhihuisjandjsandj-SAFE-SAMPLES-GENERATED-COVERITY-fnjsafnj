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

int is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
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

void tokenize(struct Tokenizer *tokenizer) {
    while (tokenizer->input[tokenizer->position] != '\0' && tokenizer->token_count < MAX_TOKENS) {
        char current = tokenizer->input[tokenizer->position];
        
        if (is_whitespace(current)) {
            if (current == '\n') {
                tokenizer->line++;
                tokenizer->column = 1;
            } else {
                tokenizer->column++;
            }
            tokenizer->position++;
            continue;
        }
        
        if (isdigit(current)) {
            char number[MAX_TOKEN_LEN] = {0};
            int i = 0;
            
            while (isdigit(tokenizer->input[tokenizer->position]) && i < MAX_TOKEN_LEN - 1) {
                number[i++] = tokenizer->input[tokenizer->position++];
                tokenizer->column++;
            }
            number[i] = '\0';
            add_token(tokenizer, TOKEN_NUMBER, number);
            continue;
        }
        
        if (isalpha(current) || current == '_') {
            char identifier[MAX_TOKEN_LEN] = {0};
            int i = 0;
            
            while ((isalnum(tokenizer->input[tokenizer->position]) || tokenizer->input[tokenizer->position] == '_') && i < MAX_TOKEN_LEN - 1) {
                identifier[i++] = tokenizer->input[tokenizer->position++];
                tokenizer->column++;
            }
            identifier[i] = '\0';
            add_token(tokenizer, TOKEN_IDENTIFIER, identifier);
            continue;
        }
        
        if (current == '"') {
            char string[MAX_TOKEN_LEN] = {0};
            int i = 0;
            tokenizer->position++;
            tokenizer->column++;
            
            while (tokenizer->input[tokenizer->position] != '"' && tokenizer->input[tokenizer->position] != '\0' && i < MAX_TOKEN_LEN - 1) {
                string[i++] = tokenizer->input[tokenizer->position++];
                tokenizer->column++;
            }
            
            if (tokenizer->input[tokenizer->position] == '"') {
                tokenizer->position++;
                tokenizer->column++;
                string[i] = '\0';
                add_token(tokenizer, TOKEN_STRING, string);
            } else {
                add_token(tokenizer, TOKEN_ERROR, "Unterminated string");
            }
            continue;
        }
        
        if (is_operator(current)) {
            char op[2] = {current, '\0'};
            add_token(tokenizer, TOKEN_OPERATOR, op);
            tokenizer->position++;
            tokenizer->column++;
            continue;
        }
        
        char error[2] = {current, '\0'};
        add_token(tokenizer, TOKEN_ERROR, error);
        tokenizer->position++;
        tokenizer->column++;
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
        default: return "UNKNOWN";
    }
}

void print_tokens(struct Tokenizer *tokenizer) {
    printf("Token count: %d\n