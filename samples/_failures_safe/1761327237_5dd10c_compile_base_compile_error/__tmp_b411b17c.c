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

void init_lexer(Lexer *lexer, const char *input) {
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->token_count = 0;
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
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

int read_string(Lexer *lexer, Token *token) {
    int start = lexer->position;
    lexer->position++;
    lexer->column++;
    
    while (lexer->input[lexer->position] != '\0') {
        if (lexer->input[lexer->position] == '"') {
            int length = lexer->position - start - 1;
            if (length >= MAX_TOKEN_LEN) {
                return 0;
            }
            strncpy(token->value, lexer->input + start + 1, length);
            token->value[length] = '\0';
            token->type = TOKEN_STRING;
            lexer->position++;
            lexer->column++;
            return 1;
        }
        lexer->position++;
        lexer->column++;
    }
    return 0;
}

int read_number(Lexer *lexer, Token *token) {
    int start = lexer->position;
    int has_dot = 0;
    
    while (lexer->input[lexer->position] != '\0') {
        char c = lexer->input[lexer->position];
        if (isdigit(c)) {
            lexer->position++;
            lexer->column++;
        } else if (c == '.' && !has_dot) {
            has_dot = 1;
            lexer->position++;
            lexer->column++;
        } else {
            break;
        }
    }
    
    int length = lexer->position - start;
    if (length >= MAX_TOKEN_LEN) {
        return 0;
    }
    strncpy(token->value, lexer->input + start, length);
    token->value[length] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int read_identifier(Lexer *lexer, Token *token) {
    int start = lexer->position;
    
    while (lexer->input[lexer->position] != '\0') {
        char c = lexer->input[lexer->position];
        if (isalnum(c) || c == '_') {
            lexer->position++;
            lexer->column++;
        } else {
            break;
        }
    }
    
    int length = lexer->position - start;
    if (length >= MAX_TOKEN_LEN) {
        return 0;
    }
    strncpy(token->value, lexer->input + start, length);
    token->value[length] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int read_operator(Lexer *lexer, Token *token) {
    int start = lexer->position;
    
    while (lexer->input[lexer->position] != '\0') {
        char c = lexer->input[lexer->position];
        if (is_operator_char(c)) {
            lexer->position++;
            lexer->column++;
        } else {
            break;
        }
    }
    
    int length = lexer->position - start;
    if (length >= MAX_TOKEN_LEN) {
        return 0;
    }
    strncpy(token->value, lexer->input + start, length);
    token->value[length] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

int tokenize(Lexer *lexer) {
    while (lexer->input[lexer->position] != '\0') {
        skip_whitespace(lexer);
        
        if (lexer->input[lexer->position] == '\0') {
            break;
        }
        
        if (lexer->token_count >= MAX_TOKENS) {
            return 0;
        }
        
        Token *token = &lexer->tokens[lexer->token_count];
        token->line = lexer->line;
        token