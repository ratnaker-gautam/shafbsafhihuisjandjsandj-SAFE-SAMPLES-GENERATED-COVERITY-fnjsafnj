//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
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
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->token_count = 0;
}

void init_parser(struct Parser *parser, struct Lexer *lexer) {
    parser->lexer = lexer;
    parser->current_token = 0;
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void add_token(struct Lexer *lexer, enum TokenType type, const char *value) {
    if (lexer->token_count >= MAX_TOKENS) {
        return;
    }
    struct Token *token = &lexer->tokens[lexer->token_count];
    token->type = type;
    token->line = lexer->line;
    token->column = lexer->column;
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
    lexer->token_count++;
}

void skip_whitespace(struct Lexer *lexer) {
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

int parse_number(struct Lexer *lexer) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    while (lexer->input[lexer->position] != '\0' && isdigit(lexer->input[lexer->position])) {
        if (i >= MAX_TOKEN_LEN - 1) {
            return 0;
        }
        buffer[i++] = lexer->input[lexer->position++];
        lexer->column++;
    }
    buffer[i] = '\0';
    add_token(lexer, TOKEN_NUMBER, buffer);
    return 1;
}

int parse_identifier(struct Lexer *lexer) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    while (lexer->input[lexer->position] != '\0' && (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_')) {
        if (i >= MAX_TOKEN_LEN - 1) {
            return 0;
        }
        buffer[i++] = lexer->input[lexer->position++];
        lexer->column++;
    }
    buffer[i] = '\0';
    add_token(lexer, TOKEN_IDENTIFIER, buffer);
    return 1;
}

int parse_string(struct Lexer *lexer) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    if (lexer->input[lexer->position] != '"') {
        return 0;
    }
    lexer->position++;
    lexer->column++;
    while (lexer->input[lexer->position] != '\0' && lexer->input[lexer->position] != '"') {
        if (i >= MAX_TOKEN_LEN - 1) {
            return 0;
        }
        buffer[i++] = lexer->input[lexer->position++];
        lexer->column++;
    }
    if (lexer->input[lexer->position] == '"') {
        lexer->position++;
        lexer->column++;
    }
    buffer[i] = '\0';
    add_token(lexer, TOKEN_STRING, buffer);
    return 1;
}

int parse_operator(struct Lexer *lexer) {
    char buffer[3] = {0};
    buffer[0] = lexer->input[lexer->position];
    lexer->position++;
    lexer->column++;
    if (lexer->input[lexer->position] == '=' && (buffer[0] == '=' || buffer[0] == '!' || buffer[0] == '<' || buffer[0] == '>')) {
        buffer[1] = '=';
        lexer->position++;
        lexer->column++;
    }
    add_token(lexer, TOKEN_OPERATOR, buffer);
    return 1;
}

int tokenize(struct Lexer *lexer) {
    while (lexer->input[lexer->position] != '\0') {
        skip_whitespace(lexer);
        if (lexer