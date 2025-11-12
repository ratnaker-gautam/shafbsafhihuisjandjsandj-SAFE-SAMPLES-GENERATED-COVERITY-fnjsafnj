//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64
#define MAX_INPUT_LEN 1024

enum TokenType {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_UNKNOWN
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
    int line;
    int column;
};

struct LexerState {
    const char *input;
    size_t position;
    size_t length;
    int line;
    int column;
};

struct ParserState {
    struct Token *tokens;
    size_t token_count;
    size_t current_token;
};

struct ASTNode {
    enum { NODE_BINARY_OP, NODE_NUMBER, NODE_IDENTIFIER, NODE_STRING } type;
    union {
        struct {
            struct ASTNode *left;
            struct ASTNode *right;
            char operator[8];
        } binary_op;
        double number_value;
        char identifier[MAX_TOKEN_LEN];
        char string_value[MAX_TOKEN_LEN];
    } data;
};

void init_lexer(struct LexerState *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->length = strlen(input);
    lexer->line = 1;
    lexer->column = 1;
}

char peek_char(struct LexerState *lexer) {
    if (lexer == NULL || lexer->position >= lexer->length) return '\0';
    return lexer->input[lexer->position];
}

char next_char(struct LexerState *lexer) {
    if (lexer == NULL || lexer->position >= lexer->length) return '\0';
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
    while (lexer->position < lexer->length && isspace(peek_char(lexer))) {
        next_char(lexer);
    }
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int read_operator(struct LexerState *lexer, struct Token *token) {
    if (lexer == NULL || token == NULL) return 0;
    size_t start = lexer->position;
    char buffer[8] = {0};
    int i = 0;
    
    while (i < 7 && lexer->position < lexer->length && is_operator_char(peek_char(lexer))) {
        buffer[i++] = next_char(lexer);
    }
    
    if (i > 0) {
        token->type = TOKEN_OPERATOR;
        strncpy(token->value, buffer, MAX_TOKEN_LEN - 1);
        token->value[MAX_TOKEN_LEN - 1] = '\0';
        return 1;
    }
    return 0;
}

int read_string(struct LexerState *lexer, struct Token *token) {
    if (lexer == NULL || token == NULL) return 0;
    if (peek_char(lexer) != '"') return 0;
    
    next_char(lexer);
    char buffer[MAX_TOKEN_LEN] = {0};
    int i = 0;
    
    while (lexer->position < lexer->length && peek_char(lexer) != '"' && i < MAX_TOKEN_LEN - 1) {
        buffer[i++] = next_char(lexer);
    }
    
    if (peek_char(lexer) == '"') {
        next_char(lexer);
        token->type = TOKEN_STRING;
        strncpy(token->value, buffer, MAX_TOKEN_LEN - 1);
        token->value[MAX_TOKEN_LEN - 1] = '\0';
        return 1;
    }
    return 0;
}

int read_number(struct LexerState *lexer, struct Token *token) {
    if (lexer == NULL || token == NULL) return 0;
    if (!isdigit(peek_char(lexer))) return 0;
    
    char buffer[MAX_TOKEN_LEN] = {0};
    int i = 0;
    int has_dot = 0;
    
    while (lexer->position < lexer->length && i < MAX_TOKEN_LEN - 1 && 
           (isdigit(peek_char(lexer)) || (!has_dot && peek_char(lexer) == '.'))) {
        if (peek_char(lexer) == '.') has_dot = 1;
        buffer[i++] = next_char(lexer);
    }
    
    if (i > 0) {
        token->type = TOKEN_NUMBER;
        strncpy(token->value, buffer, MAX_TOKEN_LEN - 1);
        token->value[MAX_TOKEN_LEN - 1] = '\0';
        return 1;
    }
    return 0;
}

int read_identifier(struct LexerState *lexer,