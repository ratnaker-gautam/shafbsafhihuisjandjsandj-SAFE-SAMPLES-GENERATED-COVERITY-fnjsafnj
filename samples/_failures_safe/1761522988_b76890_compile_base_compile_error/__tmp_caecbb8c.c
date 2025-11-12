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
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->token_count = 0;
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

void add_token(Lexer* lexer, TokenType type, const char* value) {
    if (lexer == NULL || value == NULL || lexer->token_count >= MAX_TOKENS) return;
    
    Token* token = &lexer->tokens[lexer->token_count];
    token->type = type;
    token->line = lexer->line;
    token->column = lexer->column;
    
    size_t len = strlen(value);
    if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
    strncpy(token->value, value, len);
    token->value[len] = '\0';
    
    lexer->token_count++;
}

char peek(Lexer* lexer) {
    if (lexer == NULL || lexer->input == NULL) return '\0';
    if (lexer->input[lexer->position] == '\0') return '\0';
    return lexer->input[lexer->position];
}

char advance(Lexer* lexer) {
    if (lexer == NULL || lexer->input == NULL) return '\0';
    char c = lexer->input[lexer->position];
    if (c == '\0') return c;
    
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
    if (lexer == NULL) return;
    char c = peek(lexer);
    while (is_whitespace(c)) {
        advance(lexer);
        c = peek(lexer);
    }
}

void read_identifier(Lexer* lexer) {
    if (lexer == NULL) return;
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    
    char c = peek(lexer);
    while (isalnum(c)) {
        if (i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = advance(lexer);
        } else {
            advance(lexer);
        }
        c = peek(lexer);
    }
    buffer[i] = '\0';
    add_token(lexer, TOKEN_IDENTIFIER, buffer);
}

void read_number(Lexer* lexer) {
    if (lexer == NULL) return;
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    int has_dot = 0;
    
    char c = peek(lexer);
    while (isdigit(c)) {
        if (i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = advance(lexer);
        } else {
            advance(lexer);
        }
        c = peek(lexer);
    }
    
    if (c == '.' && isdigit(lexer->input[lexer->position + 1])) {
        if (i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = advance(lexer);
        } else {
            advance(lexer);
        }
        has_dot = 1;
        
        c = peek(lexer);
        while (isdigit(c)) {
            if (i < MAX_TOKEN_LEN - 1) {
                buffer[i++] = advance(lexer);
            } else {
                advance(lexer);
            }
            c = peek(lexer);
        }
    }
    
    buffer[i] = '\0';
    add_token(lexer, TOKEN_NUMBER, buffer);
}

void read_string(Lexer* lexer) {
    if (lexer == NULL) return;
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    
    advance(lexer);
    
    char c = peek(lexer);
    while (c != '"' && c != '\0') {
        if (i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = advance(lexer);
        } else {
            advance(lexer);
        }
        c = peek(lexer);
    }
    
    if (c == '"') {
        advance(lexer);
    }
    
    buffer[i] = '\0';
    add_token(lexer,