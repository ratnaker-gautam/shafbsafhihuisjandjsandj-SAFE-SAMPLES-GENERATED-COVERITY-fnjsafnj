//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64
#define MAX_INPUT_LEN 1024

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

void init_lexer(struct LexerState *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->token_count = 0;
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int is_delimiter_char(char c) {
    return strchr("(),;{}[]", c) != NULL;
}

void skip_whitespace(struct LexerState *lexer) {
    if (lexer == NULL || lexer->input == NULL) return;
    while (lexer->input[lexer->position] != '\0' && 
           isspace((unsigned char)lexer->input[lexer->position])) {
        if (lexer->input[lexer->position] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
    }
}

int read_string(struct LexerState *lexer, struct Token *token) {
    if (lexer == NULL || token == NULL || lexer->input == NULL) return 0;
    if (lexer->input[lexer->position] != '"') return 0;
    
    token->type = TOKEN_STRING;
    token->line = lexer->line;
    token->column = lexer->column;
    
    uint32_t len = 0;
    lexer->position++;
    lexer->column++;
    
    while (lexer->input[lexer->position] != '\0' && 
           lexer->input[lexer->position] != '"' && 
           len < MAX_TOKEN_LEN - 1) {
        token->value[len++] = lexer->input[lexer->position++];
        lexer->column++;
    }
    
    if (lexer->input[lexer->position] == '"') {
        lexer->position++;
        lexer->column++;
    }
    
    token->value[len] = '\0';
    return 1;
}

int read_number(struct LexerState *lexer, struct Token *token) {
    if (lexer == NULL || token == NULL || lexer->input == NULL) return 0;
    if (!isdigit((unsigned char)lexer->input[lexer->position])) return 0;
    
    token->type = TOKEN_NUMBER;
    token->line = lexer->line;
    token->column = lexer->column;
    
    uint32_t len = 0;
    while (lexer->input[lexer->position] != '\0' && 
           isdigit((unsigned char)lexer->input[lexer->position]) && 
           len < MAX_TOKEN_LEN - 1) {
        token->value[len++] = lexer->input[lexer->position++];
        lexer->column++;
    }
    
    token->value[len] = '\0';
    return 1;
}

int read_identifier(struct LexerState *lexer, struct Token *token) {
    if (lexer == NULL || token == NULL || lexer->input == NULL) return 0;
    if (!isalpha((unsigned char)lexer->input[lexer->position]) && 
        lexer->input[lexer->position] != '_') return 0;
    
    token->type = TOKEN_IDENTIFIER;
    token->line = lexer->line;
    token->column = lexer->column;
    
    uint32_t len = 0;
    while (lexer->input[lexer->position] != '\0' && 
           (isalnum((unsigned char)lexer->input[lexer->position]) || 
            lexer->input[lexer->position] == '_') && 
           len < MAX_TOKEN_LEN - 1) {
        token->value[len++] = lexer->input[lexer->position++];
        lexer->column++;
    }
    
    token->value[len] = '\0';
    return 1;
}

int read_operator(struct LexerState *lexer, struct Token *token) {
    if (lexer == NULL || token == NULL || lexer->input == NULL) return 0;
    if (!is_operator_char(lexer->input[lexer->position])) return 0;
    
    token->type = TOKEN_OPERATOR;
    token->line