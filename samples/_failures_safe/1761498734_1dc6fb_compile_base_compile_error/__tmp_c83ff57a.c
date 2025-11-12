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

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int is_punctuation_char(char c) {
    return strchr("(){}[],;:", c) != NULL;
}

void init_lexer(Lexer *lexer, const char *input) {
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->token_count = 0;
}

char peek(Lexer *lexer) {
    if (lexer->position < 0) return '\0';
    if (lexer->input == NULL) return '\0';
    return lexer->input[lexer->position];
}

char advance(Lexer *lexer) {
    if (lexer->input == NULL) return '\0';
    if (lexer->input[lexer->position] == '\0') return '\0';
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

void add_token(Lexer *lexer, TokenType type, const char *value) {
    if (lexer->token_count >= MAX_TOKENS) {
        return;
    }
    Token *token = &lexer->tokens[lexer->token_count];
    token->type = type;
    token->line = lexer->line;
    size_t val_len = strlen(value);
    if (val_len > 0) {
        token->column = lexer->column - (int)val_len;
    } else {
        token->column = lexer->column;
    }
    if (token->column < 1) token->column = 1;
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
    lexer->token_count++;
}

void tokenize_identifier(Lexer *lexer) {
    char buffer[MAX_TOKEN_LEN] = {0};
    int i = 0;
    
    while ((isalnum((unsigned char)peek(lexer)) || peek(lexer) == '_') && peek(lexer) != '\0') {
        if (i >= MAX_TOKEN_LEN - 1) break;
        buffer[i++] = advance(lexer);
    }
    buffer[i] = '\0';
    add_token(lexer, TOKEN_IDENTIFIER, buffer);
}

void tokenize_number(Lexer *lexer) {
    char buffer[MAX_TOKEN_LEN] = {0};
    int i = 0;
    
    while (isdigit((unsigned char)peek(lexer)) && peek(lexer) != '\0') {
        if (i >= MAX_TOKEN_LEN - 1) break;
        buffer[i++] = advance(lexer);
    }
    buffer[i] = '\0';
    add_token(lexer, TOKEN_NUMBER, buffer);
}

void tokenize_string(Lexer *lexer) {
    char buffer[MAX_TOKEN_LEN] = {0};
    int i = 0;
    char quote = advance(lexer);
    
    while (peek(lexer) != quote && peek(lexer) != '\0') {
        if (i >= MAX_TOKEN_LEN - 1) break;
        buffer[i++] = advance(lexer);
    }
    buffer[i] = '\0';
    
    if (peek(lexer) == quote) {
        advance(lexer);
        add_token(lexer, TOKEN_STRING, buffer);
    }
}

void tokenize_operator(Lexer *lexer) {
    char buffer[3] = {0};
    buffer[0] = advance(lexer);
    
    if (is_operator_char(peek(lexer)) && peek(lexer) != '\0') {
        buffer[1] = advance(lexer);
    }
    
    add_token(lexer, TOKEN_OPERATOR, buffer);
}

void tokenize_punctuation(Lexer *lexer) {
    char buffer[2] = {0};
    buffer[0] = advance(lexer);
    add_token(lexer, TOKEN_PUNCTUATION, buffer);
}

int tokenize(Lexer *lexer) {
    if (lexer == NULL || lexer->input == NULL) return 0;
    
    while (peek(lexer) != '\0') {
        if (lexer->token_count >= MAX_TOKENS) break;
        
        char c = peek(lexer);
        
        if (