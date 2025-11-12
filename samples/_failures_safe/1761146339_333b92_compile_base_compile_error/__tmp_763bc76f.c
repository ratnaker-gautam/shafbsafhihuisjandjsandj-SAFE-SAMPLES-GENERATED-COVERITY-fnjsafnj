//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 1000
#define MAX_TOKEN_LEN 256

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
    int line;
    int column;
} Token;

typedef struct {
    char *input;
    int position;
    int line;
    int column;
    Token tokens[MAX_TOKENS];
    int token_count;
} Lexer;

void init_lexer(Lexer *lexer, char *input) {
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->token_count = 0;
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

void add_token(Lexer *lexer, TokenType type, char *value) {
    if (lexer->token_count >= MAX_TOKENS) {
        return;
    }
    Token *token = &lexer->tokens[lexer->token_count];
    token->type = type;
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
    token->line = lexer->line;
    token->column = lexer->column;
    lexer->token_count++;
}

void skip_whitespace(Lexer *lexer) {
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

int parse_number(Lexer *lexer) {
    int start = lexer->position;
    while (lexer->input[lexer->position] != '\0' && isdigit(lexer->input[lexer->position])) {
        lexer->position++;
        lexer->column++;
    }
    if (lexer->position - start >= MAX_TOKEN_LEN) {
        return 0;
    }
    char num[MAX_TOKEN_LEN];
    strncpy(num, &lexer->input[start], lexer->position - start);
    num[lexer->position - start] = '\0';
    add_token(lexer, TOKEN_NUMBER, num);
    return 1;
}

int parse_identifier(Lexer *lexer) {
    int start = lexer->position;
    while (lexer->input[lexer->position] != '\0' && (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_')) {
        lexer->position++;
        lexer->column++;
    }
    if (lexer->position - start >= MAX_TOKEN_LEN) {
        return 0;
    }
    char ident[MAX_TOKEN_LEN];
    strncpy(ident, &lexer->input[start], lexer->position - start);
    ident[lexer->position - start] = '\0';
    add_token(lexer, TOKEN_IDENTIFIER, ident);
    return 1;
}

int parse_string(Lexer *lexer) {
    lexer->position++;
    lexer->column++;
    int start = lexer->position;
    while (lexer->input[lexer->position] != '\0' && lexer->input[lexer->position] != '"') {
        if (lexer->input[lexer->position] == '\n') {
            return 0;
        }
        lexer->position++;
        lexer->column++;
    }
    if (lexer->input[lexer->position] != '"') {
        return 0;
    }
    if (lexer->position - start >= MAX_TOKEN_LEN) {
        return 0;
    }
    char str[MAX_TOKEN_LEN];
    strncpy(str, &lexer->input[start], lexer->position - start);
    str[lexer->position - start] = '\0';
    add_token(lexer, TOKEN_STRING, str);
    lexer->position++;
    lexer->column++;
    return 1;
}

int parse_operator(Lexer *lexer) {
    char op[3] = {0};
    op[0] = lexer->input[lexer->position];
    lexer->position++;
    lexer->column++;
    if (lexer->input[lexer->position] == '=') {
        if (op[0] == '<' || op[0] == '>' || op[0] == '!' || op[0] == '=') {
            op[1] = '=';
            lexer->position++;
            lexer->column++;
        }
    }
    add_token(lexer, TOKEN_OPERATOR, op);
    return 1;
}

int tokenize(Lexer *lexer) {
    while (lexer->input[lexer->position] != '\0