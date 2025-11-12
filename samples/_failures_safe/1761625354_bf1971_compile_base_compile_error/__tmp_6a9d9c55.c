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
} Lexer;

typedef void (*TokenHandler)(const Token*);

void handle_number(const Token* token) {
    printf("Found number: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void handle_identifier(const Token* token) {
    printf("Found identifier: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void handle_operator(const Token* token) {
    printf("Found operator: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void handle_string(const Token* token) {
    printf("Found string: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void handle_error(const Token* token) {
    printf("Error: Invalid token '%s' at line %d, column %d\n", token->value, token->line, token->column);
}

void skip_whitespace(Lexer* lexer) {
    while (lexer->input[lexer->position] != '\0' && isspace(lexer->input[lexer->position])) {
        if (lexer->input[lexer->position] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
    }
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

Token get_next_token(Lexer* lexer) {
    Token token = {TOKEN_ERROR, "", lexer->line, lexer->column};
    
    skip_whitespace(lexer);
    
    if (lexer->input[lexer->position] == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char current = lexer->input[lexer->position];
    
    if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (lexer->input[lexer->position] != '\0' && isdigit(lexer->input[lexer->position]) && i < 31) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
    } else if (isalpha(current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (lexer->input[lexer->position] != '\0' && (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') && i < 31) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (lexer->input[lexer->position] != '\0' && is_operator_char(lexer->input[lexer->position]) && i < 31) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        token.value[0] = '"';
        int i = 1;
        lexer->position++;
        lexer->column++;
        
        while (lexer->input[lexer->position] != '\0' && lexer->input[lexer->position] != '"' && i < 30) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        
        if (lexer->input[lexer->position] == '"') {
            token.value[i++] = '"';
            lexer->position++;
            lexer->column++;
            token.value[i] = '\0';
        } else {
            token.type = TOKEN_ERROR;
        }
    } else {
        token.value[0] = current;
        token.value[1] = '\0';
        lexer->position++;
        lexer->column++;
    }
    
    return token;
}

void process_tokens(const char* input, TokenHandler handlers[]) {
    if (input == NULL || handlers == NULL) {
        return;
    }
    
    Lexer lexer = {input, 0, 1, 1};
    
    while (1) {
        Token token = get_next_token(&lexer);
        
        if (token.type == TOKEN_EOF) {
            break;
        }
        
        if (token.type >= TOKEN