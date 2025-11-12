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
    TOKEN_PUNCTUATION,
    TOKEN_EOF
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

int is_punctuation_char(char c) {
    return strchr("(){}[],;:", c) != NULL;
}

void add_token(Lexer *lexer, TokenType type, const char *value) {
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
    return lexer->input[lexer->position];
}

char advance(Lexer *lexer) {
    char c = lexer->input[lexer->position];
    if (c == '\n') {
        lexer->line++;
        lexer->column = 1;
    } else {
        lexer->column++;
    }
    lexer->position++;
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
    char c = peek(lexer);
    while (isalnum(c)) {
        if (i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = advance(lexer);
        } else {
            advance(lexer);
        }
        c = peek(lexer);
    }
    buffer[i] = '\0';
    add_token(lexer, TOKEN_IDENTIFIER, buffer);
}

void read_number(Lexer *lexer) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    char c = peek(lexer);
    while (isdigit(c)) {
        if (i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = advance(lexer);
        } else {
            advance(lexer);
        }
        c = peek(lexer);
    }
    buffer[i] = '\0';
    add_token(lexer, TOKEN_NUMBER, buffer);
}

void read_string(Lexer *lexer) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    advance(lexer);
    char c = peek(lexer);
    while (c != '"' && c != '\0') {
        if (i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = advance(lexer);
        } else {
            advance(lexer);
        }
        c = peek(lexer);
    }
    buffer[i] = '\0';
    if (peek(lexer) == '"') {
        advance(lexer);
    }
    add_token(lexer, TOKEN_STRING, buffer);
}

void tokenize(Lexer *lexer) {
    while (peek(lexer) != '\0') {
        skip_whitespace(lexer);
        char current = peek(lexer);
        if (current == '\0') break;
        
        if (isalpha(current)) {
            read_identifier(lexer);
        } else if (isdigit(current)) {
            read_number(lexer);
        } else if (current == '"') {
            read_string(lexer);
        } else if (is_operator_char(current)) {
            char op[3] = {current, '\0', '\0'};
            advance(lexer);
            if (is_operator_char(peek(lexer))) {
                op[1] = peek(lexer);
                op[2] = '\0';
                advance(lexer);
            }
            add_token(lexer, TOKEN_OPERATOR, op);
        } else if (is_punctuation_char(current)) {
            char punct[2] = {current, '\0'};
            advance(lexer);
            add_token(lexer, TOKEN_PUNCTUATION, punct);
        } else {
            advance(lexer);
        }
    }
    add_token(lexer, TOKEN_EOF, "");
}

const char* token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFI