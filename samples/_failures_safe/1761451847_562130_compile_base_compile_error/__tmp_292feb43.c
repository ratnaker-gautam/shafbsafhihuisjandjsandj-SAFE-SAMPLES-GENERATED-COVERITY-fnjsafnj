//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

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
    int error_count;
};

struct Parser {
    struct Token *tokens;
    int token_count;
    int current_token;
    int error_count;
};

void lexer_init(struct Lexer *lexer, const char *input) {
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->error_count = 0;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_delimiter(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ',' || c == ';';
}

struct Token lexer_next_token(struct Lexer *lexer) {
    struct Token token = {TOKEN_UNKNOWN, "", lexer->line, lexer->column};
    
    if (lexer->input == NULL || lexer->position < 0) {
        strncpy(token.value, "EOF", MAX_TOKEN_LEN - 1);
        token.type = TOKEN_EOF;
        return token;
    }
    
    while (lexer->input[lexer->position] != '\0' && isspace(lexer->input[lexer->position])) {
        if (lexer->input[lexer->position] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
    }
    
    if (lexer->input[lexer->position] == '\0') {
        strncpy(token.value, "EOF", MAX_TOKEN_LEN - 1);
        token.type = TOKEN_EOF;
        return token;
    }
    
    char current = lexer->input[lexer->position];
    token.line = lexer->line;
    token.column = lexer->column;
    
    if (isalpha(current) || current == '_') {
        int i = 0;
        while ((isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') && i < MAX_TOKEN_LEN - 1) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
        token.type = TOKEN_IDENTIFIER;
    } else if (isdigit(current)) {
        int i = 0;
        while (isdigit(lexer->input[lexer->position]) && i < MAX_TOKEN_LEN - 1) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
        token.type = TOKEN_NUMBER;
    } else if (current == '"') {
        int i = 0;
        lexer->position++;
        lexer->column++;
        while (lexer->input[lexer->position] != '"' && lexer->input[lexer->position] != '\0' && i < MAX_TOKEN_LEN - 1) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        if (lexer->input[lexer->position] == '"') {
            lexer->position++;
            lexer->column++;
        } else {
            lexer->error_count++;
        }
        token.value[i] = '\0';
        token.type = TOKEN_STRING;
    } else if (is_operator_char(current)) {
        int i = 0;
        while (is_operator_char(lexer->input[lexer->position]) && i < MAX_TOKEN_LEN - 1) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
        token.type = TOKEN_OPERATOR;
    } else if (is_delimiter(current)) {
        token.value[0] = lexer->input[lexer->position++];
        token.value[1] = '\0';
        lexer->column++;
        token.type = TOKEN_DELIMITER;
    } else {
        token.value[0] = lexer->input[lexer->position++];
        token.value[1] = '\0';
        lexer->column++;
        token.type = TOKEN_UNKNOWN;
        lexer->error_count++;
    }
    
    return token;
}

void parser_init(struct Parser *parser, struct Token *tokens, int token_count) {
    parser->tokens = tokens;
    parser->token_count = token_count > MAX_TOKENS ? MAX_TOKENS : token