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

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int is_delimiter_char(char c) {
    return strchr("(),;{}[]", c) != NULL;
}

void init_lexer(struct LexerState *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->length = strlen(input);
    lexer->line = 1;
    lexer->column = 1;
}

void skip_whitespace(struct LexerState *lexer) {
    if (lexer == NULL) return;
    while (lexer->position < lexer->length && isspace(lexer->input[lexer->position])) {
        if (lexer->input[lexer->position] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
    }
}

int read_string(struct LexerState *lexer, struct Token *token) {
    if (lexer == NULL || token == NULL) return 0;
    if (lexer->position >= lexer->length) return 0;
    if (lexer->input[lexer->position] != '"') return 0;
    
    size_t start = lexer->position;
    token->line = lexer->line;
    token->column = lexer->column;
    token->type = TOKEN_STRING;
    
    lexer->position++;
    lexer->column++;
    
    size_t value_pos = 0;
    while (lexer->position < lexer->length && lexer->input[lexer->position] != '"') {
        if (value_pos >= MAX_TOKEN_LEN - 1) return 0;
        token->value[value_pos++] = lexer->input[lexer->position++];
        lexer->column++;
    }
    
    if (lexer->position >= lexer->length || lexer->input[lexer->position] != '"') return 0;
    
    lexer->position++;
    lexer->column++;
    token->value[value_pos] = '\0';
    return 1;
}

int read_number(struct LexerState *lexer, struct Token *token) {
    if (lexer == NULL || token == NULL) return 0;
    if (lexer->position >= lexer->length) return 0;
    if (!isdigit(lexer->input[lexer->position])) return 0;
    
    token->line = lexer->line;
    token->column = lexer->column;
    token->type = TOKEN_NUMBER;
    
    size_t value_pos = 0;
    while (lexer->position < lexer->length && isdigit(lexer->input[lexer->position])) {
        if (value_pos >= MAX_TOKEN_LEN - 1) return 0;
        token->value[value_pos++] = lexer->input[lexer->position++];
        lexer->column++;
    }
    token->value[value_pos] = '\0';
    return 1;
}

int read_identifier(struct LexerState *lexer, struct Token *token) {
    if (lexer == NULL || token == NULL) return 0;
    if (lexer->position >= lexer->length) return 0;
    if (!isalpha(lexer->input[lexer->position]) && lexer->input[lexer->position] != '_') return 0;
    
    token->line = lexer->line;
    token->column = lexer->column;
    token->type = TOKEN_IDENTIFIER;
    
    size_t value_pos = 0;
    while (lexer->position < lexer->length && 
           (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_')) {
        if (value_pos >= MAX_TOKEN_LEN - 1) return 0;
        token->value[value_pos++] = lexer->input[lexer->position++];
        lexer->column++;
    }
    token->value[value_pos] = '\0';
    return 1;
}

int read_operator(struct LexerState *lexer, struct Token *token) {
    if (lexer == NULL || token == NULL) return 0;
    if (lexer->position >= lexer->length) return 0;
    if (!is_operator_char(lexer->input[lexer->position]))