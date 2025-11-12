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

struct TokenizerState {
    struct Token tokens[MAX_TOKENS];
    uint32_t token_count;
    uint32_t current_line;
    uint32_t current_column;
    char input[MAX_INPUT_LEN];
    uint32_t input_length;
    uint32_t position;
};

struct ParserContext {
    struct TokenizerState tokenizer;
    uint32_t current_token;
};

void initialize_tokenizer(struct TokenizerState *tokenizer, const char *input) {
    tokenizer->token_count = 0;
    tokenizer->current_line = 1;
    tokenizer->current_column = 1;
    tokenizer->position = 0;
    
    size_t input_len = strlen(input);
    if (input_len >= MAX_INPUT_LEN - 1) {
        input_len = MAX_INPUT_LEN - 1;
    }
    memcpy(tokenizer->input, input, input_len);
    tokenizer->input[input_len] = '\0';
    tokenizer->input_length = input_len;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_delimiter_char(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ';' || c == ',';
}

int is_keyword(const char *str) {
    const char *keywords[] = {"if", "else", "while", "for", "return", "int", "float"};
    size_t num_keywords = sizeof(keywords) / sizeof(keywords[0]);
    
    for (size_t i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void add_token(struct TokenizerState *tokenizer, enum TokenType type, const char *value) {
    if (tokenizer->token_count >= MAX_TOKENS) {
        return;
    }
    
    struct Token *token = &tokenizer->tokens[tokenizer->token_count];
    token->type = type;
    token->line = tokenizer->current_line;
    token->column = tokenizer->current_column;
    
    size_t value_len = strlen(value);
    if (value_len >= MAX_TOKEN_LEN - 1) {
        value_len = MAX_TOKEN_LEN - 1;
    }
    memcpy(token->value, value, value_len);
    token->value[value_len] = '\0';
    
    tokenizer->token_count++;
}

void skip_whitespace(struct TokenizerState *tokenizer) {
    while (tokenizer->position < tokenizer->input_length && 
           isspace(tokenizer->input[tokenizer->position])) {
        if (tokenizer->input[tokenizer->position] == '\n') {
            tokenizer->current_line++;
            tokenizer->current_column = 1;
        } else {
            tokenizer->current_column++;
        }
        tokenizer->position++;
    }
}

void tokenize_number(struct TokenizerState *tokenizer) {
    uint32_t start = tokenizer->position;
    while (tokenizer->position < tokenizer->input_length && 
           isdigit(tokenizer->input[tokenizer->position])) {
        tokenizer->position++;
        tokenizer->current_column++;
    }
    
    if (tokenizer->position - start > 0) {
        char number[MAX_TOKEN_LEN];
        size_t len = tokenizer->position - start;
        if (len >= MAX_TOKEN_LEN - 1) {
            len = MAX_TOKEN_LEN - 1;
        }
        memcpy(number, &tokenizer->input[start], len);
        number[len] = '\0';
        add_token(tokenizer, TOKEN_NUMBER, number);
    }
}

void tokenize_identifier_or_keyword(struct TokenizerState *tokenizer) {
    uint32_t start = tokenizer->position;
    while (tokenizer->position < tokenizer->input_length && 
           (isalnum(tokenizer->input[tokenizer->position]) || 
            tokenizer->input[tokenizer->position] == '_')) {
        tokenizer->position++;
        tokenizer->current_column++;
    }
    
    if (tokenizer->position - start > 0) {
        char identifier[MAX_TOKEN_LEN];
        size_t len = tokenizer->position - start;
        if (len >= MAX_TOKEN_LEN - 1) {
            len = MAX_TOKEN_LEN - 1;
        }
        memcpy(identifier, &tokenizer->input[start], len);
        identifier[len] = '\0';
        
        if (is_keyword(identifier)) {
            add_token(tokenizer, TOKEN_KEYWORD, identifier);
        } else {
            add_token(tokenizer, TOKEN_IDENTIFIER, identifier);
        }
    }
}

void token