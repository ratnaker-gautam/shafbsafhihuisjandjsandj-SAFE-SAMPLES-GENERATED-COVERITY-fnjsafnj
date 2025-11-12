//DeepSeek-V3 SAFE v1.3 Category: Safe ; Style: io_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_LEN 256
#define MAX_TOKENS 1000

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_OPERATOR,
    TOKEN_PUNCTUATION,
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
    char *input;
    int pos;
    int line;
    int column;
    Token tokens[MAX_TOKENS];
    int token_count;
} Lexer;

void init_lexer(Lexer *lexer, char *input) {
    lexer->input = input;
    lexer->pos = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->token_count = 0;
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int is_punctuation_char(char c) {
    return strchr("(),;{}[]", c) != NULL;
}

void add_token(Lexer *lexer, TokenType type, char *value) {
    if (lexer->token_count >= MAX_TOKENS) {
        return;
    }
    Token *token = &lexer->tokens[lexer->token_count];
    token->type = type;
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
    token->line = lexer->line;
    token->column = lexer->column;
    lexer->token_count++;
}

char peek(Lexer *lexer) {
    return lexer->input[lexer->pos];
}

char advance(Lexer *lexer) {
    char c = lexer->input[lexer->pos];
    if (c == '\n') {
        lexer->line++;
        lexer->column = 1;
    } else {
        lexer->column++;
    }
    lexer->pos++;
    return c;
}

void skip_whitespace(Lexer *lexer) {
    while (isspace(peek(lexer))) {
        advance(lexer);
    }
}

void read_identifier(Lexer *lexer) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    while (isalnum(peek(lexer)) {
        if (i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = advance(lexer);
        } else {
            advance(lexer);
        }
    }
    buffer[i] = '\0';
    add_token(lexer, TOKEN_IDENTIFIER, buffer);
}

void read_number(Lexer *lexer) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    while (isdigit(peek(lexer))) {
        if (i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = advance(lexer);
        } else {
            advance(lexer);
        }
    }
    buffer[i] = '\0';
    add_token(lexer, TOKEN_NUMBER, buffer);
}

void read_string(Lexer *lexer) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    advance(lexer);
    while (peek(lexer) != '"' && peek(lexer) != '\0') {
        if (i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = advance(lexer);
        } else {
            advance(lexer);
        }
    }
    buffer[i] = '\0';
    if (peek(lexer) == '"') {
        advance(lexer);
        add_token(lexer, TOKEN_STRING, buffer);
    }
}

void read_operator(Lexer *lexer) {
    char buffer[3] = {0};
    buffer[0] = advance(lexer);
    if (is_operator_char(peek(lexer))) {
        buffer[1] = advance(lexer);
    }
    add_token(lexer, TOKEN_OPERATOR, buffer);
}

void tokenize(Lexer *lexer) {
    while (peek(lexer) != '\0') {
        skip_whitespace(lexer);
        char c = peek(lexer);
        if (c == '\0') break;
        
        if (isalpha(c)) {
            read_identifier(lexer);
        } else if (isdigit(c)) {
            read_number(lexer);
        } else if (c == '"') {
            read_string(lexer);
        } else if (is_operator_char(c)) {
            read_operator(lexer);
        } else if (is_punctuation_char(c)) {
            char punct[2] = {advance(lexer), '\