//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 1000
#define MAX_TOKEN_LEN 128

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_ERROR
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
    return strchr("+-*/=<>!&|", c) != NULL;
}

void skip_whitespace(Lexer *lexer) {
    while (lexer->input[lexer->position] != '\0') {
        char c = lexer->input[lexer->position];
        if (c == ' ' || c == '\t') {
            lexer->position++;
            lexer->column++;
        } else if (c == '\n') {
            lexer->position++;
            lexer->line++;
            lexer->column = 1;
        } else {
            break;
        }
    }
}

int add_token(Lexer *lexer, TokenType type, const char *value) {
    if (lexer->token_count >= MAX_TOKENS) {
        return 0;
    }
    
    Token *token = &lexer->tokens[lexer->token_count];
    token->type = type;
    token->line = lexer->line;
    token->column = lexer->column;
    
    size_t len = strlen(value);
    if (len >= MAX_TOKEN_LEN) {
        len = MAX_TOKEN_LEN - 1;
    }
    strncpy(token->value, value, len);
    token->value[len] = '\0';
    
    lexer->token_count++;
    return 1;
}

int scan_identifier(Lexer *lexer) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    
    while (isalnum((unsigned char)lexer->input[lexer->position]) || 
           lexer->input[lexer->position] == '_') {
        if (i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = lexer->input[lexer->position];
        }
        lexer->position++;
        lexer->column++;
    }
    buffer[i] = '\0';
    
    return add_token(lexer, TOKEN_IDENTIFIER, buffer);
}

int scan_number(Lexer *lexer) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    int has_dot = 0;
    
    while (isdigit((unsigned char)lexer->input[lexer->position]) || 
           (!has_dot && lexer->input[lexer->position] == '.')) {
        if (lexer->input[lexer->position] == '.') {
            has_dot = 1;
        }
        if (i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = lexer->input[lexer->position];
        }
        lexer->position++;
        lexer->column++;
    }
    buffer[i] = '\0';
    
    return add_token(lexer, TOKEN_NUMBER, buffer);
}

int scan_operator(Lexer *lexer) {
    char buffer[3] = {0};
    buffer[0] = lexer->input[lexer->position];
    lexer->position++;
    lexer->column++;
    
    if (is_operator_char(lexer->input[lexer->position]) && 
        lexer->position < (int)strlen(lexer->input)) {
        buffer[1] = lexer->input[lexer->position];
        lexer->position++;
        lexer->column++;
    }
    
    return add_token(lexer, TOKEN_OPERATOR, buffer);
}

int scan_string(Lexer *lexer) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    
    lexer->position++;
    lexer->column++;
    
    while (lexer->input[lexer->position] != '\0' && 
           lexer->input[lexer->position] != '"') {
        if (i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = lexer->input[lexer->position];
        }
        lexer->position++;
        lexer->column++;
        
        if (lexer->input[lexer->position] == '\0') {
            return add_token(lexer, TOKEN_ERROR, "Unterminated string");
        }
    }
    
    if (lexer->input[lexer->position] == '"') {
        lexer->position++;
        lexer->column++;
    }
    
    buffer[i] = '\0';
    return add_token(lexer, TOKEN_STRING, buffer);
}

int tokenize(Lexer *lexer) {
    while (lexer->input[lexer->position] !=