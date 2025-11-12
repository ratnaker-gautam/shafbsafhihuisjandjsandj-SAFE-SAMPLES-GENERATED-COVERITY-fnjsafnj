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
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_PAREN,
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

struct Lexer {
    char *input;
    size_t pos;
    size_t len;
    int line;
    int column;
    struct Token tokens[MAX_TOKENS];
    size_t token_count;
};

struct Parser {
    struct Lexer *lexer;
    size_t current_token;
};

void lexer_init(struct Lexer *lexer, char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->pos = 0;
    lexer->len = strlen(input);
    lexer->line = 1;
    lexer->column = 1;
    lexer->token_count = 0;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void lexer_skip_whitespace(struct Lexer *lexer) {
    while (lexer->pos < lexer->len && isspace(lexer->input[lexer->pos])) {
        if (lexer->input[lexer->pos] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->pos++;
    }
}

int lexer_scan_number(struct Lexer *lexer, struct Token *token) {
    if (lexer->pos >= lexer->len || !isdigit(lexer->input[lexer->pos])) return 0;
    
    size_t start = lexer->pos;
    while (lexer->pos < lexer->len && isdigit(lexer->input[lexer->pos])) {
        lexer->pos++;
        lexer->column++;
    }
    
    size_t len = lexer->pos - start;
    if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
    
    strncpy(token->value, lexer->input + start, len);
    token->value[len] = '\0';
    token->type = TOKEN_NUMBER;
    token->line = lexer->line;
    token->column = lexer->column - len;
    return 1;
}

int lexer_scan_identifier(struct Lexer *lexer, struct Token *token) {
    if (lexer->pos >= lexer->len || !isalpha(lexer->input[lexer->pos])) return 0;
    
    size_t start = lexer->pos;
    while (lexer->pos < lexer->len && (isalnum(lexer->input[lexer->pos]) || lexer->input[lexer->pos] == '_')) {
        lexer->pos++;
        lexer->column++;
    }
    
    size_t len = lexer->pos - start;
    if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
    
    strncpy(token->value, lexer->input + start, len);
    token->value[len] = '\0';
    token->type = TOKEN_IDENTIFIER;
    token->line = lexer->line;
    token->column = lexer->column - len;
    return 1;
}

int lexer_scan_operator(struct Lexer *lexer, struct Token *token) {
    if (lexer->pos >= lexer->len || !is_operator_char(lexer->input[lexer->pos])) return 0;
    
    size_t start = lexer->pos;
    lexer->pos++;
    lexer->column++;
    
    strncpy(token->value, lexer->input + start, 1);
    token->value[1] = '\0';
    token->type = TOKEN_OPERATOR;
    token->line = lexer->line;
    token->column = lexer->column - 1;
    return 1;
}

int lexer_scan_paren(struct Lexer *lexer, struct Token *token) {
    if (lexer->pos >= lexer->len || (lexer->input[lexer->pos] != '(' && lexer->input[lexer->pos] != ')')) return 0;
    
    size_t start = lexer->pos;
    lexer->pos++;
    lexer->column++;
    
    strncpy(token->value, lexer->input + start, 1);
    token->value[1] = '\0';
    token->type = TOKEN_PAREN;
    token->line = lexer->line;
    token->column = lexer->column - 1;
    return 1;
}

int lexer_scan_string(struct Lexer *lexer, struct Token *token) {
    if (lexer->pos >= lexer->len || lexer->input[lexer->pos] != '"') return 0;
    
    size_t start = lexer->pos;
    lex