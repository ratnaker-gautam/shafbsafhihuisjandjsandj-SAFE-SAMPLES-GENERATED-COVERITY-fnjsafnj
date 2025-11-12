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
    Token* token = &lexer->tokens[lexer->token_count];
    token->type = type;
    token->line = lexer->line;
    token->column = lexer->column;
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
    lexer->token_count++;
}

void tokenize_number(Lexer* lexer) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    
    while (isdigit(lexer->input[lexer->position]) && i < MAX_TOKEN_LEN - 1) {
        buffer[i++] = lexer->input[lexer->position++];
        lexer->column++;
    }
    buffer[i] = '\0';
    add_token(lexer, TOKEN_NUMBER, buffer);
}

void tokenize_identifier(Lexer* lexer) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    
    while ((isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') && 
           i < MAX_TOKEN_LEN - 1) {
        buffer[i++] = lexer->input[lexer->position++];
        lexer->column++;
    }
    buffer[i] = '\0';
    add_token(lexer, TOKEN_IDENTIFIER, buffer);
}

void tokenize_string(Lexer* lexer) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    
    lexer->position++;
    lexer->column++;
    
    while (lexer->input[lexer->position] != '"' && lexer->input[lexer->position] != '\0' && 
           i < MAX_TOKEN_LEN - 1) {
        buffer[i++] = lexer->input[lexer->position++];
        lexer->column++;
    }
    
    if (lexer->input[lexer->position] == '"') {
        lexer->position++;
        lexer->column++;
    }
    
    buffer[i] = '\0';
    add_token(lexer, TOKEN_STRING, buffer);
}

void tokenize_operator(Lexer* lexer) {
    char buffer[3] = {0};
    buffer[0] = lexer->input[lexer->position];
    lexer->position++;
    lexer->column++;
    
    if (is_operator_char(lexer->input[lexer->position])) {
        buffer[1] = lexer->input[lexer->position];
        lexer->position++;
        lexer->column++;
    }
    
    add_token(lexer, TOKEN_OPERATOR, buffer);
}

void tokenize(Lexer* lexer) {
    while (lexer->input[lexer->position] != '\0' && lexer->token_count < MAX_TOKENS) {
        char current = lexer->input[lexer->position];
        
        if (is_whitespace(current)) {
            if (current == '\n') {
                lexer->line++;
                lexer->column = 1;
            } else {
                lexer->column++;
            }
            lexer->position++;
            continue;
        }
        
        if (isdigit(current)) {
            tokenize_number(lexer);
        } else if (isalpha(current) || current == '_') {
            tokenize_identifier(lexer);
        } else if (current == '"') {
            tokenize_string(lexer);
        } else if (is_operator_char(current)) {
            tokenize_operator(lexer);
        } else {
            lexer->position++;
            lexer->column++;
        }
    }
    
    add_token(lexer, TOKEN_EOF, "");
}

void print_tokens(const Lexer* lexer) {
    const char* type_names[] = {
        "NUMBER", "IDENTIFIER", "