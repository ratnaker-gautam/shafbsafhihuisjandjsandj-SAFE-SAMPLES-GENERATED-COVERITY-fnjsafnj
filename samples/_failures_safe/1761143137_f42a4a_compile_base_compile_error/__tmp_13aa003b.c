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

struct LexerState {
    const char *input;
    int position;
    int line;
    int column;
    int token_count;
};

struct ParserState {
    struct Token *tokens;
    int current_token;
    int token_count;
};

void init_lexer(struct LexerState *lexer, const char *input) {
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->token_count = 0;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_delimiter(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ',' || c == ';';
}

struct Token create_token(enum TokenType type, const char *value, int line, int column) {
    struct Token token;
    token.type = type;
    token.line = line;
    token.column = column;
    strncpy(token.value, value, MAX_TOKEN_LEN - 1);
    token.value[MAX_TOKEN_LEN - 1] = '\0';
    return token;
}

int next_token(struct LexerState *lexer, struct Token *token) {
    if (lexer->input[lexer->position] == '\0') {
        *token = create_token(TOKEN_EOF, "", lexer->line, lexer->column);
        return 1;
    }

    while (isspace(lexer->input[lexer->position])) {
        if (lexer->input[lexer->position] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
    }

    if (lexer->input[lexer->position] == '\0') {
        *token = create_token(TOKEN_EOF, "", lexer->line, lexer->column);
        return 1;
    }

    char current = lexer->input[lexer->position];
    int start_pos = lexer->position;
    int start_line = lexer->line;
    int start_col = lexer->column;

    if (isalpha(current) || current == '_') {
        char buffer[MAX_TOKEN_LEN] = {0};
        int i = 0;
        while ((isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') && i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = lexer->input[lexer->position];
            lexer->position++;
            lexer->column++;
        }
        buffer[i] = '\0';
        *token = create_token(TOKEN_IDENTIFIER, buffer, start_line, start_col);
        return 1;
    }

    if (isdigit(current)) {
        char buffer[MAX_TOKEN_LEN] = {0};
        int i = 0;
        while (isdigit(lexer->input[lexer->position]) && i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = lexer->input[lexer->position];
            lexer->position++;
            lexer->column++;
        }
        buffer[i] = '\0';
        *token = create_token(TOKEN_NUMBER, buffer, start_line, start_col);
        return 1;
    }

    if (current == '"') {
        char buffer[MAX_TOKEN_LEN] = {0};
        int i = 0;
        lexer->position++;
        lexer->column++;
        while (lexer->input[lexer->position] != '"' && lexer->input[lexer->position] != '\0' && i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = lexer->input[lexer->position];
            lexer->position++;
            lexer->column++;
        }
        if (lexer->input[lexer->position] == '"') {
            lexer->position++;
            lexer->column++;
        }
        buffer[i] = '\0';
        *token = create_token(TOKEN_STRING, buffer, start_line, start_col);
        return 1;
    }

    if (is_operator_char(current)) {
        char buffer[3] = {0};
        buffer[0] = current;
        lexer->position++;
        lexer->column++;
        if (is_operator_char(lexer->input[lexer->position])) {
            buffer[1] = lexer->input[lexer->position];
            lexer->position++;
            lexer->column++;
        }
        *token = create_token(TOKEN_OPERATOR, buffer, start_line, start_col);
        return 1;
    }

    if (is_delimiter(current)) {
        char