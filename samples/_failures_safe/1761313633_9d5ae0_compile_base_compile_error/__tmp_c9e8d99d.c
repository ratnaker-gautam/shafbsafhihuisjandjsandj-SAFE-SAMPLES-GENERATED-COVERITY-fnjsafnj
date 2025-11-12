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

int is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int is_delimiter(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ';' || c == ',';
}

int is_keyword(const char *str) {
    const char *keywords[] = {"if", "else", "while", "for", "return", "int", "float", "char", "void"};
    size_t num_keywords = sizeof(keywords) / sizeof(keywords[0]);
    for (size_t i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0) return 1;
    }
    return 0;
}

void get_next_token(struct LexerState *lexer, struct Token *token) {
    if (lexer == NULL || token == NULL) return;
    if (lexer->position >= lexer->input_length) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        token->line = lexer->line;
        token->column = lexer->column;
        return;
    }

    char current_char = lexer->input[lexer->position];
    while (is_whitespace(current_char)) {
        if (current_char == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
        if (lexer->position >= lexer->input_length) {
            token->type = TOKEN_EOF;
            token->value[0] = '\0';
            token->line = lexer->line;
            token->column = lexer->column;
            return;
        }
        current_char = lexer->input[lexer->position];
    }

    token->line = lexer->line;
    token->column = lexer->column;

    if (isalpha(current_char) || current_char == '_') {
        token->type = TOKEN_IDENTIFIER;
        size_t i = 0;
        while ((isalnum(current_char) || current_char == '_') && i < 63) {
            token->value[i++] = current_char;
            lexer->position++;
            lexer->column++;
            if (lexer->position >= lexer->input_length) break;
            current_char = lexer->input[lexer->position];
        }
        token->value[i] = '\0';
        if (is_keyword(token->value)) token->type = TOKEN_KEYWORD;
    } else if (isdigit(current_char)) {
        token->type = TOKEN_NUMBER;
        size_t i = 0;
        while (isdigit(current_char) && i < 63) {
            token->value[i++] = current_char;
            lexer->position++;
            lexer->column++;
            if (lexer->position >= lexer->input_length) break;
            current_char = lexer->input[lexer->position];
        }
        token->value[i] = '\0';
    } else if (current_char == '"') {
        token->type = TOKEN_STRING;
        size_t i = 0;
        lexer->position++;
        lexer->column++;
        if (lexer->position >= lexer->input_length) {
            token->type = TOKEN_INVALID;
            token->value[0] = '\0';
            return;
        }
        current_char = lexer->input[lexer->position];
        while (current_char != '"' && i < 63) {
            if (lexer->position >= lexer->input_length) {
                token->type = TOKEN_INVALID;
                break;
            }
            token->value[i++] = current_char;
            lexer->position