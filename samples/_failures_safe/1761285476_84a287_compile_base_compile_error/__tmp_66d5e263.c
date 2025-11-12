//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 1000
#define MAX_TOKEN_LEN 100

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

int read_next_token(Lexer* lexer) {
    if (lexer == NULL || lexer->input == NULL) return 0;
    if (lexer->token_count >= MAX_TOKENS - 1) return 0;
    
    while (lexer->input[lexer->position] != '\0') {
        char current = lexer->input[lexer->position];
        
        if (isspace(current)) {
            if (current == '\n') {
                lexer->line++;
                lexer->column = 1;
            } else {
                lexer->column++;
            }
            lexer->position++;
            continue;
        }
        
        Token* token = &lexer->tokens[lexer->token_count];
        token->line = lexer->line;
        token->column = lexer->column;
        
        if (isalpha(current) || current == '_') {
            int len = 0;
            while ((isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') && len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            token->value[len] = '\0';
            token->type = TOKEN_IDENTIFIER;
            lexer->token_count++;
            return 1;
        }
        
        if (isdigit(current)) {
            int len = 0;
            while (isdigit(lexer->input[lexer->position]) && len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            token->value[len] = '\0';
            token->type = TOKEN_NUMBER;
            lexer->token_count++;
            return 1;
        }
        
        if (current == '"') {
            int len = 0;
            lexer->position++;
            lexer->column++;
            while (lexer->input[lexer->position] != '"' && lexer->input[lexer->position] != '\0' && len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            token->value[len] = '\0';
            if (lexer->input[lexer->position] == '"') {
                lexer->position++;
                lexer->column++;
            }
            token->type = TOKEN_STRING;
            lexer->token_count++;
            return 1;
        }
        
        if (is_operator_char(current)) {
            int len = 0;
            while (is_operator_char(lexer->input[lexer->position]) && len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            token->value[len] = '\0';
            token->type = TOKEN_OPERATOR;
            lexer->token_count++;
            return 1;
        }
        
        lexer->position++;
        lexer->column++;
    }
    
    Token* token = &lexer->tokens[lexer->token_count];
    token->type = TOKEN_EOF;
    token->value[0] = '\0';
    token->line = lexer->line;
    token->column = lexer->column;
    lexer->token_count++;
    return 1;
}

void tokenize_input(Lexer* lexer) {
    if (lexer == NULL) return;
    while (read_next_token(lexer)) {
        if (lexer->tokens[lexer->token_count - 1].type == TOKEN_EOF) {
            break;
        }
    }
}

const char* token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

void print_tokens(Lexer* lexer) {
    if (lexer == NULL) return;
    for (