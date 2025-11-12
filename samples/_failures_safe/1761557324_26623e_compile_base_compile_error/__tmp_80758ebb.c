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
    const char *input;
    int position;
    int line;
    int column;
    Token current;
} Lexer;

typedef void (*TokenHandler)(Token*);

void handle_number(Token *token) {
    if (token == NULL) return;
    printf("Number: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void handle_identifier(Token *token) {
    if (token == NULL) return;
    printf("Identifier: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void handle_operator(Token *token) {
    if (token == NULL) return;
    printf("Operator: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void handle_string(Token *token) {
    if (token == NULL) return;
    printf("String: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void init_lexer(Lexer *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->current.type = TOKEN_EOF;
    lexer->current.value[0] = '\0';
}

char peek(Lexer *lexer) {
    if (lexer == NULL || lexer->input == NULL) return '\0';
    return lexer->input[lexer->position];
}

char advance(Lexer *lexer) {
    if (lexer == NULL || lexer->input == NULL) return '\0';
    char c = peek(lexer);
    if (c != '\0') {
        lexer->position++;
        lexer->column++;
        if (c == '\n') {
            lexer->line++;
            lexer->column = 1;
        }
    }
    return c;
}

void skip_whitespace(Lexer *lexer) {
    if (lexer == NULL) return;
    while (isspace((unsigned char)peek(lexer))) {
        advance(lexer);
    }
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

Token read_number(Lexer *lexer) {
    Token token = {TOKEN_NUMBER, "", lexer->line, lexer->column};
    int i = 0;
    char c;
    while ((c = peek(lexer)) != '\0' && isdigit((unsigned char)c)) {
        if (i < 31) {
            token.value[i++] = advance(lexer);
        } else {
            advance(lexer);
        }
    }
    token.value[i] = '\0';
    return token;
}

Token read_identifier(Lexer *lexer) {
    Token token = {TOKEN_IDENTIFIER, "", lexer->line, lexer->column};
    int i = 0;
    char c;
    while ((c = peek(lexer)) != '\0' && isalnum((unsigned char)c)) {
        if (i < 31) {
            token.value[i++] = advance(lexer);
        } else {
            advance(lexer);
        }
    }
    token.value[i] = '\0';
    return token;
}

Token read_operator(Lexer *lexer) {
    Token token = {TOKEN_OPERATOR, "", lexer->line, lexer->column};
    int i = 0;
    char c;
    while ((c = peek(lexer)) != '\0' && is_operator(c)) {
        if (i < 31) {
            token.value[i++] = advance(lexer);
        } else {
            advance(lexer);
        }
    }
    token.value[i] = '\0';
    return token;
}

Token read_string(Lexer *lexer) {
    Token token = {TOKEN_STRING, "", lexer->line, lexer->column};
    int i = 0;
    if (peek(lexer) == '"') {
        advance(lexer);
        while (peek(lexer) != '"' && peek(lexer) != '\0') {
            if (i < 31) {
                token.value[i++] = advance(lexer);
            } else {
                advance(lexer);
            }
        }
        if (peek(lexer) == '"') {
            advance(lexer);
        }
    }
    token.value[i] = '\0';
    return token;
}

Token next_token(Lexer *lexer) {
    if (lexer == NULL) {
        Token eof = {TOKEN_EOF, "", 0, 0};
        return eof;
    }
    
    skip_whitespace(lexer);
    
    char c = peek(lexer);
    if