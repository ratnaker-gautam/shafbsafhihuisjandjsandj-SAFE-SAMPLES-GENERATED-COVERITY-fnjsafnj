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
    if (lexer->position < 0) return '\0';
    return lexer->input[lexer->position];
}

char advance(Lexer* lexer) {
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

void skip_whitespace(Lexer* lexer) {
    char c = peek(lexer);
    while (c != '\0' && isspace(c)) {
        advance(lexer);
        c = peek(lexer);
    }
}

void read_number(Lexer* lexer) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    
    char c = peek(lexer);
    while (c != '\0' && isdigit(c)) {
        if (i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = advance(lexer);
        } else {
            advance(lexer);
        }
        c = peek(lexer);
    }
    
    if (c == '.' && i < MAX_TOKEN_LEN - 1) {
        buffer[i++] = advance(lexer);
        c = peek(lexer);
        while (c != '\0' && isdigit(c)) {
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

void read_identifier(Lexer* lexer) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    
    char c = peek(lexer);
    while (c != '\0' && isalnum(c)) {
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

void read_operator(Lexer* lexer) {
    char buffer[3] = {0};
    buffer[0] = advance(lexer);
    
    char c = peek(lexer);
    if (c != '\0' && is_operator_char(c)) {
        buffer[1] = advance(lexer);
    }
    
    add_token(lexer, TOKEN_OPERATOR, buffer);
}

void read_string(Lexer* lexer) {
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
    add_token(lexer, TOKEN_STRING, buffer);
}

int tokenize(Lexer* lexer) {
    if (lexer == NULL || lexer->input == NULL) return 0;
    
    while (peek(lexer) != '\0') {
        skip_whitespace(lexer);
        
        char c = peek(lexer);
        if (c == '\0') break;
        
        if (isdigit(c)) {
            read