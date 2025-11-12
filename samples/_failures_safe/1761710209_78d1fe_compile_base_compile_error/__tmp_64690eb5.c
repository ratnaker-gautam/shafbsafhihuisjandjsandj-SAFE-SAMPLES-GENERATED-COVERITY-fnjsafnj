//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_ERROR
};

struct Token {
    enum TokenType type;
    char value[32];
    int line;
    int column;
};

struct Parser {
    const char *input;
    int position;
    int line;
    int column;
    struct Token current_token;
};

void init_parser(struct Parser *parser, const char *input) {
    if (parser == NULL || input == NULL) return;
    parser->input = input;
    parser->position = 0;
    parser->line = 1;
    parser->column = 1;
    parser->current_token.type = TOKEN_ERROR;
    memset(parser->current_token.value, 0, sizeof(parser->current_token.value));
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(struct Parser *parser) {
    if (parser == NULL || parser->input == NULL) return;
    
    while (parser->input[parser->position] != '\0') {
        char c = parser->input[parser->position];
        if (c == ' ' || c == '\t') {
            parser->position++;
            parser->column++;
        } else if (c == '\n') {
            parser->position++;
            parser->line++;
            parser->column = 1;
        } else {
            break;
        }
    }
}

struct Token get_next_token(struct Parser *parser) {
    if (parser == NULL || parser->input == NULL) {
        struct Token error_token = {TOKEN_ERROR, "", 0, 0};
        return error_token;
    }
    
    skip_whitespace(parser);
    
    if (parser->input[parser->position] == '\0') {
        struct Token eof_token = {TOKEN_EOF, "", parser->line, parser->column};
        return eof_token;
    }
    
    char current_char = parser->input[parser->position];
    struct Token token = {TOKEN_ERROR, "", parser->line, parser->column};
    
    if (isdigit(current_char)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (isdigit(parser->input[parser->position]) && i < 31) {
            token.value[i++] = parser->input[parser->position++];
            parser->column++;
        }
        token.value[i] = '\0';
    } else if (isalpha(current_char) || current_char == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while ((isalnum(parser->input[parser->position]) || parser->input[parser->position] == '_') && i < 31) {
            token.value[i++] = parser->input[parser->position++];
            parser->column++;
        }
        token.value[i] = '\0';
    } else if (is_operator_char(current_char)) {
        token.type = TOKEN_OPERATOR;
        token.value[0] = parser->input[parser->position++];
        token.value[1] = '\0';
        parser->column++;
    } else if (current_char == '"') {
        token.type = TOKEN_STRING;
        int i = 0;
        parser->position++;
        parser->column++;
        
        while (parser->input[parser->position] != '"' && parser->input[parser->position] != '\0' && i < 31) {
            token.value[i++] = parser->input[parser->position++];
            parser->column++;
        }
        
        if (parser->input[parser->position] == '"') {
            parser->position++;
            parser->column++;
        }
        token.value[i] = '\0';
    } else {
        token.type = TOKEN_ERROR;
        token.value[0] = parser->input[parser->position++];
        token.value[1] = '\0';
        parser->column++;
    }
    
    return token;
}

void parse_expression(struct Parser *parser) {
    if (parser == NULL) return;
    
    parser->current_token = get_next_token(parser);
    
    while (parser->current_token.type != TOKEN_EOF && parser->current_token.type != TOKEN_ERROR) {
        switch (parser->current_token.type) {
            case TOKEN_NUMBER:
                printf("Number: %s at line %d, column %d\n", 
                       parser->current_token.value, parser->current_token.line, parser->current_token.column);
                break;
            case TOKEN_IDENTIFIER:
                printf("Identifier: %s at line %d, column %d\n", 
                       parser->current_token.value, parser->current_token.line, parser->current_token.column);
                break;
            case TOKEN_OPERATOR:
                printf("Operator: %s at line %d, column %d\n", 
                       parser->current_token.value, parser->current_token.line, parser->current_token.column);
                break;
            case TOKEN_STRING:
                printf("String: \"%s\" at line %d, column %d\n", 
                       parser->current_token.value, parser->current_token.line, parser->current_token.column);
                break;
            case TOKEN_ERROR: