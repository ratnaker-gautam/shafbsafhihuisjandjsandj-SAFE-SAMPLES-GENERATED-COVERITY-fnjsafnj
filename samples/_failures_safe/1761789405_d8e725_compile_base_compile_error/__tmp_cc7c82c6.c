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

struct ParserState {
    struct Token *tokens;
    uint32_t token_count;
    uint32_t current_token;
};

struct ParseTree {
    char node_type[32];
    char value[64];
    struct ParseTree **children;
    uint32_t child_count;
};

int is_keyword(const char *str) {
    const char *keywords[] = {"if", "else", "while", "for", "return", "int", "float", "char", "void"};
    const size_t keyword_count = sizeof(keywords) / sizeof(keywords[0]);
    for (size_t i = 0; i < keyword_count; i++) {
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
    while (lexer->position < lexer->input_length && isspace((unsigned char)lexer->input[lexer->position])) {
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
    struct Token token = {TOKEN_INVALID, "", lexer->line, lexer->column};
    
    if (lexer == NULL || lexer->input == NULL) {
        return token;
    }
    
    if (lexer->position >= lexer->input_length) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    skip_whitespace(lexer);
    
    if (lexer->position >= lexer->input_length) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char current = lexer->input[lexer->position];
    
    if (isalpha((unsigned char)current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        size_t i = 0;
        while (lexer->position < lexer->input_length && 
               (isalnum((unsigned char)lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') &&
               i < sizeof(token.value) - 1) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
        if (is_keyword(token.value)) {
            token.type = TOKEN_KEYWORD;
        }
    } else if (isdigit((unsigned char)current)) {
        token.type = TOKEN_NUMBER;
        size_t i = 0;
        while (lexer->position < lexer->input_length && 
               (isdigit((unsigned char)lexer->input[lexer->position]) || lexer->input[lexer->position] == '.') &&
               i < sizeof(token.value) - 1) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        size_t i = 0;
        lexer->position++;
        lexer->column++;
        while (lexer->position < lexer->input_length && 
               lexer->input[lexer->position] != '"' &&
               i < sizeof(token.value) - 1) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
        if (lexer->position < lexer->input_length && lexer->input[lexer->position] == '"') {
            lexer->position++;
            lexer->column++;
        }
    } else if (strchr("+-*/=<>!&|", current)) {
        token.type = TOKEN_OPERATOR;
        size_t i = 0;
        while (lexer->position < lexer->input_length && 
               strchr("+-*/=<>!&|", lexer->input[lexer->position]) &&
               i < sizeof(token.value) - 1) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }