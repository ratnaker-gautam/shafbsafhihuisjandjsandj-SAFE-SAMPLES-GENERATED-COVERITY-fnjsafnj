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
    TOKEN_UNKNOWN
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

typedef void (*TokenHandler)(Token);

void handle_number(Token token) {
    printf("Number: %s at line %d, column %d\n", token.value, token.line, token.column);
}

void handle_identifier(Token token) {
    printf("Identifier: %s at line %d, column %d\n", token.value, token.line, token.column);
}

void handle_operator(Token token) {
    printf("Operator: %s at line %d, column %d\n", token.value, token.line, token.column);
}

void handle_string(Token token) {
    printf("String: %s at line %d, column %d\n", token.value, token.line, token.column);
}

void handle_unknown(Token token) {
    printf("Unknown: %s at line %d, column %d\n", token.value, token.line, token.column);
}

void init_lexer(Lexer *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->current.type = TOKEN_UNKNOWN;
    memset(lexer->current.value, 0, sizeof(lexer->current.value));
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

char peek(Lexer *lexer) {
    if (lexer == NULL || lexer->input == NULL) return '\0';
    if (lexer->position >= (int)strlen(lexer->input)) return '\0';
    return lexer->input[lexer->position];
}

void advance(Lexer *lexer) {
    if (lexer == NULL || lexer->input == NULL) return;
    if (lexer->position >= (int)strlen(lexer->input)) return;
    if (lexer->input[lexer->position] == '\n') {
        lexer->line++;
        lexer->column = 1;
    } else {
        lexer->column++;
    }
    lexer->position++;
}

void skip_whitespace(Lexer *lexer) {
    if (lexer == NULL) return;
    while (peek(lexer) != '\0' && isspace(peek(lexer))) {
        advance(lexer);
    }
}

Token get_next_token(Lexer *lexer) {
    if (lexer == NULL) {
        Token invalid = {TOKEN_UNKNOWN, "", 0, 0};
        return invalid;
    }
    
    skip_whitespace(lexer);
    
    char current = peek(lexer);
    if (current == '\0') {
        Token eof = {TOKEN_EOF, "", lexer->line, lexer->column};
        return eof;
    }
    
    Token token;
    token.line = lexer->line;
    token.column = lexer->column;
    memset(token.value, 0, sizeof(token.value));
    
    if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (peek(lexer) != '\0' && isdigit(peek(lexer)) && i < 31) {
            token.value[i++] = peek(lexer);
            advance(lexer);
        }
        token.value[i] = '\0';
    } else if (isalpha(current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (peek(lexer) != '\0' && (isalnum(peek(lexer)) || peek(lexer) == '_') && i < 31) {
            token.value[i++] = peek(lexer);
            advance(lexer);
        }
        token.value[i] = '\0';
    } else if (is_operator(current)) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (peek(lexer) != '\0' && is_operator(peek(lexer)) && i < 31) {
            token.value[i++] = peek(lexer);
            advance(lexer);
        }
        token.value[i] = '\0';
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        advance(lexer);
        int i = 0;
        while (peek(lexer) != '\0' && peek(lexer) != '"' && i < 31) {
            token.value[i++] = peek(lexer);
            advance(lexer);
        }
        token.value[i] = '\0';
        if (peek(lexer) == '"') {
            advance(lexer);
        }
    } else {