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
    return strchr("+-*/=<>!&|", c) != NULL;
}

int is_delimiter(char c) {
    return strchr("(),;{}[]", c) != NULL;
}

struct Token create_token(enum TokenType type, const char *value, int line, int column) {
    struct Token token;
    token.type = type;
    strncpy(token.value, value, MAX_TOKEN_LEN - 1);
    token.value[MAX_TOKEN_LEN - 1] = '\0';
    token.line = line;
    token.column = column;
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

    int start_pos = lexer->position;
    int start_line = lexer->line;
    int start_col = lexer->column;

    if (isalpha(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') {
        while (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') {
            lexer->position++;
            lexer->column++;
        }
        int len = lexer->position - start_pos;
        if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
        char value[MAX_TOKEN_LEN];
        strncpy(value, &lexer->input[start_pos], len);
        value[len] = '\0';
        *token = create_token(TOKEN_IDENTIFIER, value, start_line, start_col);
        return 1;
    }

    if (isdigit(lexer->input[lexer->position])) {
        while (isdigit(lexer->input[lexer->position])) {
            lexer->position++;
            lexer->column++;
        }
        if (lexer->input[lexer->position] == '.') {
            lexer->position++;
            lexer->column++;
            while (isdigit(lexer->input[lexer->position])) {
                lexer->position++;
                lexer->column++;
            }
        }
        int len = lexer->position - start_pos;
        if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
        char value[MAX_TOKEN_LEN];
        strncpy(value, &lexer->input[start_pos], len);
        value[len] = '\0';
        *token = create_token(TOKEN_NUMBER, value, start_line, start_col);
        return 1;
    }

    if (lexer->input[lexer->position] == '"') {
        lexer->position++;
        lexer->column++;
        start_pos = lexer->position;
        start_col = lexer->column;
        while (lexer->input[lexer->position] != '"' && lexer->input[lexer->position] != '\0') {
            if (lexer->input[lexer->position] == '\n') {
                lexer->line++;
                lexer->column = 1;
            } else {
                lexer->column++;
            }
            lexer->position++;
        }
        if (lexer->input[lexer->position] == '"') {
            int len = lexer->position - start_pos;
            if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
            char value[MAX_TOKEN_LEN];
            strncpy(value, &lexer->input[start_pos], len);
            value[len] = '\0';
            lexer->position++;
            lexer->column++;
            *token = create_token(TOKEN_STRING, value, start_line, start_col);
            return 1