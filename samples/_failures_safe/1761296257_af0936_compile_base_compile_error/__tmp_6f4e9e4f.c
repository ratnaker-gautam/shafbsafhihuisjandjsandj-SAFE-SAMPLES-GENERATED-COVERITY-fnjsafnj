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
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
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

struct Lexer {
    const char *input;
    size_t position;
    size_t input_len;
    int line;
    int column;
    struct Token tokens[MAX_TOKENS];
    size_t token_count;
};

void lexer_init(struct Lexer *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->input_len = strlen(input);
    lexer->line = 1;
    lexer->column = 1;
    lexer->token_count = 0;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || 
           c == '=' || c == '<' || c == '>' || c == '!';
}

int is_delimiter_char(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || 
           c == '[' || c == ']' || c == ',' || c == ';' || c == ':';
}

void add_token(struct Lexer *lexer, enum TokenType type, const char *value) {
    if (lexer->token_count >= MAX_TOKENS) return;
    if (value == NULL) return;
    struct Token *token = &lexer->tokens[lexer->token_count];
    token->type = type;
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
    token->line = lexer->line;
    token->column = lexer->column;
    lexer->token_count++;
}

void skip_whitespace(struct Lexer *lexer) {
    while (lexer->position < lexer->input_len && isspace(lexer->input[lexer->position])) {
        if (lexer->input[lexer->position] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
    }
}

void read_identifier(struct Lexer *lexer) {
    char buffer[MAX_TOKEN_LEN] = {0};
    size_t buf_pos = 0;
    
    while (lexer->position < lexer->input_len && 
           (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_')) {
        if (buf_pos < MAX_TOKEN_LEN - 1) {
            buffer[buf_pos++] = lexer->input[lexer->position];
        }
        lexer->position++;
        lexer->column++;
    }
    buffer[buf_pos] = '\0';
    add_token(lexer, TOKEN_IDENTIFIER, buffer);
}

void read_number(struct Lexer *lexer) {
    char buffer[MAX_TOKEN_LEN] = {0};
    size_t buf_pos = 0;
    
    while (lexer->position < lexer->input_len && 
           (isdigit(lexer->input[lexer->position]) || lexer->input[lexer->position] == '.')) {
        if (buf_pos < MAX_TOKEN_LEN - 1) {
            buffer[buf_pos++] = lexer->input[lexer->position];
        }
        lexer->position++;
        lexer->column++;
    }
    buffer[buf_pos] = '\0';
    add_token(lexer, TOKEN_NUMBER, buffer);
}

void read_string(struct Lexer *lexer) {
    char buffer[MAX_TOKEN_LEN] = {0};
    size_t buf_pos = 0;
    
    lexer->position++;
    lexer->column++;
    
    while (lexer->position < lexer->input_len && lexer->input[lexer->position] != '"') {
        if (buf_pos < MAX_TOKEN_LEN - 1) {
            buffer[buf_pos++] = lexer->input[lexer->position];
        }
        lexer->position++;
        lexer->column++;
    }
    
    if (lexer->position < lexer->input_len && lexer->input[lexer->position] == '"') {
        lexer->position++;
        lexer->column++;
    }
    
    buffer[buf_pos] = '\0';
    add_token(lexer, TOKEN_STRING, buffer);
}

void tokenize(struct Lexer *lexer) {
    if (lexer == NULL) return;
    
    while (lexer->position < lexer->input_len && lexer->token_count < MAX_TOKENS) {
        skip_whitespace(lexer);
        if (lexer->position >= lexer->input_len) break;
        
        char current = lexer->input[lexer->position];
        
        if (isalpha(current) || current == '_')