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
    int token_count;
};

struct Parser {
    struct Lexer *lexer;
    struct Token current_token;
    int error_count;
};

void init_lexer(struct Lexer *lexer, const char *input) {
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->token_count = 0;
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int is_delimiter(char c) {
    return strchr("(){}[],;", c) != NULL;
}

void skip_whitespace(struct Lexer *lexer) {
    while (lexer->input[lexer->position] != '\0' && 
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

struct Token get_next_token(struct Lexer *lexer) {
    struct Token token = {TOKEN_UNKNOWN, "", lexer->line, lexer->column};
    
    skip_whitespace(lexer);
    
    if (lexer->input[lexer->position] == '\0') {
        token.type = TOKEN_EOF;
        strcpy(token.value, "EOF");
        return token;
    }
    
    char current = lexer->input[lexer->position];
    int value_pos = 0;
    
    if (isalpha(current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        while ((isalnum(current) || current == '_') && value_pos < MAX_TOKEN_LEN - 1) {
            token.value[value_pos++] = current;
            lexer->position++;
            lexer->column++;
            current = lexer->input[lexer->position];
        }
        token.value[value_pos] = '\0';
    } else if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        while (isdigit(current) && value_pos < MAX_TOKEN_LEN - 1) {
            token.value[value_pos++] = current;
            lexer->position++;
            lexer->column++;
            current = lexer->input[lexer->position];
        }
        token.value[value_pos] = '\0';
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        token.value[value_pos++] = current;
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
            token.value[value_pos++] = current;
            lexer->position++;
            lexer->column++;
        }
        token.value[value_pos] = '\0';
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        token.value[value_pos++] = current;
        lexer->position++;
        lexer->column++;
        
        current = lexer->input[lexer->position];
        if (is_operator_char(current) && value_pos < MAX_TOKEN_LEN - 1) {
            token.value[value_pos++] = current;
            lexer->position++;
            lexer->column++;
        }
        token.value[value_pos] = '\0';
    } else if (is_delimiter(current)) {
        token.type = TOKEN_DELIMITER;
        token.value[value_pos++] = current;
        lexer->position++;
        lexer->column++;
        token.value[value_pos] = '\0';
    } else {
        token.type = TOKEN_UNKNOWN;
        token.value[value_pos++] = current;
        lexer->position++;
        lexer->column++;
        token.value[value_pos] = '\0';
    }
    
    lexer->token_count++;
    return token;
}

void init_parser(struct Parser *parser, struct Lexer *lexer) {
    parser->lexer = lexer;
    parser->current_token = get_next_token(lexer);
    parser->error_count = 0;
}

void advance_parser(struct Parser *parser) {
    parser->current_token = get_next_token(parser->lexer);
}

void parse_expression(struct Parser *parser) {
    if (parser->current_token.type == TOKEN_NUMBER || 
        parser->current_token.type == TOKEN_IDENTIFIER) {
        advance_parser(