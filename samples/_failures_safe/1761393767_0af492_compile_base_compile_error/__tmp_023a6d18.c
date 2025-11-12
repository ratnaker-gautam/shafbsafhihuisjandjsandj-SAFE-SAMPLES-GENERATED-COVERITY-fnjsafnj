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
    if (token) {
        printf("Number: %s at line %d, column %d\n", token->value, token->line, token->column);
    }
}

void handle_identifier(Token* token) {
    if (token) {
        printf("Identifier: %s at line %d, column %d\n", token->value, token->line, token->column);
    }
}

void handle_operator(Token* token) {
    if (token) {
        printf("Operator: %s at line %d, column %d\n", token->value, token->line, token->column);
    }
}

void handle_string(Token* token) {
    if (token) {
        printf("String: %s at line %d, column %d\n", token->value, token->line, token->column);
    }
}

void handle_error(Token* token) {
    if (token) {
        printf("Error: Invalid token '%s' at line %d, column %d\n", token->value, token->line, token->column);
    }
}

TokenHandler get_handler(TokenType type) {
    static TokenHandler handlers[] = {
        handle_number,
        handle_identifier,
        handle_operator,
        handle_string,
        NULL,
        handle_error
    };
    if (type >= 0 && type <= TOKEN_ERROR) {
        return handlers[type];
    }
    return handle_error;
}

void init_lexer(Lexer* lexer, const char* input) {
    if (lexer && input) {
        lexer->input = input;
        lexer->position = 0;
        lexer->line = 1;
        lexer->column = 1;
    }
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

char peek(Lexer* lexer) {
    if (!lexer || !lexer->input) return '\0';
    size_t len = strlen(lexer->input);
    if (lexer->position < 0 || lexer->position >= (int)len) return '\0';
    return lexer->input[lexer->position];
}

char advance(Lexer* lexer) {
    if (!lexer || !lexer->input) return '\0';
    size_t len = strlen(lexer->input);
    if (lexer->position < 0 || lexer->position >= (int)len) return '\0';
    char c = lexer->input[lexer->position++];
    if (c == '\n') {
        lexer->line++;
        lexer->column = 1;
    } else {
        lexer->column++;
    }
    return c;
}

void skip_whitespace(Lexer* lexer) {
    if (!lexer) return;
    while (isspace((unsigned char)peek(lexer))) {
        advance(lexer);
    }
}

Token next_token(Lexer* lexer) {
    Token token = {TOKEN_ERROR, "", 0, 0};
    if (!lexer) return token;
    
    skip_whitespace(lexer);
    
    int start_line = lexer->line;
    int start_col = lexer->column;
    
    char c = peek(lexer);
    if (c == '\0') {
        token.type = TOKEN_EOF;
        strncpy(token.value, "EOF", sizeof(token.value)-1);
        token.value[sizeof(token.value)-1] = '\0';
    } else if (isdigit((unsigned char)c)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (isdigit((unsigned char)peek(lexer)) && i < (int)sizeof(token.value)-1) {
            token.value[i++] = advance(lexer);
        }
        token.value[i] = '\0';
    } else if (isalpha((unsigned char)c) || c == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while ((isalnum((unsigned char)peek(lexer)) || peek(lexer) == '_') && i < (int)sizeof(token.value)-1) {
            token.value[i++] = advance(lexer);
        }
        token.value[i] = '\0';
    } else if (is_operator(c)) {
        token.type = TOKEN_OPERATOR;
        token.value[0] = advance(lexer);
        token.value[1] = '\0';
    } else if (c == '"') {
        token.type = TOKEN_STRING;
        advance(lexer);
        int i = 0;
        while (peek(lexer) != '"' && peek(lexer) != '\0' && i < (int)sizeof(token.value)-1) {
            token.value