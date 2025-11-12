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
    TOKEN_EOF
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
    if (token == NULL) return;
    printf("Number: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void handle_identifier(const Token* token) {
    if (token == NULL) return;
    printf("Identifier: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void handle_operator(const Token* token) {
    if (token == NULL) return;
    printf("Operator: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void handle_string(const Token* token) {
    if (token == NULL) return;
    printf("String: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void skip_whitespace(Lexer* lexer) {
    if (lexer == NULL || lexer->input == NULL) return;
    
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

int read_token(Lexer* lexer, Token* token) {
    if (lexer == NULL || token == NULL || lexer->input == NULL) return 0;
    
    skip_whitespace(lexer);
    
    if (lexer->input[lexer->position] == '\0') {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }
    
    char current = lexer->input[lexer->position];
    token->line = lexer->line;
    token->column = lexer->column;
    
    if (isdigit(current)) {
        token->type = TOKEN_NUMBER;
        int i = 0;
        while (i < 31 && isdigit(lexer->input[lexer->position])) {
            token->value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token->value[i] = '\0';
    } else if (isalpha(current) || current == '_') {
        token->type = TOKEN_IDENTIFIER;
        int i = 0;
        while (i < 31 && (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_')) {
            token->value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token->value[i] = '\0';
    } else if (is_operator_char(current)) {
        token->type = TOKEN_OPERATOR;
        int i = 0;
        while (i < 31 && is_operator_char(lexer->input[lexer->position])) {
            token->value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token->value[i] = '\0';
    } else if (current == '"') {
        token->type = TOKEN_STRING;
        int i = 0;
        lexer->position++;
        lexer->column++;
        while (i < 31 && lexer->input[lexer->position] != '"' && lexer->input[lexer->position] != '\0') {
            token->value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        if (lexer->input[lexer->position] == '"') {
            lexer->position++;
            lexer->column++;
        }
        token->value[i] = '\0';
    } else {
        return 0;
    }
    
    return 1;
}

void process_tokens(const char* input, TokenHandler handlers[5]) {
    if (input == NULL || handlers == NULL) {
        return;
    }
    
    Lexer lexer;
    lexer.input = input;
    lexer.position = 0;
    lexer.line = 1;
    lexer.column = 1;
    
    Token token;
    
    while (read_token(&lexer, &token)) {
        if (token.type == TOKEN_EOF) {
            break;
        }
        
        if (token.type >= 0 && token.type < 5 && handlers[token.type] !=