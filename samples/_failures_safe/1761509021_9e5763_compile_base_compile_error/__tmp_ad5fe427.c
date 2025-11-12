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

int is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

void add_token(Lexer* lexer, TokenType type, const char* value) {
    if (lexer->token_count >= MAX_TOKENS) {
        return;
    }
    Token* token = &lexer->tokens[lexer->token_count++];
    token->type = type;
    token->line = lexer->line;
    token->column = lexer->column;
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
}

char peek(Lexer* lexer) {
    if (lexer->input == NULL) return '\0';
    return lexer->input[lexer->position];
}

char advance(Lexer* lexer) {
    if (lexer->input == NULL) return '\0';
    char c = lexer->input[lexer->position];
    if (c == '\0') return '\0';
    lexer->position++;
    if (c == '\n') {
        lexer->line++;
        lexer->column = 1;
    } else {
        lexer->column++;
    }
    return c;
}

void skip_whitespace(Lexer* lexer) {
    if (lexer->input == NULL) return;
    while (is_whitespace(peek(lexer))) {
        advance(lexer);
    }
}

void tokenize_number(Lexer* lexer) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    char current;
    
    current = peek(lexer);
    while (isdigit(current)) {
        if (i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = advance(lexer);
        } else {
            advance(lexer);
        }
        current = peek(lexer);
    }
    
    if (current == '.' && i < MAX_TOKEN_LEN - 1) {
        buffer[i++] = advance(lexer);
        current = peek(lexer);
        while (isdigit(current)) {
            if (i < MAX_TOKEN_LEN - 1) {
                buffer[i++] = advance(lexer);
            } else {
                advance(lexer);
            }
            current = peek(lexer);
        }
    }
    
    buffer[i] = '\0';
    add_token(lexer, TOKEN_NUMBER, buffer);
}

void tokenize_identifier(Lexer* lexer) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    char current;
    
    current = peek(lexer);
    while (isalnum(current)) {
        if (i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = advance(lexer);
        } else {
            advance(lexer);
        }
        current = peek(lexer);
    }
    
    buffer[i] = '\0';
    add_token(lexer, TOKEN_IDENTIFIER, buffer);
}

void tokenize_operator(Lexer* lexer) {
    char buffer[3] = {0};
    buffer[0] = advance(lexer);
    char next_char = peek(lexer);
    
    if (is_operator_char(next_char)) {
        if (buffer[0] == '<' && next_char == '=') {
            buffer[1] = advance(lexer);
        } else if (buffer[0] == '>' && next_char == '=') {
            buffer[1] = advance(lexer);
        } else if (buffer[0] == '=' && next_char == '=') {
            buffer[1] = advance(lexer);
        } else if (buffer[0] == '!' && next_char == '=') {
            buffer[1] = advance(lexer);
        } else if (buffer[0] == '&' && next_char == '&') {
            buffer[1] = advance(lexer);
        } else if (buffer[0] == '|' && next_char == '|') {
            buffer[1] = advance(lexer);
        }
    }
    
    add_token(lexer, TOKEN_OPERATOR