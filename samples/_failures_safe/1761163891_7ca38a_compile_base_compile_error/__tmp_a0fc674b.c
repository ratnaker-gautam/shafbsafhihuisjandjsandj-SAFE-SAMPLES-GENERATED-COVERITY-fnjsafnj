//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 1000
#define MAX_TOKEN_LEN 256

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
    int line;
    int column;
} Token;

typedef struct {
    const char *input;
    int position;
    int line;
    int column;
    Token tokens[MAX_TOKENS];
    int token_count;
} Lexer;

void init_lexer(Lexer *lexer, const char *input) {
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->token_count = 0;
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int is_identifier_char(char c) {
    return isalnum(c) || c == '_';
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
    while (lexer->input[lexer->position] != '\0' && isspace(lexer->input[lexer->position])) {
        if (lexer->input[lexer->position] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
    }
}

int parse_number(Lexer *lexer) {
    int start = lexer->position;
    int has_dot = 0;
    
    while (lexer->input[lexer->position] != '\0' && 
           (isdigit(lexer->input[lexer->position]) || lexer->input[lexer->position] == '.')) {
        if (lexer->input[lexer->position] == '.') {
            if (has_dot) break;
            has_dot = 1;
        }
        lexer->position++;
        lexer->column++;
    }
    
    if (lexer->position - start > 0) {
        char number[MAX_TOKEN_LEN];
        size_t len = lexer->position - start;
        if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
        strncpy(number, &lexer->input[start], len);
        number[len] = '\0';
        add_token(lexer, TOKEN_NUMBER, number);
        return 1;
    }
    return 0;
}

int parse_identifier(Lexer *lexer) {
    int start = lexer->position;
    
    while (lexer->input[lexer->position] != '\0' && is_identifier_char(lexer->input[lexer->position])) {
        lexer->position++;
        lexer->column++;
    }
    
    if (lexer->position - start > 0) {
        char identifier[MAX_TOKEN_LEN];
        size_t len = lexer->position - start;
        if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
        strncpy(identifier, &lexer->input[start], len);
        identifier[len] = '\0';
        add_token(lexer, TOKEN_IDENTIFIER, identifier);
        return 1;
    }
    return 0;
}

int parse_operator(Lexer *lexer) {
    if (lexer->input[lexer->position] != '\0' && is_operator(lexer->input[lexer->position])) {
        char op[3] = {lexer->input[lexer->position], '\0'};
        
        if (lexer->input[lexer->position + 1] != '\0' && is_operator(lexer->input[lexer->position + 1])) {
            op[1] = lexer->input[lexer->position + 1];
            op[2] = '\0';
            lexer->position += 2;
            lexer->column += 2;
        } else {
            lexer->position++;
            lexer->column++;
        }
        
        add_token(lexer, TOKEN_OPERATOR, op);
        return 1;
    }
    return 0;
}

int parse_string(Lexer *lexer) {
    if (lexer->input[lexer->position] != '"') return 0;
    
    int start = lexer->position;
    lexer->position++;
    lexer->column++;
    
    while (lexer->input[lexer->position] != '\0'