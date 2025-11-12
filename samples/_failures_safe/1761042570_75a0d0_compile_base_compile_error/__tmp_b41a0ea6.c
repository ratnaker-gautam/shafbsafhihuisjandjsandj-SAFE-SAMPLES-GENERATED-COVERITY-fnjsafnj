//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: parser_tokenizer
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
    if (lexer == NULL || value == NULL || lexer->token_count >= MAX_TOKENS) {
        return;
    }
    Token* token = &lexer->tokens[lexer->token_count];
    token->type = type;
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
    token->line = lexer->line;
    token->column = lexer->column;
    lexer->token_count++;
}

char peek(Lexer* lexer) {
    if (lexer == NULL || lexer->input == NULL || lexer->position < 0) {
        return '\0';
    }
    return lexer->input[lexer->position];
}

char advance(Lexer* lexer) {
    if (lexer == NULL || lexer->input == NULL || lexer->position < 0) {
        return '\0';
    }
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

void tokenize_identifier(Lexer* lexer) {
    if (lexer == NULL) return;
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    char current;
    
    while ((current = peek(lexer)) != '\0' && isalnum(current)) {
        if (i >= MAX_TOKEN_LEN - 1) break;
        buffer[i++] = advance(lexer);
    }
    buffer[i] = '\0';
    add_token(lexer, TOKEN_IDENTIFIER, buffer);
}

void tokenize_number(Lexer* lexer) {
    if (lexer == NULL) return;
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    char current;
    
    while ((current = peek(lexer)) != '\0' && isdigit(current)) {
        if (i >= MAX_TOKEN_LEN - 1) break;
        buffer[i++] = advance(lexer);
    }
    buffer[i] = '\0';
    add_token(lexer, TOKEN_NUMBER, buffer);
}

void tokenize_operator(Lexer* lexer) {
    if (lexer == NULL) return;
    char buffer[3] = {0};
    buffer[0] = advance(lexer);
    
    if (is_operator_char(peek(lexer))) {
        buffer[1] = advance(lexer);
    }
    
    add_token(lexer, TOKEN_OPERATOR, buffer);
}

void tokenize_string(Lexer* lexer) {
    if (lexer == NULL) return;
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    advance(lexer);
    char current;
    
    while ((current = peek(lexer)) != '\0' && current != '"') {
        if (i >= MAX_TOKEN_LEN - 1) break;
        buffer[i++] = advance(lexer);
    }
    
    if (peek(lexer) == '"') {
        advance(lexer);
    }
    buffer[i] = '\0';
    add_token(lexer, TOKEN_STRING, buffer);
}

void tokenize(Lexer* lexer) {
    if (lexer == NULL) return;
    char current;
    while ((current = peek(lexer)) != '\0') {
        if (is_whitespace(current)) {
            advance(lexer);
            continue;
        }
        
        if (isalpha(current)) {
            tokenize_identifier(lexer);
        } else if (isdigit(current)) {
            tokenize_number(lexer);
        } else if (current == '"') {
            tokenize_string(lexer);
        } else if (is_operator_char(current)) {
            tokenize_operator(lexer);
        } else {
            advance(lexer);
        }
    }
    add_token(lexer, TOKEN_EOF, "");
}

const char