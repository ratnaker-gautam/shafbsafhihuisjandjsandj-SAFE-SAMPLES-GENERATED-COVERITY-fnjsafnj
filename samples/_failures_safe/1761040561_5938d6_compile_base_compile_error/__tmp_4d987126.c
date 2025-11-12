//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: parser_tokenizer
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
    return strchr("+-*/=<>!&|", c) != NULL;
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

int read_identifier(Lexer* lexer, Token* token) {
    int start = lexer->position;
    while (isalnum((unsigned char)lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') {
        lexer->position++;
        lexer->column++;
    }
    int len = lexer->position - start;
    if (len >= MAX_TOKEN_LEN) {
        return 0;
    }
    strncpy(token->value, lexer->input + start, len);
    token->value[len] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int read_number(Lexer* lexer, Token* token) {
    int start = lexer->position;
    while (isdigit((unsigned char)lexer->input[lexer->position])) {
        lexer->position++;
        lexer->column++;
    }
    int len = lexer->position - start;
    if (len >= MAX_TOKEN_LEN) {
        return 0;
    }
    strncpy(token->value, lexer->input + start, len);
    token->value[len] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int read_operator(Lexer* lexer, Token* token) {
    int start = lexer->position;
    while (is_operator_char(lexer->input[lexer->position])) {
        lexer->position++;
        lexer->column++;
    }
    int len = lexer->position - start;
    if (len >= MAX_TOKEN_LEN) {
        return 0;
    }
    strncpy(token->value, lexer->input + start, len);
    token->value[len] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

int read_string(Lexer* lexer, Token* token) {
    if (lexer->input[lexer->position] != '"') {
        return 0;
    }
    lexer->position++;
    lexer->column++;
    int start = lexer->position;
    while (lexer->input[lexer->position] != '\0' && lexer->input[lexer->position] != '"') {
        if (lexer->input[lexer->position] == '\n') {
            return 0;
        }
        lexer->position++;
        lexer->column++;
    }
    if (lexer->input[lexer->position] != '"') {
        return 0;
    }
    int len = lexer->position - start;
    if (len >= MAX_TOKEN_LEN) {
        return 0;
    }
    strncpy(token->value, lexer->input + start, len);
    token->value[len] = '\0';
    token->type = TOKEN_STRING;
    lexer->position++;
    lexer->column++;
    return 1;
}

int next_token(Lexer* lexer, Token* token) {
    skip_whitespace(lexer);
    if (lexer->input[lexer->position] == '\0') {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }
    token->line = lexer->line;
    token->column = lexer->column;
    char c = lexer->input[lexer->position];
    if (isalpha((unsigned char)c) || c == '_') {
        return read_identifier(lexer, token);
    } else if (isdigit((unsigned char)c)) {
        return read_number(lexer, token);
    } else if (is_operator_char(c)) {
        return read_operator(lexer, token);
    } else if (c == '"') {
        return read_string(lexer,