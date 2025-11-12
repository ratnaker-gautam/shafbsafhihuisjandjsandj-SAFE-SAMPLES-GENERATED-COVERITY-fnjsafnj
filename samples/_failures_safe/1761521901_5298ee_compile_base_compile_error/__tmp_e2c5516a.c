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

char lexer_peek(struct LexerState *lexer) {
    if (lexer->position >= lexer->input_length) return '\0';
    return lexer->input[lexer->position];
}

char lexer_advance(struct LexerState *lexer) {
    if (lexer->position >= lexer->input_length) return '\0';
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

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int is_delimiter_char(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == ';' || c == ',' || c == ':';
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
    
    lexer_skip_whitespace(lexer);
    if (lexer->position >= lexer->input_length) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char c = lexer_peek(lexer);
    token.line = lexer->line;
    token.column = lexer->column;
    
    if (isalpha(c) || c == '_') {
        uint32_t i = 0;
        while ((isalnum(c) || c == '_') && i < 63) {
            token.value[i++] = lexer_advance(lexer);
            c = lexer_peek(lexer);
        }
        token.value[i] = '\0';
        token.type = is_keyword(token.value) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
    } else if (isdigit(c)) {
        uint32_t i = 0;
        while (isdigit(c) && i < 63) {
            token.value[i++] = lexer_advance(lexer);
            c = lexer_peek(lexer);
        }
        token.value[i] = '\0';
        token.type = TOKEN_NUMBER;
    } else if (c == '"') {
        lexer_advance(lexer);
        uint32_t i = 0;
        c = lexer_peek(lexer);
        while (c != '"' && c != '\0' && i < 63) {
            token.value[i++] = lexer_