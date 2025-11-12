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
    char value[32];
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
    return (c == '+' || c == '-' || c == '*' || c == '/' || 
            c == '=' || c == '<' || c == '>' || c == '!' ||
            c == '&' || c == '|');
}

int is_delimiter_char(char c) {
    return (c == '(' || c == ')' || c == '{' || c == '}' ||
            c == '[' || c == ']' || c == ';' || c == ',' || c == '.');
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
    
    while (lexer->position < lexer->input_length && 
           isspace(lexer->input[lexer->position])) {
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
    
    if (lexer == NULL || lexer->position >= lexer->input_length) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    skip_whitespace(lexer);
    
    if (lexer->position >= lexer->input_length) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char current_char = lexer->input[lexer->position];
    
    if (isalpha(current_char) || current_char == '_') {
        token.type = TOKEN_IDENTIFIER;
        uint32_t i = 0;
        while (lexer->position < lexer->input_length && 
               i < sizeof(token.value) - 1 &&
               (isalnum(lexer->input[lexer->position]) || 
                lexer->input[lexer->position] == '_')) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
        
        static struct KeywordTable keywords[] = {
            {"if", TOKEN_KEYWORD},
            {"else", TOKEN_KEYWORD},
            {"while", TOKEN_KEYWORD},
            {"for", TOKEN_KEYWORD},
            {"return", TOKEN_KEYWORD},
            {"int", TOKEN_KEYWORD},
            {"char", TOKEN_KEYWORD},
            {"void", TOKEN_KEYWORD}
        };
        
        for (uint32_t j = 0; j < sizeof(keywords)/sizeof(keywords[0]); j++) {
            if (strcmp(token.value, keywords[j].keyword) == 0) {
                token.type = TOKEN_KEYWORD;
                break;
            }
        }
    }
    else if (isdigit(current_char)) {
        token.type = TOKEN_NUMBER;
        uint32_t i = 0;
        while (lexer->position < lexer->input_length && 
               i < sizeof(token.value) - 1 &&
               isdigit(lexer->input[lexer->position])) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
    }
    else if (current_char == '"') {
        token.type = TOKEN_STRING;
        uint32_t i = 0;
        lexer->position++;
        lexer->column++;
        
        while (lexer->position < lexer->input_length && 
               i < sizeof(token.value) - 1 &&
               lexer->input[lexer->position] != '"') {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        
        if (lexer->position < lexer->input_length && 
            lexer->input[lexer->position] == '"') {
            lexer->position++;
            lexer->column++;
        }
        token.value[i] = '\0';
    }
    else if (is_operator_char(current_char)) {
        token.type = TOKEN_OPERATOR;
        uint32_t i