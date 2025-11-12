//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
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

void symbol_table_init(struct SymbolTable *table) {
    table->count = 0;
}

int symbol_table_add(struct SymbolTable *table, const char *name, enum TokenType type, uint32_t line) {
    if (table->count >= 256) return 0;
    if (strlen(name) >= 64) return 0;
    
    for (uint32_t i = 0; i < table->count; i++) {
        if (strcmp(table->entries[i].name, name) == 0) {
            return 0;
        }
    }
    
    strcpy(table->entries[table->count].name, name);
    table->entries[table->count].type = type;
    table->entries[table->count].declared_line = line;
    table->count++;
    return 1;
}

void lexer_init(struct LexerState *lexer, const char *input) {
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->input_length = strlen(input);
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int is_keyword(const char *str) {
    const char *keywords[] = {"if", "else", "while", "for", "return", "int", "float", "char", "void"};
    for (int i = 0; i < 9; i++) {
        if (strcmp(str, keywords[i]) == 0) return 1;
    }
    return 0;
}

struct Token lexer_next_token(struct LexerState *lexer) {
    struct Token token = {TOKEN_INVALID, "", lexer->line, lexer->column};
    
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
    token.line = lexer->line;
    token.column = lexer->column;
    
    if (isalpha(current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while ((lexer->position < lexer->input_length) && 
               (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') &&
               i < 63) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
        if (is_keyword(token.value)) {
            token.type = TOKEN_KEYWORD;
        }
    } else if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while ((lexer->position < lexer->input_length) && 
               (isdigit(lexer->input[lexer->position]) || lexer->input[lexer->position] == '.') &&
               i < 63) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        int i = 0;
        lexer->position++;
        lexer->column++;
        while ((lexer->position < lexer->input_length) && 
               (lexer->input[lexer->position] != '"') && i < 63) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
        if (lexer->position < lexer->input_length && lexer->input[lexer->position] == '"') {
            lexer->position++;
            lexer->column++;
        }
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while ((lexer->position <