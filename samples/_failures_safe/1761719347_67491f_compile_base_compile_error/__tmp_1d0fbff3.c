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
    struct ASTNode *left;
    struct ASTNode *right;
};

struct Token create_token(enum TokenType type, const char *value, size_t line, size_t column) {
    struct Token token;
    token.type = type;
    token.line = line;
    token.column = column;
    if (value != NULL) {
        token.value = malloc(strlen(value) + 1);
        if (token.value != NULL) {
            strcpy(token.value, value);
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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_delimiter_char(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ',' || c == ';';
}

int is_keyword(const char *str) {
    const char *keywords[] = {"if", "else", "while", "for", "return", "int", "float", "char", "void"};
    size_t num_keywords = sizeof(keywords) / sizeof(keywords[0]);
    for (size_t i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
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

char lexer_peek(struct LexerState *lexer) {
    if (lexer->position >= lexer->input_length) {
        return '\0';
    }
    return lexer->input[lexer->position];
}

char lexer_advance(struct LexerState *lexer) {
    if (lexer->position >= lexer->input_length) {
        return '\0';
    }
    char c = lexer->input[lexer->position++];
    if (c == '\n') {
        lexer->line++;
        lexer->column = 1;
    } else {
        lexer->column++;
    }
    return c;
}

void lexer_skip_whitespace(struct LexerState *lexer) {
    while (lexer->position < lexer->input_length && isspace(lexer_peek(lexer))) {
        lexer_advance(lexer);
    }
}

struct Token lexer_next_token(struct LexerState *lexer) {
    lexer_skip_whitespace(lexer);
    if (lexer->position >= lexer->input_length) {
        return create_token(TOKEN_EOF, NULL, lexer->line, lexer->column);
    }
    char current = lexer_peek(lexer);
    size_t start_line = lexer->line;
    size_t start_column = lexer->column;
    if (isalpha(current) || current == '_') {
        char buffer[256];
        size_t i = 0;
        while ((lexer->position < lexer->input_length) && (isalnum(current) || current == '_') && i < 255) {
            buffer[i++] = lexer_advance(lexer);
            current = lexer_peek(lexer);
        }
        buffer[i] = '\0';
        if (is_keyword(buffer)) {
            return create_token(TOKEN_KEYWORD, buffer, start_line, start_column);
        } else {
            return create_token(TOKEN_IDENTIFIER, buffer, start_line, start_column);
        }
    } else if (isdigit(current)) {
        char buffer[256];
        size_t i = 0;
        while ((lexer->position < lexer->input_length) && (isdigit(current) || current == '.') && i < 255) {
            buffer[i++] = lexer_advance(lexer);
            current = lexer_peek(lexer);
        }
        buffer[i] = '\0';
        return create_token(TOKEN_NUMBER, buffer, start_line, start_column);
    } else if (current == '"') {
        lexer_