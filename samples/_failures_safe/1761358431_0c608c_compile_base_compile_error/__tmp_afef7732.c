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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int skip_whitespace(Lexer* lexer) {
    int skipped = 0;
    while (lexer->input[lexer->position] != '\0' && 
           isspace(lexer->input[lexer->position])) {
        if (lexer->input[lexer->position] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
        skipped = 1;
    }
    return skipped;
}

int read_number(Lexer* lexer, Token* token) {
    int len = 0;
    
    while (lexer->input[lexer->position] != '\0' && 
           isdigit(lexer->input[lexer->position])) {
        if (len < MAX_TOKEN_LEN - 1) {
            token->value[len++] = lexer->input[lexer->position];
        }
        lexer->position++;
        lexer->column++;
    }
    
    if (len > 0) {
        token->value[len] = '\0';
        token->type = TOKEN_NUMBER;
        token->line = lexer->line;
        token->column = lexer->column - len;
        return 1;
    }
    return 0;
}

int read_identifier(Lexer* lexer, Token* token) {
    int len = 0;
    
    if (lexer->input[lexer->position] != '\0' && 
        (isalpha(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_')) {
        
        while (lexer->input[lexer->position] != '\0' && 
               (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_')) {
            if (len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = lexer->input[lexer->position];
            }
            lexer->position++;
            lexer->column++;
        }
        
        if (len > 0) {
            token->value[len] = '\0';
            token->type = TOKEN_IDENTIFIER;
            token->line = lexer->line;
            token->column = lexer->column - len;
            return 1;
        }
    }
    return 0;
}

int read_operator(Lexer* lexer, Token* token) {
    int len = 0;
    
    if (lexer->input[lexer->position] != '\0' && is_operator_char(lexer->input[lexer->position])) {
        
        while (lexer->input[lexer->position] != '\0' && 
               is_operator_char(lexer->input[lexer->position]) && len < 2) {
            if (len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = lexer->input[lexer->position];
            }
            lexer->position++;
            lexer->column++;
        }
        
        if (len > 0) {
            token->value[len] = '\0';
            token->type = TOKEN_OPERATOR;
            token->line = lexer->line;
            token->column = lexer->column - len;
            return 1;
        }
    }
    return 0;
}

int read_string(Lexer* lexer, Token* token) {
    int len = 0;
    
    if (lexer->input[lexer->position] == '"') {
        lexer->position++;
        lexer->column++;
        
        while (lexer->input[lexer->position] != '\0' && 
               lexer->input[lexer->position] != '"') {
            if (len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = lexer->input[lexer->position];
            }
            lexer->position++;
            lexer->column++;
        }
        
        if (lexer->input[lexer->position] == '"') {
            lexer->position++;
            lexer->column++;
            token->value[len] = '\0';
            token->type = TOKEN_STRING;
            token->line =