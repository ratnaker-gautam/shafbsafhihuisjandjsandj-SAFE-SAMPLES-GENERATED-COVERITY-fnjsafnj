//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
    int line;
    int column;
};

struct Lexer {
    const char *input;
    int position;
    int line;
    int column;
    int error_count;
};

struct Parser {
    struct Lexer *lexer;
    struct Token current_token;
    int error_count;
};

void lexer_init(struct Lexer *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->error_count = 0;
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int is_delimiter(char c) {
    return strchr("(){}[],;", c) != NULL;
}

struct Token lexer_next_token(struct Lexer *lexer) {
    struct Token token;
    if (lexer == NULL) {
        token.type = TOKEN_UNKNOWN;
        token.line = 1;
        token.column = 1;
        memset(token.value, 0, MAX_TOKEN_LEN);
        return token;
    }
    
    token.line = lexer->line;
    token.column = lexer->column;
    memset(token.value, 0, MAX_TOKEN_LEN);
    
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
    
    if (lexer->input[lexer->position] == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char current = lexer->input[lexer->position];
    int value_pos = 0;
    
    if (isalpha((unsigned char)current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        while ((isalnum((unsigned char)current) || current == '_') && value_pos < MAX_TOKEN_LEN - 1) {
            token.value[value_pos++] = current;
            lexer->position++;
            lexer->column++;
            current = lexer->input[lexer->position];
        }
    } else if (isdigit((unsigned char)current)) {
        token.type = TOKEN_NUMBER;
        while (isdigit((unsigned char)current) && value_pos < MAX_TOKEN_LEN - 1) {
            token.value[value_pos++] = current;
            lexer->position++;
            lexer->column++;
            current = lexer->input[lexer->position];
        }
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        lexer->position++;
        lexer->column++;
        current = lexer->input[lexer->position];
        
        while (current != '"' && current != '\0' && value_pos < MAX_TOKEN_LEN - 1) {
            token.value[value_pos++] = current;
            lexer->position++;
            lexer->column++;
            current = lexer->input[lexer->position];
        }
        
        if (current == '"') {
            lexer->position++;
            lexer->column++;
        } else {
            lexer->error_count++;
        }
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        while (is_operator_char(current) && value_pos < MAX_TOKEN_LEN - 1) {
            token.value[value_pos++] = current;
            lexer->position++;
            lexer->column++;
            current = lexer->input[lexer->position];
        }
    } else if (is_delimiter(current)) {
        token.type = TOKEN_DELIMITER;
        token.value[value_pos++] = current;
        lexer->position++;
        lexer->column++;
    } else {
        token.type = TOKEN_UNKNOWN;
        token.value[value_pos++] = current;
        lexer->position++;
        lexer->column++;
        lexer->error_count++;
    }
    
    token.value[value_pos] = '\0';
    return token;
}

void parser_init(struct Parser *parser, struct Lexer *lexer) {
    if (parser == NULL || lexer == NULL) return;
    parser->lexer = lexer;
    parser->current_token = lexer_next_token(lexer);
    parser->error_count = 0;
}

void parser_advance(struct Parser *parser) {
    if (parser == NULL || parser->lexer == NULL) return;
    parser->current_token = lexer_next_token(parser->lexer);
}

void parser_parse_expression(struct Parser *parser) {
    if (parser == NULL) return;
    if (parser->current_token.type == TOKEN_N