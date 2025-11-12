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
    TOKEN_DELIMITER,
    TOKEN_KEYWORD,
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

void init_lexer(struct Lexer *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    size_t input_len = strlen(input);
    if (input_len >= MAX_INPUT_LEN) input_len = MAX_INPUT_LEN - 1;
    
    lexer->input = malloc(MAX_INPUT_LEN);
    if (lexer->input == NULL) return;
    
    strncpy(lexer->input, input, input_len);
    lexer->input[input_len] = '\0';
    lexer->pos = 0;
    lexer->len = input_len;
    lexer->line = 1;
    lexer->column = 1;
    lexer->token_count = 0;
}

void free_lexer(struct Lexer *lexer) {
    if (lexer != NULL) {
        free(lexer->input);
    }
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_delimiter(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == ';' || c == ',';
}

int is_keyword(const char *str) {
    static const char *keywords[] = {"if", "else", "while", "for", "return", NULL};
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void skip_whitespace(struct Lexer *lexer) {
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

int read_number(struct Lexer *lexer, struct Token *token) {
    if (lexer == NULL || token == NULL) return 0;
    
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

int read_identifier(struct Lexer *lexer, struct Token *token) {
    if (lexer == NULL || token == NULL) return 0;
    
    size_t start = lexer->pos;
    while (lexer->pos < lexer->len && (isalnum(lexer->input[lexer->pos]) || lexer->input[lexer->pos] == '_')) {
        lexer->pos++;
        lexer->column++;
    }
    
    size_t len = lexer->pos - start;
    if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
    
    strncpy(token->value, lexer->input + start, len);
    token->value[len] = '\0';
    token->line = lexer->line;
    token->column = lexer->column - len;
    
    if (is_keyword(token->value)) {
        token->type = TOKEN_KEYWORD;
    } else {
        token->type = TOKEN_IDENTIFIER;
    }
    
    return 1;
}

int read_operator(struct Lexer *lexer, struct Token *token) {
    if (lexer == NULL || token == NULL) return 0;
    
    size_t start = lexer->pos;
    while (lexer->pos < lexer->len && is_operator_char(lexer->input[lexer->pos])) {
        lexer->pos++;
        lexer->column++;
    }
    
    size_t len = lexer->pos - start;
    if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
    
    strncpy(token->value, lexer->input + start, len