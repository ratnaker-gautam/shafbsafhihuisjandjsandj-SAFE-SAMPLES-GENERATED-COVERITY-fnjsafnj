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
    char input[MAX_INPUT_LEN];
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
    strncpy(lexer->input, input, MAX_INPUT_LEN - 1);
    lexer->input[MAX_INPUT_LEN - 1] = '\0';
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->token_count = 0;
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_delimiter(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == ';' || c == ',';
}

int is_keyword(const char *str) {
    const char *keywords[] = {"if", "else", "while", "for", "return", NULL};
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void add_token(struct Lexer *lexer, enum TokenType type, const char *value) {
    if (lexer->token_count >= MAX_TOKENS) {
        return;
    }
    struct Token *token = &lexer->tokens[lexer->token_count];
    token->type = type;
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
    token->line = lexer->line;
    token->column = lexer->column;
    lexer->token_count++;
}

char peek(struct Lexer *lexer) {
    if (lexer->position >= MAX_INPUT_LEN || lexer->input[lexer->position] == '\0') {
        return '\0';
    }
    return lexer->input[lexer->position];
}

char advance(struct Lexer *lexer) {
    if (lexer->position >= MAX_INPUT_LEN || lexer->input[lexer->position] == '\0') {
        return '\0';
    }
    char c = lexer->input[lexer->position];
    if (c == '\n') {
        lexer->line++;
        lexer->column = 1;
    } else {
        lexer->column++;
    }
    lexer->position++;
    return c;
}

void skip_whitespace(struct Lexer *lexer) {
    while (isspace(peek(lexer))) {
        advance(lexer);
    }
}

void read_number(struct Lexer *lexer) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    char c = peek(lexer);
    while (isdigit(c) && i < MAX_TOKEN_LEN - 1) {
        buffer[i++] = advance(lexer);
        c = peek(lexer);
    }
    buffer[i] = '\0';
    add_token(lexer, TOKEN_NUMBER, buffer);
}

void read_identifier(struct Lexer *lexer) {
    char buffer[MAX_TOKEN_LEN];
    int i = 0;
    char c = peek(lexer);
    while ((isalnum(c) || c == '_') && i < MAX_TOKEN_LEN - 1) {
        buffer[i++] = advance(lexer);
        c = peek(lexer);
    }
    buffer[i] = '\0';
    if (is_keyword(buffer)) {
        add_token(lexer, TOKEN_KEYWORD, buffer);
    } else {
        add_token(lexer, TOKEN_IDENTIFIER, buffer);
    }
}

void tokenize(struct Lexer *lexer) {
    while (peek(lexer) != '\0') {
        skip_whitespace(lexer);
        char c = peek(lexer);
        if (c == '\0') break;
        
        if (isdigit(c)) {
            read_number(lexer);
        } else if (isalpha(c) || c == '_') {
            read_identifier(lexer);
        } else if (is_operator(c)) {
            char op[2] = {advance(lexer), '\0'};
            add_token(lexer, TOKEN_OPERATOR, op);
        } else if (is_delimiter(c)) {
            char delim[2] = {advance(lexer), '\0'};
            add_token(lexer, TOKEN_DELIMITER, delim);
        } else {
            char unknown[2] = {adv