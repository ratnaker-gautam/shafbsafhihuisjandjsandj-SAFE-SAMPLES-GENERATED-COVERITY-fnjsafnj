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
    uint32_t line_declared;
};

struct SymbolTable {
    struct SymbolTableEntry entries[256];
    uint32_t count;
};

struct ParseTree {
    char node_type[32];
    char value[64];
    struct ParseTree *children[8];
    uint32_t child_count;
};

void init_lexer(struct LexerState *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->input_length = strlen(input);
}

int is_keyword(const char *str) {
    const char *keywords[] = {"if", "else", "while", "for", "return", "int", "float", "char"};
    const uint32_t num_keywords = 8;
    for (uint32_t i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

struct Token get_next_token(struct LexerState *lexer) {
    struct Token token = {TOKEN_INVALID, "", lexer->line, lexer->column};
    if (lexer == NULL || lexer->input == NULL) return token;
    
    while (lexer->position < lexer->input_length) {
        char current = lexer->input[lexer->position];
        
        if (isspace(current)) {
            if (current == '\n') {
                lexer->line++;
                lexer->column = 1;
            } else {
                lexer->column++;
            }
            lexer->position++;
            continue;
        }
        
        if (isalpha(current) || current == '_') {
            uint32_t i = 0;
            while ((lexer->position < lexer->input_length) && 
                   (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') &&
                   (i < 63)) {
                token.value[i++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            token.value[i] = '\0';
            token.type = is_keyword(token.value) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
            return token;
        }
        
        if (isdigit(current)) {
            uint32_t i = 0;
            while ((lexer->position < lexer->input_length) && 
                   (isdigit(lexer->input[lexer->position]) || lexer->input[lexer->position] == '.') &&
                   (i < 63)) {
                token.value[i++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            token.value[i] = '\0';
            token.type = TOKEN_NUMBER;
            return token;
        }
        
        if (current == '"') {
            uint32_t i = 0;
            lexer->position++;
            lexer->column++;
            while ((lexer->position < lexer->input_length) && 
                   (lexer->input[lexer->position] != '"') &&
                   (i < 63)) {
                token.value[i++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            token.value[i] = '\0';
            if (lexer->position < lexer->input_length && lexer->input[lexer->position] == '"') {
                lexer->position++;
                lexer->column++;
            }
            token.type = TOKEN_STRING;
            return token;
        }
        
        if (strchr("+-*/=<>!&|", current) != NULL) {
            uint32_t i = 0;
            token.value[i++] = current;
            lexer->position++;
            lexer->column++;
            if (lexer->position < lexer->input_length && strchr("=+-", lexer->input[lexer->position]) != NULL) {
                token.value[i++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            token.value[i] = '\0';
            token.type = TOKEN_OPERATOR;
            return token;
        }
        
        if (strchr("(),;{}[]", current) != NULL) {
            token.value[0] = current;
            token.value[1] = '\0';
            token.type = TOKEN_DELIMITER;
            lexer->position++;
            lexer->column++;
            return token;
        }
        
        token.value