//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_TOKENS 1000
#define MAX_TOKEN_LEN 256

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_PUNCTUATION,
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
    int line;
    int column;
} Token;

typedef struct {
    const char *input;
    size_t pos;
    size_t len;
    int line;
    int column;
    Token tokens[MAX_TOKENS];
    size_t token_count;
} Lexer;

void lexer_init(Lexer *lexer, const char *input) {
    lexer->input = input;
    lexer->pos = 0;
    lexer->len = strlen(input);
    lexer->line = 1;
    lexer->column = 1;
    lexer->token_count = 0;
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int is_punctuation_char(char c) {
    return strchr("(),;{}[]", c) != NULL;
}

void add_token(Lexer *lexer, TokenType type, const char *value) {
    if (lexer->token_count >= MAX_TOKENS) return;
    
    Token *token = &lexer->tokens[lexer->token_count];
    token->type = type;
    token->line = lexer->line;
    token->column = lexer->column;
    
    size_t len = strlen(value);
    if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
    strncpy(token->value, value, len);
    token->value[len] = '\0';
    
    lexer->token_count++;
}

void skip_whitespace(Lexer *lexer) {
    while (lexer->pos < lexer->len && isspace(lexer->input[lexer->pos])) {
        if (lexer->input[lexer->pos] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->pos++;
    }
}

void read_identifier(Lexer *lexer) {
    char buffer[MAX_TOKEN_LEN];
    size_t i = 0;
    
    while (lexer->pos < lexer->len && 
           (isalnum(lexer->input[lexer->pos]) || lexer->input[lexer->pos] == '_')) {
        if (i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = lexer->input[lexer->pos];
        }
        lexer->pos++;
        lexer->column++;
    }
    buffer[i] = '\0';
    add_token(lexer, TOKEN_IDENTIFIER, buffer);
}

void read_number(Lexer *lexer) {
    char buffer[MAX_TOKEN_LEN];
    size_t i = 0;
    
    while (lexer->pos < lexer->len && 
           (isdigit(lexer->input[lexer->pos]) || lexer->input[lexer->pos] == '.')) {
        if (i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = lexer->input[lexer->pos];
        }
        lexer->pos++;
        lexer->column++;
    }
    buffer[i] = '\0';
    add_token(lexer, TOKEN_NUMBER, buffer);
}

void read_string(Lexer *lexer) {
    char buffer[MAX_TOKEN_LEN];
    size_t i = 0;
    
    lexer->pos++;
    lexer->column++;
    
    while (lexer->pos < lexer->len && lexer->input[lexer->pos] != '"') {
        if (i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = lexer->input[lexer->pos];
        }
        lexer->pos++;
        lexer->column++;
    }
    
    if (lexer->pos < lexer->len && lexer->input[lexer->pos] == '"') {
        lexer->pos++;
        lexer->column++;
    }
    
    buffer[i] = '\0';
    add_token(lexer, TOKEN_STRING, buffer);
}

void read_operator(Lexer *lexer) {
    char buffer[3] = {0};
    buffer[0] = lexer->input[lexer->pos];
    
    if (lexer->pos + 1 < lexer->len && is_operator_char(lexer->input[lexer->pos + 1])) {
        buffer[1] = lexer->input[lexer->pos + 1];
        lexer->pos++;
        lexer->column++;
    }
    
    lexer->pos++;
    lexer->column++;
    add_token(lexer, TOKEN_OPERATOR, buffer);
}

int tokenize(Lexer *lexer) {
    while (lexer->pos < lexer->len) {
        skip_whitespace(lexer);
        if (lexer->pos >= lexer->len) break;
        
        char current = lexer->input[lexer->pos];
        
        if