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
    struct Token current_token;
    struct LexerState *lexer;
};

struct SymbolTableEntry {
    char name[64];
    enum TokenType type;
    uint32_t declared_line;
};

struct SymbolTable {
    struct SymbolTableEntry entries[256];
    uint32_t count;
};

void initialize_lexer(struct LexerState *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->input_length = strlen(input);
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

struct Token get_next_token(struct LexerState *lexer) {
    struct Token token = {TOKEN_INVALID, "", lexer->line, lexer->column};
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
    
    char current_char = lexer->input[lexer->position];
    size_t value_index = 0;
    
    if (isalpha(current_char) || current_char == '_') {
        token.type = TOKEN_IDENTIFIER;
        while (lexer->position < lexer->input_length && 
               (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_')) {
            if (value_index < sizeof(token.value) - 1) {
                token.value[value_index++] = lexer->input[lexer->position];
            }
            lexer->position++;
            lexer->column++;
        }
        token.value[value_index] = '\0';
        if (is_keyword(token.value)) {
            token.type = TOKEN_KEYWORD;
        }
    } else if (isdigit(current_char)) {
        token.type = TOKEN_NUMBER;
        while (lexer->position < lexer->input_length && 
               (isdigit(lexer->input[lexer->position]) || lexer->input[lexer->position] == '.')) {
            if (value_index < sizeof(token.value) - 1) {
                token.value[value_index++] = lexer->input[lexer->position];
            }
            lexer->position++;
            lexer->column++;
        }
        token.value[value_index] = '\0';
    } else if (current_char == '"') {
        token.type = TOKEN_STRING;
        lexer->position++;
        lexer->column++;
        while (lexer->position < lexer->input_length && lexer->input[lexer->position] != '"') {
            if (value_index < sizeof(token.value) - 1) {
                token.value[value_index++] = lexer->input[lexer->position];
            }
            lexer->position++;
            lexer->column++;
        }
        token.value[value_index] = '\0';
        if (lexer->position < lexer->input_length && lexer->input[lexer->position] == '"') {
            lexer->position++;
            lexer->column++;
        }
    } else if (strchr("+-*/=<>!&|", current_char) != NULL) {
        token.type = TOKEN_OPERATOR;
        if (value_index < sizeof(token.value) - 1) {
            token.value[value_index++] = current_char;
        }
        lexer->position++;
        lexer->column++;
        if (lexer->position < lexer->input_length && strchr("=+-", lexer->input[lexer->position]) != NULL) {
            if (value_index < sizeof(token.value) - 1) {
                token.value[value_index++] = lexer->input[lexer->position];
            }
            lexer->position++;
            lexer->column++;
        }
        token.value[value_index