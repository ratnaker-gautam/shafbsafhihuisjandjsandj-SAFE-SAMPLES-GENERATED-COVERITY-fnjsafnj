//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 1000
#define MAX_TOKEN_LEN 100

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
    return strchr("+-*/=<>!&|", c) != NULL;
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
    
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
    
    lexer->token_count++;
}

char peek(Lexer *lexer) {
    if (lexer->position < 0) return '\0';
    if (lexer->input == NULL) return '\0';
    if (lexer->input[lexer->position] == '\0') return '\0';
    return lexer->input[lexer->position];
}

char advance(Lexer *lexer) {
    if (lexer->input == NULL) return '\0';
    if (lexer->position < 0) return '\0';
    
    char c = lexer->input[lexer->position];
    if (c == '\0') return '\0';
    
    if (c == '\n') {
        lexer->line++;
        lexer->column = 1;
    } else {
        lexer->column++;
    }
    lexer->position++;
    return c;
}

void skip_whitespace(Lexer *lexer) {
    if (lexer->input == NULL) return;
    
    while (is_whitespace(peek(lexer))) {
        advance(lexer);
    }
}

void read_number(Lexer *lexer) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    
    while (isdigit((unsigned char)peek(lexer))) {
        if (i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = advance(lexer);
        } else {
            advance(lexer);
        }
    }
    
    if (peek(lexer) == '.' && i < MAX_TOKEN_LEN - 1) {
        buffer[i++] = advance(lexer);
        while (isdigit((unsigned char)peek(lexer))) {
            if (i < MAX_TOKEN_LEN - 1) {
                buffer[i++] = advance(lexer);
            } else {
                advance(lexer);
            }
        }
    }
    
    buffer[i] = '\0';
    add_token(lexer, TOKEN_NUMBER, buffer);
}

void read_identifier(Lexer *lexer) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    
    while (isalnum((unsigned char)peek(lexer))) {
        if (i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = advance(lexer);
        } else {
            advance(lexer);
        }
    }
    
    buffer[i] = '\0';
    add_token(lexer, TOKEN_IDENTIFIER, buffer);
}

void read_operator(Lexer *lexer) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    
    while (is_operator_char(peek(lexer))) {
        if (i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = advance(lexer);
        } else {
            advance(lexer);
        }
    }
    
    buffer[i] = '\0';
    add_token(lexer, TOKEN_OPERATOR, buffer);
}

void read_string(Lexer *lexer) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    
    advance(lexer);
    
    while (peek(lexer) != '"' && peek(lexer) != '\0') {
        if (i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = advance(lexer);
        } else {
            advance(lexer);
        }
    }
    
    buffer[i] = '\0';
    
    if (peek(lexer) == '"') {
        advance(lexer);
        add_token(lexer, TOKEN_STRING, buffer);
    }
}

int tokenize(Lexer *lexer) {
    if (lex