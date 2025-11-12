//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

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
} Token;

typedef struct {
    const char* input;
    size_t pos;
    size_t len;
} Lexer;

typedef void (*TokenHandler)(Token*);

void handle_number(Token* token) {
    if (token) {
        printf("Found number: %s\n", token->value);
    }
}

void handle_identifier(Token* token) {
    if (token) {
        printf("Found identifier: %s\n", token->value);
    }
}

void handle_operator(Token* token) {
    if (token) {
        printf("Found operator: %s\n", token->value);
    }
}

void handle_string(Token* token) {
    if (token) {
        printf("Found string: %s\n", token->value);
    }
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

void skip_whitespace(Lexer* lexer) {
    if (lexer) {
        while (lexer->pos < lexer->len && isspace(lexer->input[lexer->pos])) {
            lexer->pos++;
        }
    }
}

int lex_number(Lexer* lexer, Token* token) {
    if (!lexer || !token) return 0;
    size_t start = lexer->pos;
    while (lexer->pos < lexer->len && isdigit(lexer->input[lexer->pos])) {
        lexer->pos++;
    }
    if (lexer->pos > start) {
        size_t len = lexer->pos - start;
        if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
        strncpy(token->value, lexer->input + start, len);
        token->value[len] = '\0';
        token->type = TOKEN_NUMBER;
        return 1;
    }
    return 0;
}

int lex_identifier(Lexer* lexer, Token* token) {
    if (!lexer || !token) return 0;
    size_t start = lexer->pos;
    if (lexer->pos < lexer->len && isalpha(lexer->input[lexer->pos])) {
        lexer->pos++;
        while (lexer->pos < lexer->len && (isalnum(lexer->input[lexer->pos]) || lexer->input[lexer->pos] == '_')) {
            lexer->pos++;
        }
        size_t len = lexer->pos - start;
        if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
        strncpy(token->value, lexer->input + start, len);
        token->value[len] = '\0';
        token->type = TOKEN_IDENTIFIER;
        return 1;
    }
    return 0;
}

int lex_operator(Lexer* lexer, Token* token) {
    if (!lexer || !token) return 0;
    if (lexer->pos < lexer->len && is_operator_char(lexer->input[lexer->pos])) {
        size_t start = lexer->pos;
        lexer->pos++;
        while (lexer->pos < lexer->len && is_operator_char(lexer->input[lexer->pos])) {
            lexer->pos++;
        }
        size_t len = lexer->pos - start;
        if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
        strncpy(token->value, lexer->input + start, len);
        token->value[len] = '\0';
        token->type = TOKEN_OPERATOR;
        return 1;
    }
    return 0;
}

int lex_string(Lexer* lexer, Token* token) {
    if (!lexer || !token) return 0;
    if (lexer->pos < lexer->len && lexer->input[lexer->pos] == '"') {
        size_t start = lexer->pos;
        lexer->pos++;
        while (lexer->pos < lexer->len && lexer->input[lexer->pos] != '"') {
            lexer->pos++;
        }
        if (lexer->pos < lexer->len && lexer->input[lexer->pos] == '"') {
            lexer->pos++;
            size_t len = lexer->pos - start;
            if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
            strncpy(token->value, lexer->input + start, len);
            token->value[len] = '\0';
            token->type = TOKEN_STRING;
            return 1;
        }
        lexer->pos = start;
    }
    return 0;
}

int next_token(Lexer* lexer, Token* token) {
    if (!lexer || !token) return 0;
    skip_whitespace(lexer);
    if (lexer->pos >= lexer->len) {
        token->type = TO