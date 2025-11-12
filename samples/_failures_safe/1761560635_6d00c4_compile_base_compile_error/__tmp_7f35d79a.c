//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 1000
#define MAX_TOKEN_LEN 256

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
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
    const char* input;
    int position;
    int line;
    int column;
    Token tokens[MAX_TOKENS];
    int token_count;
} Lexer;

void init_lexer(Lexer* lexer, const char* input) {
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->token_count = 0;
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int get_operator_length(const char* str) {
    if (str[0] == '\0') return 0;
    if (str[1] == '\0') return 1;
    if ((str[0] == '=' && str[1] == '=') ||
        (str[0] == '!' && str[1] == '=') ||
        (str[0] == '<' && str[1] == '=') ||
        (str[0] == '>' && str[1] == '=') ||
        (str[0] == '&' && str[1] == '&') ||
        (str[0] == '|' && str[1] == '|')) {
        return 2;
    }
    return 1;
}

void add_token(Lexer* lexer, TokenType type, const char* value, int line, int column) {
    if (lexer->token_count >= MAX_TOKENS) return;
    Token* token = &lexer->tokens[lexer->token_count];
    token->type = type;
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
    token->line = line;
    token->column = column;
    lexer->token_count++;
}

void skip_whitespace(Lexer* lexer) {
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

int tokenize_identifier(Lexer* lexer) {
    int start = lexer->position;
    int line = lexer->line;
    int column = lexer->column;
    
    while (lexer->input[lexer->position] != '\0' && (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_')) {
        lexer->position++;
        lexer->column++;
    }
    
    int length = lexer->position - start;
    if (length >= MAX_TOKEN_LEN) length = MAX_TOKEN_LEN - 1;
    
    char value[MAX_TOKEN_LEN];
    strncpy(value, &lexer->input[start], length);
    value[length] = '\0';
    
    add_token(lexer, TOKEN_IDENTIFIER, value, line, column);
    return 1;
}

int tokenize_number(Lexer* lexer) {
    int start = lexer->position;
    int line = lexer->line;
    int column = lexer->column;
    
    while (lexer->input[lexer->position] != '\0' && isdigit(lexer->input[lexer->position])) {
        lexer->position++;
        lexer->column++;
    }
    
    int length = lexer->position - start;
    if (length >= MAX_TOKEN_LEN) length = MAX_TOKEN_LEN - 1;
    
    char value[MAX_TOKEN_LEN];
    strncpy(value, &lexer->input[start], length);
    value[length] = '\0';
    
    add_token(lexer, TOKEN_NUMBER, value, line, column);
    return 1;
}

int tokenize_string(Lexer* lexer) {
    if (lexer->input[lexer->position] != '"') return 0;
    
    int start = lexer->position;
    int line = lexer->line;
    int column = lexer->column;
    
    lexer->position++;
    lexer->column++;
    
    while (lexer->input[lexer->position] != '\0' && lexer->input[lexer->position] != '"') {
        if (lexer->input[lexer->position] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
    }
    
    if (lexer->input[lexer->position] == '"') {
        lexer->position++;
        lexer->column++;
    }
    
    int length = lexer->position -