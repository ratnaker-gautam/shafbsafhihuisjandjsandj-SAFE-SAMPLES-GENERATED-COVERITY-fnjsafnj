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
    TOKEN_UNKNOWN
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
    enum { NODE_EXPRESSION, NODE_ASSIGNMENT, NODE_LITERAL } type;
    struct Token token;
    struct ParserNode *left;
    struct ParserNode *right;
};

struct ParserState {
    struct LexerState *lexer;
    struct Token current_token;
};

void init_lexer(struct LexerState *lexer, const char *input) {
    if (lexer == NULL || input == NULL) {
        return;
    }
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->input_length = strlen(input);
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_delimiter(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ',' || c == ';';
}

int is_keyword(const char *str) {
    static const char *keywords[] = {"if", "else", "while", "for", "return", NULL};
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

struct Token next_token(struct LexerState *lexer) {
    struct Token token = {TOKEN_UNKNOWN, NULL, lexer->line, lexer->column};
    
    if (lexer == NULL) {
        return token;
    }
    
    if (lexer->position >= lexer->input_length) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char current = lexer->input[lexer->position];
    
    while (isspace(current)) {
        if (current == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
        if (lexer->position >= lexer->input_length) {
            token.type = TOKEN_EOF;
            return token;
        }
        current = lexer->input[lexer->position];
    }
    
    token.line = lexer->line;
    token.column = lexer->column;
    
    if (isalpha(current) || current == '_') {
        size_t start = lexer->position;
        while (lexer->position < lexer->input_length && 
               (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_')) {
            lexer->position++;
            lexer->column++;
        }
        size_t length = lexer->position - start;
        if (length > 0) {
            token.value = malloc(length + 1);
            if (token.value != NULL) {
                strncpy(token.value, lexer->input + start, length);
                token.value[length] = '\0';
                if (is_keyword(token.value)) {
                    token.type = TOKEN_KEYWORD;
                } else {
                    token.type = TOKEN_IDENTIFIER;
                }
            }
        }
    } else if (isdigit(current)) {
        size_t start = lexer->position;
        while (lexer->position < lexer->input_length && isdigit(lexer->input[lexer->position])) {
            lexer->position++;
            lexer->column++;
        }
        size_t length = lexer->position - start;
        if (length > 0) {
            token.value = malloc(length + 1);
            if (token.value != NULL) {
                strncpy(token.value, lexer->input + start, length);
                token.value[length] = '\0';
                token.type = TOKEN_NUMBER;
            }
        }
    } else if (current == '"') {
        size_t start = ++lexer->position;
        lexer->column++;
        while (lexer->position < lexer->input_length && lexer->input[lexer->position] != '"') {
            if (lexer->input[lexer->position] == '\n') {
                lexer->line++;
                lexer->column = 1;
            } else {
                lexer->column++;
            }
            lexer->position++;
        }
        if (lexer->position < lexer->input_length && lexer->input[lexer->position] == '"') {
            size_t length = lexer->position - start;
            token.value = malloc(length + 1);
            if (token.value != NULL) {
                strncpy(token.value, lexer->input + start, length);