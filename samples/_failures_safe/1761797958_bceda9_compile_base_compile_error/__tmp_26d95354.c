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
    TOKEN_ERROR
};

struct Token {
    enum TokenType type;
    char value[64];
    uint32_t line;
    uint32_t column;
};

struct LexerState {
    const char *input;
    uint32_t position;
    uint32_t line;
    uint32_t column;
    uint32_t input_length;
};

struct ParserNode {
    enum { NODE_EXPRESSION, NODE_STATEMENT, NODE_LITERAL } type;
    struct Token token;
    struct ParserNode *left;
    struct ParserNode *right;
};

struct ParseResult {
    int success;
    struct ParserNode *ast;
    char error_msg[128];
};

static const char *keywords[] = {
    "if", "else", "while", "for", "return", "int", "char", "void"
};
static const size_t num_keywords = 8;

int is_keyword(const char *str) {
    for (size_t i = 0; i < num_keywords; i++) {
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
    struct Token token = {TOKEN_ERROR, "", lexer->line, lexer->column};
    if (lexer == NULL || lexer->input == NULL) return token;
    
    skip_whitespace(lexer);
    if (lexer->position >= lexer->input_length) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char current = lexer->input[lexer->position];
    
    if (isalpha(current) || current == '_') {
        size_t i = 0;
        while ((lexer->position < lexer->input_length) && 
               (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') &&
               (i < sizeof(token.value) - 1)) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
        token.type = is_keyword(token.value) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
    }
    else if (isdigit(current)) {
        size_t i = 0;
        while ((lexer->position < lexer->input_length) && 
               (isdigit(lexer->input[lexer->position])) &&
               (i < sizeof(token.value) - 1)) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
        token.type = TOKEN_NUMBER;
    }
    else if (current == '"') {
        size_t i = 0;
        lexer->position++;
        lexer->column++;
        while ((lexer->position < lexer->input_length) && 
               (lexer->input[lexer->position] != '"') &&
               (i < sizeof(token.value) - 1)) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
        if (lexer->position < lexer->input_length && lexer->input[lexer->position] == '"') {
            lexer->position++;
            lexer->column++;
            token.type = TOKEN_STRING;
        } else {
            token.type = TOKEN_ERROR;
        }
    }
    else if (strchr("+-*/=<>!&|", current) != NULL) {
        size_t i = 0;
        while ((lexer->position < lexer->input_length) && 
               (strchr("+-*/=<>!&|", lexer->input[lexer->position]) != NULL) &&
               (i < sizeof(token.value) - 1)) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
        token.type = TOKEN_OPERATOR;
    }
    else if (strchr("(),;{}[]", current) != NULL) {
        token.value[0] = current;