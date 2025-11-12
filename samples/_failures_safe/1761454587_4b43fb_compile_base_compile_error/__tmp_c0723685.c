//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

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

struct Lexer {
    const char *input;
    int position;
    int line;
    int column;
    struct Token tokens[MAX_TOKENS];
    int token_count;
};

struct Parser {
    struct Lexer *lexer;
    int current_token;
};

void init_lexer(struct Lexer *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->token_count = 0;
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_delimiter(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ',' || c == ';';
}

void add_token(struct Lexer *lexer, enum TokenType type, const char *value) {
    if (lexer == NULL || value == NULL || lexer->token_count >= MAX_TOKENS) return;
    struct Token *token = &lexer->tokens[lexer->token_count];
    token->type = type;
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
    token->line = lexer->line;
    token->column = lexer->column;
    lexer->token_count++;
}

void skip_whitespace(struct Lexer *lexer) {
    if (lexer == NULL || lexer->input == NULL) return;
    while (lexer->input[lexer->position] != '\0' && isspace(lexer->input[lexer->position])) {
        if (lexer->input[lexer->position] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
    }
}

void tokenize_identifier(struct Lexer *lexer) {
    if (lexer == NULL || lexer->input == NULL) return;
    char buffer[MAX_TOKEN_LEN] = {0};
    int i = 0;
    while (lexer->input[lexer->position] != '\0' && 
           (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') &&
           i < MAX_TOKEN_LEN - 1) {
        buffer[i++] = lexer->input[lexer->position];
        lexer->position++;
        lexer->column++;
    }
    buffer[i] = '\0';
    add_token(lexer, TOKEN_IDENTIFIER, buffer);
}

void tokenize_number(struct Lexer *lexer) {
    if (lexer == NULL || lexer->input == NULL) return;
    char buffer[MAX_TOKEN_LEN] = {0};
    int i = 0;
    while (lexer->input[lexer->position] != '\0' && 
           isdigit(lexer->input[lexer->position]) &&
           i < MAX_TOKEN_LEN - 1) {
        buffer[i++] = lexer->input[lexer->position];
        lexer->position++;
        lexer->column++;
    }
    buffer[i] = '\0';
    add_token(lexer, TOKEN_NUMBER, buffer);
}

void tokenize_string(struct Lexer *lexer) {
    if (lexer == NULL || lexer->input == NULL) return;
    char buffer[MAX_TOKEN_LEN] = {0};
    int i = 0;
    lexer->position++;
    lexer->column++;
    while (lexer->input[lexer->position] != '\0' && 
           lexer->input[lexer->position] != '"' &&
           i < MAX_TOKEN_LEN - 1) {
        buffer[i++] = lexer->input[lexer->position];
        lexer->position++;
        lexer->column++;
    }
    buffer[i] = '\0';
    if (lexer->input[lexer->position] == '"') {
        lexer->position++;
        lexer->column++;
    }
    add_token(lexer, TOKEN_STRING, buffer);
}

int tokenize(struct Lexer *lexer) {
    if (lexer == NULL || lexer->input == NULL) return 0;
    while (lexer->input[lexer->position] != '\0') {
        skip_whitespace(lexer);
        if (lexer->input[lexer->position] == '\0') break;

        char current = lexer->input[lexer->position];
        if (isalpha(current) || current == '_') {
            tokenize_identifier(