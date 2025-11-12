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
    TOKEN_PUNCTUATION,
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

struct ParserNode {
    enum { NODE_EXPRESSION, NODE_STATEMENT, NODE_LITERAL } node_type;
    struct Token token;
    struct ParserNode *left;
    struct ParserNode *right;
};

struct ParserState {
    struct Token *tokens;
    size_t token_count;
    size_t current_token;
};

int is_keyword(const char *str) {
    static const char *keywords[] = {"if", "else", "while", "for", "return", "int", "char", "void"};
    size_t num_keywords = sizeof(keywords) / sizeof(keywords[0]);
    for (size_t i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

struct Token create_token(enum TokenType type, const char *value, size_t line, size_t column) {
    struct Token token;
    token.type = type;
    token.line = line;
    token.column = column;
    if (value != NULL) {
        size_t len = strlen(value);
        if (len > 0) {
            token.value = malloc(len + 1);
            if (token.value != NULL) {
                strncpy(token.value, value, len);
                token.value[len] = '\0';
            } else {
                token.value = NULL;
            }
        } else {
            token.value = NULL;
        }
    } else {
        token.value = NULL;
    }
    return token;
}

void free_token(struct Token *token) {
    if (token != NULL && token->value != NULL) {
        free(token->value);
        token->value = NULL;
    }
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int is_punctuation_char(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == ';' || c == ',' || c == '.';
}

struct LexerState init_lexer(const char *input) {
    struct LexerState lexer;
    lexer.input = input;
    lexer.position = 0;
    lexer.line = 1;
    lexer.column = 1;
    lexer.input_length = strlen(input);
    return lexer;
}

struct Token get_next_token(struct LexerState *lexer) {
    if (lexer == NULL || lexer->input == NULL) {
        return create_token(TOKEN_INVALID, NULL, 0, 0);
    }

    while (lexer->position < lexer->input_length && isspace(lexer->input[lexer->position])) {
        if (lexer->input[lexer->position] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
    }

    if (lexer->position >= lexer->input_length) {
        return create_token(TOKEN_EOF, NULL, lexer->line, lexer->column);
    }

    char current = lexer->input[lexer->position];
    size_t start_line = lexer->line;
    size_t start_column = lexer->column;

    if (isalpha(current) || current == '_') {
        size_t start = lexer->position;
        while (lexer->position < lexer->input_length && (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_')) {
            lexer->position++;
            lexer->column++;
        }
        size_t length = lexer->position - start;
        char *ident = malloc(length + 1);
        if (ident == NULL) {
            return create_token(TOKEN_INVALID, NULL, start_line, start_column);
        }
        strncpy(ident, lexer->input + start, length);
        ident[length] = '\0';
        enum TokenType type = is_keyword(ident) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
        struct Token token = create_token(type, ident, start_line, start_column);
        free(ident);
        return token;
    } else if (isdigit(current)) {
        size_t start = lexer->position;
        while (lexer->position < lexer->input_length && isdigit(lexer->input[lexer->position])) {
            lexer->position++;
            lexer->column++;
        }
        size_t length = lexer->position - start;
        char *num_str = malloc(length + 1);
        if (num_str == NULL) {
            return create_token(TOKEN_IN