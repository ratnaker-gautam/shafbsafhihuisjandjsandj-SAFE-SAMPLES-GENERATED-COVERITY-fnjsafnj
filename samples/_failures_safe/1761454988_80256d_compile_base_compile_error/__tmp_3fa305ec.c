//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_ERROR
};

struct Token {
    enum TokenType type;
    char value[32];
    int line;
    int column;
};

struct Lexer {
    const char *input;
    int position;
    int line;
    int column;
};

void lexer_init(struct Lexer *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
}

int lexer_peek(struct Lexer *lexer) {
    if (lexer == NULL || lexer->input == NULL) return -1;
    if (lexer->position < 0) return -1;
    return lexer->input[lexer->position];
}

int lexer_advance(struct Lexer *lexer) {
    if (lexer == NULL || lexer->input == NULL) return -1;
    if (lexer->position < 0) return -1;
    int c = lexer_peek(lexer);
    if (c == '\n') {
        lexer->line++;
        lexer->column = 1;
    } else {
        lexer->column++;
    }
    lexer->position++;
    return c;
}

void token_init(struct Token *token, enum TokenType type, const char *value, int line, int column) {
    if (token == NULL || value == NULL) return;
    token->type = type;
    strncpy(token->value, value, sizeof(token->value) - 1);
    token->value[sizeof(token->value) - 1] = '\0';
    token->line = line;
    token->column = column;
}

int is_operator(int c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

struct Token lexer_next(struct Lexer *lexer) {
    struct Token token;
    token_init(&token, TOKEN_ERROR, "", lexer->line, lexer->column);
    
    if (lexer == NULL || lexer->input == NULL) {
        return token;
    }
    
    while (1) {
        int c = lexer_peek(lexer);
        if (c == -1) {
            token_init(&token, TOKEN_EOF, "EOF", lexer->line, lexer->column);
            return token;
        }
        
        if (isspace(c)) {
            lexer_advance(lexer);
            continue;
        }
        
        if (isdigit(c)) {
            char buffer[32] = {0};
            int i = 0;
            int start_line = lexer->line;
            int start_col = lexer->column;
            
            while (isdigit(c) && i < sizeof(buffer) - 1) {
                buffer[i++] = c;
                lexer_advance(lexer);
                c = lexer_peek(lexer);
            }
            buffer[i] = '\0';
            token_init(&token, TOKEN_NUMBER, buffer, start_line, start_col);
            return token;
        }
        
        if (isalpha(c) || c == '_') {
            char buffer[32] = {0};
            int i = 0;
            int start_line = lexer->line;
            int start_col = lexer->column;
            
            while ((isalnum(c) || c == '_') && i < sizeof(buffer) - 1) {
                buffer[i++] = c;
                lexer_advance(lexer);
                c = lexer_peek(lexer);
            }
            buffer[i] = '\0';
            token_init(&token, TOKEN_IDENTIFIER, buffer, start_line, start_col);
            return token;
        }
        
        if (c == '"') {
            char buffer[32] = {0};
            int i = 0;
            int start_line = lexer->line;
            int start_col = lexer->column;
            
            lexer_advance(lexer);
            c = lexer_peek(lexer);
            
            while (c != '"' && c != -1 && i < sizeof(buffer) - 1) {
                buffer[i++] = c;
                lexer_advance(lexer);
                c = lexer_peek(lexer);
            }
            
            if (c == '"') {
                lexer_advance(lexer);
                buffer[i] = '\0';
                token_init(&token, TOKEN_STRING, buffer, start_line, start_col);
            } else {
                token_init(&token, TOKEN_ERROR, "Unterminated string", start_line, start_col);
            }
            return token;
        }
        
        if (is_operator(c)) {
            char buffer[2] = {0};
            int start_line = lexer->line;
            int start_col = lexer->column;
            buffer[0] = c;
            lexer_advance(lexer);
            token_init(&token, TOKEN_OPERATOR, buffer, start_line, start_col);
            return token;
        }
        
        char buffer[2] = {0};