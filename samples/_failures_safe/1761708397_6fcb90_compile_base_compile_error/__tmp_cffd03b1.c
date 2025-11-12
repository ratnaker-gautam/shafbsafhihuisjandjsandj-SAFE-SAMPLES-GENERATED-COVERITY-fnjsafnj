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

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

void add_token(Lexer *lexer, TokenType type, const char *value) {
    if (lexer->token_count >= MAX_TOKENS) return;
    
    Token *token = &lexer->tokens[lexer->token_count];
    token->type = type;
    token->line = lexer->line;
    token->column = lexer->column;
    
    if (value) {
        strncpy(token->value, value, MAX_TOKEN_LEN - 1);
        token->value[MAX_TOKEN_LEN - 1] = '\0';
    } else {
        token->value[0] = '\0';
    }
    
    lexer->token_count++;
}

void skip_whitespace(Lexer *lexer) {
    while (lexer->input[lexer->position] && isspace(lexer->input[lexer->position])) {
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
    while (lexer->input[lexer->position] && isdigit(lexer->input[lexer->position])) {
        lexer->position++;
        lexer->column++;
    }
    
    if (lexer->position - start >= MAX_TOKEN_LEN) return 0;
    
    char number[MAX_TOKEN_LEN];
    strncpy(number, &lexer->input[start], lexer->position - start);
    number[lexer->position - start] = '\0';
    add_token(lexer, TOKEN_NUMBER, number);
    return 1;
}

int parse_identifier(Lexer *lexer) {
    int start = lexer->position;
    while (lexer->input[lexer->position] && 
           (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_')) {
        lexer->position++;
        lexer->column++;
    }
    
    if (lexer->position - start >= MAX_TOKEN_LEN) return 0;
    
    char identifier[MAX_TOKEN_LEN];
    strncpy(identifier, &lexer->input[start], lexer->position - start);
    identifier[lexer->position - start] = '\0';
    add_token(lexer, TOKEN_IDENTIFIER, identifier);
    return 1;
}

int parse_string(Lexer *lexer) {
    if (lexer->input[lexer->position] != '"') return 0;
    
    lexer->position++;
    lexer->column++;
    int start = lexer->position;
    
    while (lexer->input[lexer->position] && lexer->input[lexer->position] != '"') {
        if (lexer->input[lexer->position] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
    }
    
    if (!lexer->input[lexer->position]) return 0;
    
    if (lexer->position - start >= MAX_TOKEN_LEN) return 0;
    
    char string[MAX_TOKEN_LEN];
    strncpy(string, &lexer->input[start], lexer->position - start);
    string[lexer->position - start] = '\0';
    add_token(lexer, TOKEN_STRING, string);
    
    lexer->position++;
    lexer->column++;
    return 1;
}

int parse_operator(Lexer *lexer) {
    int start = lexer->position;
    while (lexer->input[lexer->position] && is_operator_char(lexer->input[lexer->position])) {
        lexer->position++;
        lexer->column++;
    }
    
    if (lexer->position - start >= MAX_TOKEN_LEN) return 0;
    
    char op[MAX_TOKEN_LEN];
    strncpy(op, &lexer->input[start], lexer->position - start);
    op[lexer->position - start] = '\0