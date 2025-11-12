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

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
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

void read_number(Lexer* lexer) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    
    while (lexer->input[lexer->position] != '\0' && i < MAX_TOKEN_LEN - 1) {
        char c = lexer->input[lexer->position];
        if (isdigit(c) || c == '.') {
            buffer[i++] = c;
            lexer->position++;
            lexer->column++;
        } else {
            break;
        }
    }
    buffer[i] = '\0';
    add_token(lexer, TOKEN_NUMBER, buffer);
}

void read_identifier(Lexer* lexer) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    
    while (lexer->input[lexer->position] != '\0' && i < MAX_TOKEN_LEN - 1) {
        char c = lexer->input[lexer->position];
        if (isalnum(c) || c == '_') {
            buffer[i++] = c;
            lexer->position++;
            lexer->column++;
        } else {
            break;
        }
    }
    buffer[i] = '\0';
    add_token(lexer, TOKEN_IDENTIFIER, buffer);
}

void read_string(Lexer* lexer) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    
    lexer->position++;
    lexer->column++;
    
    while (lexer->input[lexer->position] != '\0' && i < MAX_TOKEN_LEN - 1) {
        char c = lexer->input[lexer->position];
        if (c == '"') {
            lexer->position++;
            lexer->column++;
            break;
        }
        buffer[i++] = c;
        lexer->position++;
        lexer->column++;
    }
    buffer[i] = '\0';
    add_token(lexer, TOKEN_STRING, buffer);
}

void read_operator(Lexer* lexer) {
    char buffer[3] = {0};
    buffer[0] = lexer->input[lexer->position];
    lexer->position++;
    lexer->column++;
    
    if (lexer->input[lexer->position] != '\0') {
        char next = lexer->input[lexer->position];
        if ((buffer[0] == '=' && next == '=') ||
            (buffer[0] == '!' && next == '=') ||
            (buffer[0] == '<' && next == '=') ||
            (buffer[0] == '>' && next == '=')) {
            buffer[1] = next;
            lexer->position++;
            lexer->column++;
        }
    }
    add_token(lexer, TOKEN_OPERATOR, buffer);
}

void tokenize(Lexer* lexer) {
    while (lexer->input[lexer->position] != '\0') {
        skip_whitespace(lexer);
        
        if (lexer->input[lexer->position] == '\0') break;
        
        char c = lexer->input[lexer->position