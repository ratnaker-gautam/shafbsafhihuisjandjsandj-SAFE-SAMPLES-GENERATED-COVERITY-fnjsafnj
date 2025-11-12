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
    char error_message[128];
};

static const char *keywords[] = {
    "if", "else", "while", "for", "return", "int", "char", "void"
};
static const size_t num_keywords = sizeof(keywords) / sizeof(keywords[0]);

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
    for (size_t i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

struct Token get_next_token(struct LexerState *lexer) {
    struct Token token = {TOKEN_ERROR, "", lexer->line, lexer->column};
    
    if (lexer == NULL || lexer->input == NULL) {
        strcpy(token.value, "Invalid lexer state");
        return token;
    }
    
    if (lexer->position >= lexer->input_length) {
        token.type = TOKEN_EOF;
        strcpy(token.value, "EOF");
        return token;
    }
    
    char current = lexer->input[lexer->position];
    
    while (isspace((unsigned char)current)) {
        if (current == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
        if (lexer->position >= lexer->input_length) {
            token.type = TOKEN_EOF;
            strcpy(token.value, "EOF");
            return token;
        }
        current = lexer->input[lexer->position];
    }
    
    token.line = lexer->line;
    token.column = lexer->column;
    
    if (isalpha((unsigned char)current) || current == '_') {
        size_t value_index = 0;
        while ((isalnum((unsigned char)current) || current == '_') && value_index < 63) {
            token.value[value_index++] = current;
            lexer->position++;
            lexer->column++;
            if (lexer->position >= lexer->input_length) break;
            current = lexer->input[lexer->position];
        }
        token.value[value_index] = '\0';
        token.type = is_keyword(token.value) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
    } else if (isdigit((unsigned char)current)) {
        size_t value_index = 0;
        while (isdigit((unsigned char)current) && value_index < 63) {
            token.value[value_index++] = current;
            lexer->position++;
            lexer->column++;
            if (lexer->position >= lexer->input_length) break;
            current = lexer->input[lexer->position];
        }
        token.value[value_index] = '\0';
        token.type = TOKEN_NUMBER;
    } else if (current == '"') {
        size_t value_index = 0;
        lexer->position++;
        lexer->column++;
        if (lexer->position >= lexer->input_length) {
            token.type = TOKEN_ERROR;
            strcpy(token.value, "Unterminated string");
            return token;
        }
        current = lexer->input[lexer->position];
        
        while (current != '"' && value_index < 63) {
            if (lexer->position >= lexer->input_length) {
                token.type = TOKEN_ERROR;
                strcpy(token.value, "Unterminated string");
                return token;
            }
            token.value[value_index++] = current;
            lexer->position++;
            lexer->column++;
            if (lexer->position >= lexer->input_length) break;
            current = lexer->input[lexer->position];
        }
        
        if (current != '"') {
            token.type = TOKEN_ERROR;
            strcpy(token.value, "Unterminated string");
            return token;
        }
        
        token.value[value_index] = '\0';
        token.type = TOKEN_STRING;
        lexer->position++;
        lexer->column++;
    } else if (