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

typedef void (*TokenHandler)(const Token*);

void handle_number(const Token *token) {
    printf("Number: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void handle_identifier(const Token *token) {
    printf("Identifier: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void handle_operator(const Token *token) {
    printf("Operator: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void handle_string(const Token *token) {
    printf("String: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void handle_error(const Token *token) {
    printf("Error: Invalid token '%s' at line %d, column %d\n", token->value, token->line, token->column);
}

void skip_whitespace(Lexer *lexer) {
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

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

Token get_next_token(Lexer *lexer) {
    Token token = {TOKEN_ERROR, "", lexer->line, lexer->column};
    
    skip_whitespace(lexer);
    
    if (lexer->input[lexer->position] == '\0') {
        token.type = TOKEN_EOF;
        strcpy(token.value, "EOF");
        return token;
    }
    
    char current = lexer->input[lexer->position];
    
    if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (isdigit(lexer->input[lexer->position]) && i < 31) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
    } else if (isalpha(current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while ((isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') && i < 31) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (is_operator_char(lexer->input[lexer->position]) && i < 31) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        token.value[0] = '"';
        lexer->position++;
        lexer->column++;
        int i = 1;
        while (lexer->input[lexer->position] != '"' && lexer->input[lexer->position] != '\0' && i < 30) {
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
        token.value[0] = lexer->input[lexer->position++];
        token.value[1] = '\0';
        lexer->column++;
    }
    
    return token;
}

void process_tokens(const char *input, TokenHandler handlers[]) {
    Lexer lexer = {input, 0, 1, 1};
    
    while (1) {
        Token token = get_next_token(&lexer);
        
        if (token.type == TOKEN_EOF) {
            break;
        }
        
        if (token.type >= TOKEN_NUMBER && token.type <= TOKEN_ERROR) {
            handlers[token.type](&token);
        }
        
        if (token.type == TOKEN_ERROR) {
            break;
        }
    }
}