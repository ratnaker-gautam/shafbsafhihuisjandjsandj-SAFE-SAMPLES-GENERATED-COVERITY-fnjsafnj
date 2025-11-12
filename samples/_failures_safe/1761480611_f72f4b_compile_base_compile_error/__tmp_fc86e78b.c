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

void lexer_init(Lexer* lexer, const char* input) {
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->token_count = 0;
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

void add_token(Lexer* lexer, TokenType type, const char* value) {
    if (lexer->token_count >= MAX_TOKENS) return;
    
    Token* token = &lexer->tokens[lexer->token_count++];
    token->type = type;
    token->line = lexer->line;
    token->column = lexer->column;
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
}

void skip_whitespace(Lexer* lexer) {
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

int parse_number(Lexer* lexer) {
    int start = lexer->position;
    int has_dot = 0;
    
    while (lexer->input[lexer->position] != '\0') {
        char c = lexer->input[lexer->position];
        if (isdigit(c)) {
            lexer->position++;
            lexer->column++;
        } else if (c == '.' && !has_dot) {
            has_dot = 1;
            lexer->position++;
            lexer->column++;
        } else {
            break;
        }
    }
    
    if (lexer->position - start > 0) {
        char num[MAX_TOKEN_LEN];
        int len = lexer->position - start;
        if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
        strncpy(num, &lexer->input[start], len);
        num[len] = '\0';
        add_token(lexer, TOKEN_NUMBER, num);
        return 1;
    }
    return 0;
}

int parse_string(Lexer* lexer) {
    if (lexer->input[lexer->position] != '"') return 0;
    
    lexer->position++;
    lexer->column++;
    int start = lexer->position;
    
    while (lexer->input[lexer->position] != '\0' && 
           lexer->input[lexer->position] != '"') {
        lexer->position++;
        lexer->column++;
    }
    
    if (lexer->input[lexer->position] == '"') {
        char str[MAX_TOKEN_LEN];
        int len = lexer->position - start;
        if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
        strncpy(str, &lexer->input[start], len);
        str[len] = '\0';
        add_token(lexer, TOKEN_STRING, str);
        lexer->position++;
        lexer->column++;
        return 1;
    }
    return 0;
}

int parse_identifier(Lexer* lexer) {
    if (!isalpha(lexer->input[lexer->position]) && 
        lexer->input[lexer->position] != '_') return 0;
    
    int start = lexer->position;
    
    while (lexer->input[lexer->position] != '\0' && 
           (isalnum(lexer->input[lexer->position]) || 
            lexer->input[lexer->position] == '_')) {
        lexer->position++;
        lexer->column++;
    }
    
    if (lexer->position - start > 0) {
        char ident[MAX_TOKEN_LEN];
        int len = lexer->position - start;
        if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
        strncpy(ident, &lexer->input[start], len);
        ident[len] = '\0';
        add_token(lexer, TOKEN_IDENTIFIER, ident);
        return 1;
    }
    return 0;
}

int parse_operator(Lexer* lexer) {
    if (!is_operator_char(lexer->input[lexer->position])) return 0;
    
    int start = lexer->position;
    
    while (