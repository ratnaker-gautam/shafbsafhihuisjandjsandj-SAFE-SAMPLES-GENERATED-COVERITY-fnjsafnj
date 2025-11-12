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

struct ParserNode {
    enum { NODE_EXPRESSION, NODE_STATEMENT, NODE_DECLARATION } type;
    struct Token token;
    struct ParserNode *left;
    struct ParserNode *right;
};

struct ParseResult {
    int success;
    struct ParserNode *ast;
    char *error_message;
};

static const char *KEYWORDS[] = {"if", "else", "while", "for", "return", "int", "float", "char", NULL};

void init_lexer(struct LexerState *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->input_length = strlen(input);
}

int is_keyword(const char *str) {
    if (str == NULL) return 0;
    for (int i = 0; KEYWORDS[i] != NULL; i++) {
        if (strcmp(str, KEYWORDS[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

char *substring(const char *str, size_t start, size_t length) {
    if (str == NULL || start > strlen(str)) return NULL;
    size_t str_len = strlen(str);
    if (start + length > str_len) length = str_len - start;
    char *result = malloc(length + 1);
    if (result == NULL) return NULL;
    strncpy(result, str + start, length);
    result[length] = '\0';
    return result;
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

struct Token next_token(struct LexerState *lexer) {
    struct Token token = {TOKEN_INVALID, NULL, lexer->line, lexer->column};
    if (lexer == NULL || lexer->input == NULL) return token;
    
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
        token.type = TOKEN_EOF;
        return token;
    }
    
    char current = lexer->input[lexer->position];
    size_t start = lexer->position;
    
    if (isalpha(current) || current == '_') {
        while (lexer->position < lexer->input_length && (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_')) {
            lexer->position++;
            lexer->column++;
        }
        size_t length = lexer->position - start;
        token.value = substring(lexer->input, start, length);
        if (token.value == NULL) {
            token.type = TOKEN_INVALID;
            return token;
        }
        token.type = is_keyword(token.value) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
    } else if (isdigit(current)) {
        while (lexer->position < lexer->input_length && isdigit(lexer->input[lexer->position])) {
            lexer->position++;
            lexer->column++;
        }
        if (lexer->position < lexer->input_length && lexer->input[lexer->position] == '.') {
            lexer->position++;
            lexer->column++;
            while (lexer->position < lexer->input_length && isdigit(lexer->input[lexer->position])) {
                lexer->position++;
                lexer->column++;
            }
        }
        size_t length = lexer->position - start;
        token.value = substring(lexer->input, start, length);
        token.type = token.value ? TOKEN_NUMBER : TOKEN_INVALID;
    } else if (current == '"') {
        lexer->position++;
        lexer->column++;
        start++;
        while (lexer->position < lexer->input_length && lexer->input[lexer->position] != '"') {
            if (lexer->input[lexer->position] == '\\') {
                lexer->position++;
                lexer->column++;
            }
            lexer->position++;
            lexer->column++;
        }
        if (lexer->position >= lexer->input_length) {
            token.type = TOKEN_INVALID;
            return token;
        }
        size_t length = lexer->position - start;
        token.value