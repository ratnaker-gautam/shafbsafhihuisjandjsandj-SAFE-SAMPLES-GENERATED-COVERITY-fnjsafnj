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

int is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

void add_token(Lexer* lexer, TokenType type, const char* value) {
    if (lexer->token_count >= MAX_TOKENS) return;
    
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
    return lexer->input[lexer->position];
}

char advance(Lexer* lexer) {
    char c = lexer->input[lexer->position++];
    if (c == '\n') {
        lexer->line++;
        lexer->column = 1;
    } else {
        lexer->column++;
    }
    return c;
}

void tokenize_identifier(Lexer* lexer) {
    char buffer[MAX_TOKEN_LEN];
    int pos = 0;
    
    while (isalnum(peek(lexer))) {
        if (pos < MAX_TOKEN_LEN - 1) {
            buffer[pos++] = advance(lexer);
        } else {
            advance(lexer);
        }
    }
    buffer[pos] = '\0';
    add_token(lexer, TOKEN_IDENTIFIER, buffer);
}

void tokenize_number(Lexer* lexer) {
    char buffer[MAX_TOKEN_LEN];
    int pos = 0;
    
    while (isdigit(peek(lexer))) {
        if (pos < MAX_TOKEN_LEN - 1) {
            buffer[pos++] = advance(lexer);
        } else {
            advance(lexer);
        }
    }
    buffer[pos] = '\0';
    add_token(lexer, TOKEN_NUMBER, buffer);
}

void tokenize_operator(Lexer* lexer) {
    char buffer[3] = {0};
    buffer[0] = advance(lexer);
    
    if (is_operator_char(peek(lexer))) {
        buffer[1] = advance(lexer);
    }
    
    add_token(lexer, TOKEN_OPERATOR, buffer);
}

void tokenize_string(Lexer* lexer) {
    char buffer[MAX_TOKEN_LEN];
    int pos = 0;
    
    advance(lexer);
    
    while (peek(lexer) != '"' && peek(lexer) != '\0') {
        if (pos < MAX_TOKEN_LEN - 1) {
            buffer[pos++] = advance(lexer);
        } else {
            advance(lexer);
        }
    }
    
    if (peek(lexer) == '"') {
        advance(lexer);
    }
    
    buffer[pos] = '\0';
    add_token(lexer, TOKEN_STRING, buffer);
}

int tokenize(Lexer* lexer) {
    while (peek(lexer) != '\0') {
        char current = peek(lexer);
        
        if (is_whitespace(current)) {
            advance(lexer);
            continue;
        }
        
        if (isalpha(current)) {
            tokenize_identifier(lexer);
        } else if (isdigit(current)) {
            tokenize_number(lexer);
        } else if (is_operator_char(current)) {
            tokenize_operator(lexer);
        } else if (current == '"') {
            tokenize_string(lexer);
        } else {
            advance(lexer);
        }
    }
    
    add_token(lexer, TOKEN_EOF, "");
    return lexer->token_count;
}

void print_tokens(Lexer* lexer) {
    const char* type_names[] = {
        "IDENTIFIER", "NUMBER", "OPERATOR", "STRING", "EOF"
    };
    
    for (int i = 0; i < lexer->token_count; i++) {
        Token* token = &lexer->tokens[i];
        printf("Line %d, Column %d: %s '%s'\n", 
               token->line,