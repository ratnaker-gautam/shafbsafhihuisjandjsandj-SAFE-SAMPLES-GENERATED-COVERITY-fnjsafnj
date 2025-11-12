//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

enum TokenType {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
    TOKEN_KEYWORD,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_UNKNOWN
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
    enum TokenType node_type;
    char value[64];
    struct ParserNode *left;
    struct ParserNode *right;
};

struct ParseResult {
    int success;
    struct ParserNode *ast;
    char error_msg[128];
};

const char *keywords[] = {"if", "else", "while", "for", "return", NULL};

int is_keyword(const char *str) {
    if (str == NULL) return 0;
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void init_lexer(struct LexerState *lexer, const char *input) {
    if (lexer == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    if (input != NULL) {
        lexer->input_length = strlen(input);
    } else {
        lexer->input_length = 0;
    }
}

void skip_whitespace(struct LexerState *lexer) {
    if (lexer == NULL || lexer->input == NULL) return;
    
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

int get_next_token(struct LexerState *lexer, struct Token *token) {
    if (lexer == NULL || token == NULL || lexer->input == NULL) return 0;
    
    skip_whitespace(lexer);
    
    if (lexer->position >= lexer->input_length) {
        token->type = TOKEN_EOF;
        strcpy(token->value, "");
        token->line = lexer->line;
        token->column = lexer->column;
        return 1;
    }
    
    char current = lexer->input[lexer->position];
    token->line = lexer->line;
    token->column = lexer->column;
    
    if (isalpha((unsigned char)current) || current == '_') {
        int i = 0;
        while ((lexer->position < lexer->input_length) && 
               (isalnum((unsigned char)lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') &&
               i < 63) {
            token->value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token->value[i] = '\0';
        
        if (is_keyword(token->value)) {
            token->type = TOKEN_KEYWORD;
        } else {
            token->type = TOKEN_IDENTIFIER;
        }
    }
    else if (isdigit((unsigned char)current)) {
        int i = 0;
        while ((lexer->position < lexer->input_length) && 
               (isdigit((unsigned char)lexer->input[lexer->position]) || lexer->input[lexer->position] == '.') &&
               i < 63) {
            token->value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_NUMBER;
    }
    else if (current == '"') {
        int i = 0;
        lexer->position++;
        lexer->column++;
        while ((lexer->position < lexer->input_length) && 
               (lexer->input[lexer->position] != '"') && i < 63) {
            token->value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        if (lexer->position < lexer->input_length && lexer->input[lexer->position] == '"') {
            lexer->position++;
            lexer->column++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_STRING;
    }
    else if (strchr("+-*/=<>!&|", current) != NULL) {
        token->value[0] = current;
        token->value[1] = '\0';
        lexer->position++;
        lexer->column++;
        token->type = TOKEN_OPERATOR;
    }
    else if (strchr("(),;{}[]", current) != NULL) {
        token->value[0] = current;