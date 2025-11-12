//DeepSeek-V3 SAFE v1.3 Category: Safe ; Style: modular_functions ; Variation: parser_tokenizer
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
    const char* input;
    int position;
    int line;
    int column;
    struct Token tokens[MAX_TOKENS];
    int token_count;
};

void tokenizer_init(struct Tokenizer* tokenizer, const char* input) {
    tokenizer->input = input;
    tokenizer->position = 0;
    tokenizer->line = 1;
    tokenizer->column = 1;
    tokenizer->token_count = 0;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(struct Tokenizer* tokenizer) {
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

int parse_number(struct Tokenizer* tokenizer, struct Token* token) {
    int start = tokenizer->position;
    int len = 0;
    
    while (isdigit(tokenizer->input[tokenizer->position])) {
        tokenizer->position++;
        tokenizer->column++;
        len++;
        if (len >= MAX_TOKEN_LEN - 1) break;
    }
    
    if (len > 0) {
        strncpy(token->value, &tokenizer->input[start], len);
        token->value[len] = '\0';
        token->type = TOKEN_NUMBER;
        token->line = tokenizer->line;
        token->column = tokenizer->column - len;
        return 1;
    }
    return 0;
}

int parse_identifier(struct Tokenizer* tokenizer, struct Token* token) {
    int start = tokenizer->position;
    int len = 0;
    
    if (isalpha(tokenizer->input[tokenizer->position]) || tokenizer->input[tokenizer->position] == '_') {
        while (isalnum(tokenizer->input[tokenizer->position]) || tokenizer->input[tokenizer->position] == '_') {
            tokenizer->position++;
            tokenizer->column++;
            len++;
            if (len >= MAX_TOKEN_LEN - 1) break;
        }
        
        if (len > 0) {
            strncpy(token->value, &tokenizer->input[start], len);
            token->value[len] = '\0';
            token->type = TOKEN_IDENTIFIER;
            token->line = tokenizer->line;
            token->column = tokenizer->column - len;
            return 1;
        }
    }
    return 0;
}

int parse_operator(struct Tokenizer* tokenizer, struct Token* token) {
    if (is_operator_char(tokenizer->input[tokenizer->position])) {
        token->value[0] = tokenizer->input[tokenizer->position];
        token->value[1] = '\0';
        token->type = TOKEN_OPERATOR;
        token->line = tokenizer->line;
        token->column = tokenizer->column;
        tokenizer->position++;
        tokenizer->column++;
        return 1;
    }
    return 0;
}

int parse_string(struct Tokenizer* tokenizer, struct Token* token) {
    if (tokenizer->input[tokenizer->position] == '"') {
        int start = tokenizer->position;
        int len = 0;
        tokenizer->position++;
        tokenizer->column++;
        
        while (tokenizer->input[tokenizer->position] != '"' && tokenizer->input[tokenizer->position] != '\0') {
            tokenizer->position++;
            tokenizer->column++;
            len++;
            if (len >= MAX_TOKEN_LEN - 1) break;
        }
        
        if (tokenizer->input[tokenizer->position] == '"') {
            strncpy(token->value, &tokenizer->input[start + 1], len);
            token->value[len] = '\0';
            token->type = TOKEN_STRING;
            token->line = tokenizer->line;
            token->column = tokenizer->column - len - 1;
            tokenizer->position++;
            tokenizer->column++;
            return 1;
        }
    }
    return 0;
}

int tokenize_next