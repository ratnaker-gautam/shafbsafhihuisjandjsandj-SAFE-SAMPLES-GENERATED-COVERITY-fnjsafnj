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

struct KeywordTable {
    const char *keyword;
    enum TokenType type;
};

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int is_delimiter_char(char c) {
    return strchr("(),;{}[]", c) != NULL;
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

struct Token get_next_token(struct LexerState *lexer) {
    struct Token token = {TOKEN_INVALID, "", lexer->line, lexer->column};
    if (lexer == NULL) return token;
    
    skip_whitespace(lexer);
    if (lexer->position >= lexer->input_length) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char current_char = lexer->input[lexer->position];
    uint32_t start_column = lexer->column;
    
    if (isalpha(current_char) || current_char == '_') {
        size_t i = 0;
        while (lexer->position < lexer->input_length && 
               (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') &&
               i < sizeof(token.value) - 1) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
        token.type = TOKEN_IDENTIFIER;
        
        struct KeywordTable keywords[] = {
            {"if", TOKEN_KEYWORD},
            {"else", TOKEN_KEYWORD},
            {"while", TOKEN_KEYWORD},
            {"return", TOKEN_KEYWORD},
            {"int", TOKEN_KEYWORD},
            {"char", TOKEN_KEYWORD},
            {NULL, TOKEN_IDENTIFIER}
        };
        
        for (int j = 0; keywords[j].keyword != NULL; j++) {
            if (strcmp(token.value, keywords[j].keyword) == 0) {
                token.type = keywords[j].type;
                break;
            }
        }
    }
    else if (isdigit(current_char)) {
        size_t i = 0;
        while (lexer->position < lexer->input_length && 
               isdigit(lexer->input[lexer->position]) &&
               i < sizeof(token.value) - 1) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
        token.type = TOKEN_NUMBER;
    }
    else if (current_char == '"') {
        size_t i = 0;
        token.value[i++] = lexer->input[lexer->position++];
        lexer->column++;
        
        while (lexer->position < lexer->input_length && 
               lexer->input[lexer->position] != '"' &&
               i < sizeof(token.value) - 1) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        
        if (lexer->position < lexer->input_length && lexer->input[lexer->position] == '"') {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
        token.type = TOKEN_STRING;
    }
    else if (is_operator_char(current_char)) {
        size_t i = 0;
        while (lexer->position < lexer->input_length && 
               is_operator_char(lexer->input[lexer->position]) &&
               i < sizeof(token.value) - 1) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
        token.type = TOKEN_OPERATOR;
    }