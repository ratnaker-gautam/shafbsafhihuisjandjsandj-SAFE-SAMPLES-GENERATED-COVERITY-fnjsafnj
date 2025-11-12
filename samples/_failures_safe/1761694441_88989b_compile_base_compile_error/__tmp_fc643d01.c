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
    const char *input;
    int position;
    int line;
    int column;
} Lexer;

typedef void (*TokenHandler)(Token *);

void handle_number(Token *token) {
    printf("Number: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void handle_identifier(Token *token) {
    printf("Identifier: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void handle_operator(Token *token) {
    printf("Operator: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void handle_string(Token *token) {
    printf("String: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void handle_error(Token *token) {
    printf("Error: Invalid token '%s' at line %d, column %d\n", token->value, token->line, token->column);
}

void skip_whitespace(Lexer *lexer) {
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

Token get_next_token(Lexer *lexer) {
    Token token = {TOKEN_ERROR, "", lexer->line, lexer->column};
    skip_whitespace(lexer);
    
    if (lexer->input[lexer->position] == '\0') {
        token.type = TOKEN_EOF;
        strncpy(token.value, "EOF", sizeof(token.value) - 1);
        token.value[sizeof(token.value) - 1] = '\0';
        return token;
    }
    
    char current = lexer->input[lexer->position];
    
    if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (lexer->input[lexer->position] != '\0' && i < (int)sizeof(token.value) - 1 && 
               (isdigit(lexer->input[lexer->position]) || lexer->input[lexer->position] == '.')) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
    } else if (isalpha(current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (lexer->input[lexer->position] != '\0' && i < (int)sizeof(token.value) - 1 && 
               (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_')) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
    } else if (strchr("+-*/=<>!&|", current) != NULL) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (lexer->input[lexer->position] != '\0' && i < (int)sizeof(token.value) - 1 && 
               strchr("+-*/=<>!&|", lexer->input[lexer->position]) != NULL) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        int i = 0;
        lexer->position++;
        lexer->column++;
        while (lexer->input[lexer->position] != '\0' && i < (int)sizeof(token.value) - 1 && 
               lexer->input[lexer->position] != '"') {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
        if (lexer->input[lexer->position] == '"') {
            lexer->position++;
            lexer->column++;
        } else {
            token.type = TOKEN_ERROR;
        }
    } else {
        token.value[0] = lexer->input[lexer->position++];
        token.value[1] = '\0';
        lexer->column++;
    }
    
    return token;
}

void process_tokens(const char *input, TokenHandler handlers[]) {
    Lexer lexer = {input, 0, 1, 1};
    Token token;
    
    do {
        token = get_next_token(&lexer);
        if (token.type >= 0 && token.type