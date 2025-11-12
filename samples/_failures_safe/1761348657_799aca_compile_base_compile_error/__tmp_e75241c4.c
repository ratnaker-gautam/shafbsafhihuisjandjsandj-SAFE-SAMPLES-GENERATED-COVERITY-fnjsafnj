//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 1000
#define MAX_TOKEN_LEN 128

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
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
    return c == '+' || c == '-' || c == '*' || c == '/' || 
           c == '=' || c == '<' || c == '>' || c == '!' ||
           c == '&' || c == '|';
}

int is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
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

char peek(Lexer *lexer) {
    return lexer->input[lexer->position];
}

char advance(Lexer *lexer) {
    char c = lexer->input[lexer->position];
    if (c == '\n') {
        lexer->line++;
        lexer->column = 1;
    } else {
        lexer->column++;
    }
    lexer->position++;
    return c;
}

void tokenize_number(Lexer *lexer) {
    char buffer[MAX_TOKEN_LEN];
    int index = 0;
    
    while (isdigit(peek(lexer)) && index < MAX_TOKEN_LEN - 1) {
        buffer[index++] = advance(lexer);
    }
    
    if (peek(lexer) == '.' && index < MAX_TOKEN_LEN - 1) {
        buffer[index++] = advance(lexer);
        while (isdigit(peek(lexer)) && index < MAX_TOKEN_LEN - 1) {
            buffer[index++] = advance(lexer);
        }
    }
    
    buffer[index] = '\0';
    add_token(lexer, TOKEN_NUMBER, buffer);
}

void tokenize_identifier(Lexer *lexer) {
    char buffer[MAX_TOKEN_LEN];
    int index = 0;
    
    while ((isalnum(peek(lexer)) || peek(lexer) == '_') && index < MAX_TOKEN_LEN - 1) {
        buffer[index++] = advance(lexer);
    }
    
    buffer[index] = '\0';
    add_token(lexer, TOKEN_IDENTIFIER, buffer);
}

void tokenize_operator(Lexer *lexer) {
    char buffer[3] = {0};
    buffer[0] = advance(lexer);
    
    if (is_operator_char(peek(lexer))) {
        buffer[1] = advance(lexer);
    }
    
    add_token(lexer, TOKEN_OPERATOR, buffer);
}

void tokenize_string(Lexer *lexer) {
    char buffer[MAX_TOKEN_LEN];
    int index = 0;
    
    advance(lexer);
    
    while (peek(lexer) != '"' && peek(lexer) != '\0' && index < MAX_TOKEN_LEN - 1) {
        buffer[index++] = advance(lexer);
    }
    
    if (peek(lexer) == '"') {
        advance(lexer);
        buffer[index] = '\0';
        add_token(lexer, TOKEN_STRING, buffer);
    } else {
        add_token(lexer, TOKEN_ERROR, "Unterminated string");
    }
}

int tokenize(Lexer *lexer) {
    while (peek(lexer) != '\0') {
        if (is_whitespace(peek(lexer))) {
            advance(lexer);
            continue;
        }
        
        if (isdigit(peek(lexer))) {
            tokenize_number(lexer);
        } else if (isalpha(peek(lexer)) || peek(lexer) == '_') {
            tokenize_identifier(lexer);
        } else if (is_operator_char(peek(lexer))) {
            tokenize_operator(lexer);
        } else if (peek(lexer) == '"') {
            tokenize_string(lexer);
        } else {
            char unknown[2] = {