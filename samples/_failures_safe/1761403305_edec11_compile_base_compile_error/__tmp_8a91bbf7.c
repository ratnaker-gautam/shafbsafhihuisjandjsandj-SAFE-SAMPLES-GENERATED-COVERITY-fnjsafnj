//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[32];
    int line;
    int column;
} Token;

typedef struct {
    const char* input;
    int position;
    int line;
    int column;
    Token current;
} Lexer;

typedef void (*TokenHandler)(Token*);

void handle_number(Token* token) {
    if (token == NULL) return;
    printf("Number: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void handle_identifier(Token* token) {
    if (token == NULL) return;
    printf("Identifier: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void handle_operator(Token* token) {
    if (token == NULL) return;
    printf("Operator: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void handle_string(Token* token) {
    if (token == NULL) return;
    printf("String: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void handle_unknown(Token* token) {
    if (token == NULL) return;
    printf("Unknown: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void init_lexer(Lexer* lexer, const char* input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->current.type = TOKEN_UNKNOWN;
    lexer->current.value[0] = '\0';
    lexer->current.line = 1;
    lexer->current.column = 1;
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

Token get_next_token(Lexer* lexer) {
    Token token = {TOKEN_UNKNOWN, "", lexer->line, lexer->column};
    if (lexer == NULL || lexer->input == NULL) return token;
    
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
        
        if (isdigit(current)) {
            token.type = TOKEN_NUMBER;
            int i = 0;
            while (isdigit(lexer->input[lexer->position]) && i < 31) {
                token.value[i++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            token.value[i] = '\0';
            token.line = lexer->line;
            token.column = lexer->column - i;
            return token;
        }
        
        if (isalpha(current) || current == '_') {
            token.type = TOKEN_IDENTIFIER;
            int i = 0;
            while ((isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') && i < 31) {
                token.value[i++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            token.value[i] = '\0';
            token.line = lexer->line;
            token.column = lexer->column - i;
            return token;
        }
        
        if (is_operator(current)) {
            token.type = TOKEN_OPERATOR;
            int i = 0;
            while (is_operator(lexer->input[lexer->position]) && i < 31) {
                token.value[i++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            token.value[i] = '\0';
            token.line = lexer->line;
            token.column = lexer->column - i;
            return token;
        }
        
        if (current == '"') {
            token.type = TOKEN_STRING;
            int i = 0;
            lexer->position++;
            lexer->column++;
            while (lexer->input[lexer->position] != '"' && lexer->input[lexer->position] != '\0' && i < 31) {
                token.value[i++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            token.value[i] = '\0';
            if (lexer->input[lexer->position] == '"') {
                lexer->position++;
                lexer->column++;
            }
            token.line = lexer->line;
            token.column = lexer->column - i - 1;
            return token;
        }
        
        token.type =