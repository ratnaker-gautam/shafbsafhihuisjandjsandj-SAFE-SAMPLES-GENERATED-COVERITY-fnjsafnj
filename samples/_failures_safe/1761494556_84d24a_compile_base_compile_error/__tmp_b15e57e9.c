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
    const char* input;
    int position;
    int line;
    int column;
} Lexer;

typedef void (*TokenHandler)(Token*);

void handle_number(Token* token) {
    printf("Found number: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void handle_identifier(Token* token) {
    printf("Found identifier: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void handle_operator(Token* token) {
    printf("Found operator: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void handle_string(Token* token) {
    printf("Found string: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void handle_eof(Token* token) {
    printf("End of input reached at line %d, column %d\n", token->line, token->column);
}

void handle_error(Token* token) {
    printf("Lexical error: %s at line %d, column %d\n", token->value, token->line, token->column);
}

TokenHandler get_handler(TokenType type) {
    static TokenHandler handlers[] = {
        handle_number,
        handle_identifier,
        handle_operator,
        handle_string,
        handle_eof,
        handle_error
    };
    
    if (type >= TOKEN_NUMBER && type <= TOKEN_ERROR) {
        return handlers[type];
    }
    return handle_error;
}

void init_lexer(Lexer* lexer, const char* input) {
    if (!lexer || !input) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
}

char peek(Lexer* lexer) {
    if (!lexer || !lexer->input) return '\0';
    return lexer->input[lexer->position];
}

char advance(Lexer* lexer) {
    if (!lexer || !lexer->input) return '\0';
    char c = lexer->input[lexer->position];
    if (c != '\0') {
        lexer->position++;
        if (c == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
    }
    return c;
}

void skip_whitespace(Lexer* lexer) {
    if (!lexer) return;
    while (isspace(peek(lexer))) {
        advance(lexer);
    }
}

int is_operator(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

Token next_token(Lexer* lexer) {
    Token token = {TOKEN_ERROR, "", 0, 0};
    if (!lexer) return token;
    
    skip_whitespace(lexer);
    
    token.line = lexer->line;
    token.column = lexer->column;
    
    char current = peek(lexer);
    if (current == '\0') {
        token.type = TOKEN_EOF;
        strcpy(token.value, "EOF");
        return token;
    }
    
    if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        char next_char;
        while ((next_char = peek(lexer)) != '\0' && isdigit(next_char)) {
            if (i < 31) {
                token.value[i++] = advance(lexer);
            } else {
                advance(lexer);
            }
        }
        token.value[i] = '\0';
    } else if (isalpha(current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        char next_char;
        while ((next_char = peek(lexer)) != '\0' && (isalnum(next_char) || next_char == '_')) {
            if (i < 31) {
                token.value[i++] = advance(lexer);
            } else {
                advance(lexer);
            }
        }
        token.value[i] = '\0';
    } else if (is_operator(current)) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        char next_char;
        while ((next_char = peek(lexer)) != '\0' && is_operator(next_char)) {
            if (i < 31) {
                token.value[i++] = advance(lexer);
            } else {
                advance(lexer);
            }
        }
        token.value[i] = '\0';
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        advance(lexer);
        int i = 0;
        char next_char;
        while ((next_char = peek(lexer)) != '\0' && next_char != '"') {
            if (i < 31) {
                token