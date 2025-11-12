//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

enum TokenType {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
    TOKEN_KEYWORD,
    TOKEN_EOF,
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char *value;
    size_t line;
    size_t column;
};

struct LexerState {
    const char *input;
    size_t position;
    size_t line;
    size_t column;
    size_t input_length;
};

struct ParserState {
    struct Token *tokens;
    size_t token_count;
    size_t current_token;
};

struct ASTNode {
    char *type;
    char *value;
    struct ASTNode **children;
    size_t child_count;
};

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int is_delimiter(char c) {
    return strchr("(),;{}[]", c) != NULL;
}

int is_keyword(const char *str) {
    const char *keywords[] = {"if", "else", "while", "for", "return", "int", "float", "char", "void"};
    size_t count = sizeof(keywords) / sizeof(keywords[0]);
    for (size_t i = 0; i < count; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void init_lexer(struct LexerState *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->input_length = strlen(input);
}

void skip_whitespace(struct LexerState *lexer) {
    if (lexer == NULL) return;
    while (lexer->position < lexer->input_length && isspace(lexer->input[lexer->position])) {
        if (lexer->input[lexer->position] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
    }
}

struct Token next_token(struct LexerState *lexer) {
    struct Token token = {TOKEN_INVALID, NULL, lexer->line, lexer->column};
    if (lexer == NULL || lexer->position >= lexer->input_length) {
        token.type = TOKEN_EOF;
        return token;
    }

    skip_whitespace(lexer);
    if (lexer->position >= lexer->input_length) {
        token.type = TOKEN_EOF;
        return token;
    }

    char current = lexer->input[lexer->position];
    size_t start_pos = lexer->position;
    size_t start_col = lexer->column;

    if (isalpha(current) || current == '_') {
        while (lexer->position < lexer->input_length && 
               (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_')) {
            lexer->position++;
            lexer->column++;
        }
        size_t length = lexer->position - start_pos;
        token.value = malloc(length + 1);
        if (token.value != NULL) {
            strncpy(token.value, lexer->input + start_pos, length);
            token.value[length] = '\0';
            token.type = is_keyword(token.value) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
        }
    } else if (isdigit(current)) {
        while (lexer->position < lexer->input_length && isdigit(lexer->input[lexer->position])) {
            lexer->position++;
            lexer->column++;
        }
        size_t length = lexer->position - start_pos;
        token.value = malloc(length + 1);
        if (token.value != NULL) {
            strncpy(token.value, lexer->input + start_pos, length);
            token.value[length] = '\0';
            token.type = TOKEN_NUMBER;
        }
    } else if (current == '"') {
        lexer->position++;
        lexer->column++;
        start_pos = lexer->position;
        while (lexer->position < lexer->input_length && lexer->input[lexer->position] != '"') {
            lexer->position++;
            lexer->column++;
        }
        if (lexer->position < lexer->input_length && lexer->input[lexer->position] == '"') {
            size_t length = lexer->position - start_pos;
            token.value = malloc(length + 1);
            if (token.value != NULL) {
                strncpy(token.value, lexer->input + start_pos, length);
                token.value[length] = '\0';
                token.type = TOKEN_STRING;
            }
            lexer->position++;
            lexer->column++;
        }
    } else if (is_operator_char(current)) {
        while (lexer->position < lexer->input_length && is_operator_char(lexer->input[lexer->position])) {
            lexer->position++;