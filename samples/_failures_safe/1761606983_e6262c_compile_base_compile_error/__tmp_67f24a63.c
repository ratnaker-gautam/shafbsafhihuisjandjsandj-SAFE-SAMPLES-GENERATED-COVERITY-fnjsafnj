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

int is_punctuation_char(char c) {
    return strchr("(){}[],;:", c) != NULL;
}

void add_token(Lexer *lexer, TokenType type, const char *value) {
    if (lexer->token_count >= MAX_TOKENS) return;
    
    Token *token = &lexer->tokens[lexer->token_count];
    token->type = type;
    token->line = lexer->line;
    token->column = lexer->column;
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
    lexer->token_count++;
}

void skip_whitespace(Lexer *lexer) {
    while (lexer->input[lexer->position] != '\0' && 
           isspace(lexer->input[lexer->position])) {
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
    while (isdigit(lexer->input[lexer->position])) {
        lexer->position++;
        lexer->column++;
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

int parse_identifier(Lexer *lexer) {
    int start = lexer->position;
    if (isalpha(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') {
        lexer->position++;
        lexer->column++;
        while (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') {
            lexer->position++;
            lexer->column++;
        }
        
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

int parse_string(Lexer *lexer) {
    if (lexer->input[lexer->position] == '"') {
        int start = lexer->position;
        lexer->position++;
        lexer->column++;
        
        while (lexer->input[lexer->position] != '\0' && 
               lexer->input[lexer->position] != '"') {
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
            
            char str[MAX_TOKEN_LEN];
            int len = lexer->position - start;
            if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
            strncpy(str, &lexer->input[start], len);
            str[len] = '\0';
            add_token(lexer, TOKEN_STRING, str);
            return 1;
        }
    }
    return 0;
}

int parse_operator(Lexer *lexer) {
    if (is_operator_char(lexer->input[lexer->position])) {
        int start = lexer->position;
        lexer->position++;
        lexer->column++;
        
        if (is_operator_char(lexer->input[lexer->position])) {
            lexer->position++;
            lexer->