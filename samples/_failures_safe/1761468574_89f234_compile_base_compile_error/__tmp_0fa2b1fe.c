//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

enum TokenType {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
    TOKEN_KEYWORD,
    TOKEN_EOF,
    TOKEN_UNKNOWN
};

struct Token {
    enum TokenType type;
    char *value;
    size_t line;
    size_t column;
};

struct LexerState {
    const char *input;
    size_t position;
    size_t line;
    size_t column;
    size_t input_length;
};

struct ParserState {
    struct Token *tokens;
    size_t token_count;
    size_t current_token;
};

struct ASTNode {
    char *type;
    char *value;
    struct ASTNode **children;
    size_t child_count;
};

int is_keyword(const char *str) {
    static const char *keywords[] = {"if", "else", "while", "for", "return", "int", "float", "char", "void", NULL};
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void init_lexer(struct LexerState *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->input_length = strlen(input);
}

char peek_char(struct LexerState *lexer) {
    if (lexer == NULL || lexer->position >= lexer->input_length) return '\0';
    return lexer->input[lexer->position];
}

char next_char(struct LexerState *lexer) {
    if (lexer == NULL || lexer->position >= lexer->input_length) return '\0';
    char c = lexer->input[lexer->position++];
    if (c == '\n') {
        lexer->line++;
        lexer->column = 1;
    } else {
        lexer->column++;
    }
    return c;
}

void skip_whitespace(struct LexerState *lexer) {
    if (lexer == NULL) return;
    while (lexer->position < lexer->input_length && isspace(peek_char(lexer))) {
        next_char(lexer);
    }
}

struct Token *create_token(enum TokenType type, const char *value, size_t line, size_t column) {
    if (value == NULL) return NULL;
    struct Token *token = malloc(sizeof(struct Token));
    if (token == NULL) return NULL;
    token->type = type;
    token->value = malloc(strlen(value) + 1);
    if (token->value == NULL) {
        free(token);
        return NULL;
    }
    strcpy(token->value, value);
    token->line = line;
    token->column = column;
    return token;
}

void free_token(struct Token *token) {
    if (token == NULL) return;
    free(token->value);
    free(token);
}

struct Token *next_token(struct LexerState *lexer) {
    if (lexer == NULL) return NULL;
    
    skip_whitespace(lexer);
    if (lexer->position >= lexer->input_length) {
        return create_token(TOKEN_EOF, "", lexer->line, lexer->column);
    }
    
    char c = peek_char(lexer);
    size_t line = lexer->line;
    size_t column = lexer->column;
    
    if (isalpha(c) || c == '_') {
        char buffer[256];
        size_t i = 0;
        while ((isalnum(c) || c == '_') && i < 255) {
            buffer[i++] = next_char(lexer);
            c = peek_char(lexer);
        }
        buffer[i] = '\0';
        enum TokenType type = is_keyword(buffer) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
        return create_token(type, buffer, line, column);
    }
    
    if (isdigit(c)) {
        char buffer[256];
        size_t i = 0;
        while (isdigit(c) && i < 255) {
            buffer[i++] = next_char(lexer);
            c = peek_char(lexer);
        }
        buffer[i] = '\0';
        return create_token(TOKEN_NUMBER, buffer, line, column);
    }
    
    if (c == '"') {
        char buffer[256];
        size_t i = 0;
        next_char(lexer);
        c = peek_char(lexer);
        while (c != '"' && c != '\0' && i < 255) {
            buffer[i++] = next_char(lexer);
            c = peek_char(lexer);
        }
        if (c == '"') next_char(lexer);
        buffer[i] = '\0';
        return create_token(TOKEN_STRING, buffer, line, column);
    }
    
    if (strchr("+-*/=<>!&|", c) != NULL) {
        char buffer[3] = {0};
        buffer[0] = next_char(lexer);
        c = peek_char(lexer);