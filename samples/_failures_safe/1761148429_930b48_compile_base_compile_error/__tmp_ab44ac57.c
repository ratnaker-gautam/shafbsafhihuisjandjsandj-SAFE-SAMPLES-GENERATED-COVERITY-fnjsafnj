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
    TOKEN_ERROR
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

void handle_error(Token* token) {
    if (token == NULL) return;
    printf("Error: Invalid token '%s' at line %d, column %d\n", token->value, token->line, token->column);
}

void init_lexer(Lexer* lexer, const char* input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->current.type = TOKEN_ERROR;
    lexer->current.value[0] = '\0';
    lexer->current.line = 1;
    lexer->current.column = 1;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

void next_token(Lexer* lexer) {
    if (lexer == NULL || lexer->input == NULL) return;
    
    while (isspace((unsigned char)lexer->input[lexer->position])) {
        if (lexer->input[lexer->position] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
    }
    
    if (lexer->input[lexer->position] == '\0') {
        lexer->current.type = TOKEN_EOF;
        strcpy(lexer->current.value, "");
        lexer->current.line = lexer->line;
        lexer->current.column = lexer->column;
        return;
    }
    
    int start_pos = lexer->position;
    int start_line = lexer->line;
    int start_col = lexer->column;
    
    char current = lexer->input[lexer->position];
    
    if (isdigit((unsigned char)current)) {
        int i = 0;
        while (isdigit((unsigned char)lexer->input[lexer->position]) && i < 31) {
            lexer->current.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        lexer->current.value[i] = '\0';
        lexer->current.type = TOKEN_NUMBER;
    } else if (isalpha((unsigned char)current) || current == '_') {
        int i = 0;
        while ((isalnum((unsigned char)lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') && i < 31) {
            lexer->current.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        lexer->current.value[i] = '\0';
        lexer->current.type = TOKEN_IDENTIFIER;
    } else if (is_operator_char(current)) {
        int i = 0;
        while (is_operator_char(lexer->input[lexer->position]) && i < 31) {
            lexer->current.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        lexer->current.value[i] = '\0';
        lexer->current.type = TOKEN_OPERATOR;
    } else if (current == '"') {
        int i = 0;
        lexer->position++;
        lexer->column++;
        while (lexer->input[lexer->position] != '"' && lexer->input[lexer->position] != '\0' && i < 31) {
            lexer->current.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        lexer->current.value[i] = '\0';
        if (lexer->input[lexer->position] == '"') {
            lexer->position++;