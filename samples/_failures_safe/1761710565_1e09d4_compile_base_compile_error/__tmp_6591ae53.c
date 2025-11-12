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
    enum { NODE_EXPRESSION, NODE_STATEMENT, NODE_DECLARATION } type;
    struct Token token;
    struct ParserNode *left;
    struct ParserNode *right;
};

struct ParserState {
    struct LexerState *lexer;
    struct Token current_token;
    size_t error_count;
};

void init_lexer(struct LexerState *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->input_length = strlen(input);
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_punctuation_char(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == ';' || c == ',';
}

int is_keyword(const char *str) {
    static const char *keywords[] = {"if", "else", "while", "return", "int", "char", NULL};
    for (size_t i = 0; keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0) return 1;
    }
    return 0;
}

struct Token next_token(struct LexerState *lexer) {
    struct Token token = {TOKEN_INVALID, NULL, lexer->line, lexer->column};
    
    if (lexer->position >= lexer->input_length) {
        token.type = TOKEN_EOF;
        return token;
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
        start_pos++;
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
        token.value = malloc(2);
        if (token.value != NULL) {
            token.value[0] = current;
            token.value[1] = '\0';
            token.type = TOKEN_