//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

enum TokenType {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_UNKNOWN
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
    uint32_t line;
    uint32_t column;
};

struct LexerState {
    const char *input;
    uint32_t position;
    uint32_t line;
    uint32_t column;
    uint32_t token_count;
};

struct ParserState {
    struct Token *tokens;
    uint32_t token_index;
    uint32_t token_count;
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
    lexer->token_count = 0;
}

void skip_whitespace(struct LexerState *lexer) {
    if (lexer == NULL || lexer->input == NULL) return;
    
    while (lexer->input[lexer->position] != '\0') {
        char c = lexer->input[lexer->position];
        if (c == ' ' || c == '\t') {
            lexer->position++;
            lexer->column++;
        } else if (c == '\n') {
            lexer->position++;
            lexer->line++;
            lexer->column = 1;
        } else {
            break;
        }
    }
}

int get_next_token(struct LexerState *lexer, struct Token *token) {
    if (lexer == NULL || token == NULL || lexer->input == NULL) return 0;
    
    skip_whitespace(lexer);
    
    if (lexer->input[lexer->position] == '\0') {
        token->type = TOKEN_EOF;
        strncpy(token->value, "", MAX_TOKEN_LEN - 1);
        token->value[MAX_TOKEN_LEN - 1] = '\0';
        token->line = lexer->line;
        token->column = lexer->column;
        return 1;
    }
    
    char current = lexer->input[lexer->position];
    token->line = lexer->line;
    token->column = lexer->column;
    
    if (isalpha(current) || current == '_') {
        token->type = TOKEN_IDENTIFIER;
        uint32_t len = 0;
        while ((isalnum(lexer->input[lexer->position]) || 
                lexer->input[lexer->position] == '_') && 
               len < MAX_TOKEN_LEN - 1) {
            token->value[len++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token->value[len] = '\0';
    } else if (isdigit(current)) {
        token->type = TOKEN_NUMBER;
        uint32_t len = 0;
        while (isdigit(lexer->input[lexer->position]) && 
               len < MAX_TOKEN_LEN - 1) {
            token->value[len++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token->value[len] = '\0';
    } else if (current == '"') {
        token->type = TOKEN_STRING;
        uint32_t len = 0;
        lexer->position++;
        lexer->column++;
        while (lexer->input[lexer->position] != '"' && 
               lexer->input[lexer->position] != '\0' && 
               len < MAX_TOKEN_LEN - 1) {
            token->value[len++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token->value[len] = '\0';
        if (lexer->input[lexer->position] == '"') {
            lexer->position++;
            lexer->column++;
        }
    } else if (is_operator_char(current)) {
        token->type = TOKEN_OPERATOR;
        uint32_t len = 0;
        while (is_operator_char(lexer->input[lexer->position]) && 
               len < MAX_TOKEN_LEN - 1) {
            token->value[len++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token->value[len] = '\0';
    } else if (is_delimiter_char(current)) {
        token->type = TOKEN_DELIMITER;
        token->value[0] = lexer->input[lexer->position++];
        token->value[1] = '\0';
        lexer->column++;
    } else {
        token->type = TOKEN_UNKNOWN;
        token->value[0] = lexer->input[lexer->position++];
        token->value[1