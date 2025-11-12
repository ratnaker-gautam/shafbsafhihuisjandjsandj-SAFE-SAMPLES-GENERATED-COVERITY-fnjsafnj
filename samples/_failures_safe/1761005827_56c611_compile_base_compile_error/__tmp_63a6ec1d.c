//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
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

void parser_init(struct Parser *parser, const char *input) {
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
    struct Token token = {TOKEN_ERROR, "", parser->line, parser->column};
    if (parser == NULL || parser->input == NULL) return token;
    
    skip_whitespace(parser);
    
    if (parser->input[parser->position] == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char current = parser->input[parser->position];
    int value_pos = 0;
    
    if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        while (parser->input[parser->position] != '\0' && isdigit(parser->input[parser->position])) {
            if (value_pos < 31) {
                token.value[value_pos++] = parser->input[parser->position];
            }
            parser->position++;
            parser->column++;
        }
        token.value[value_pos] = '\0';
    } else if (isalpha(current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        while (parser->input[parser->position] != '\0' && 
               (isalnum(parser->input[parser->position]) || parser->input[parser->position] == '_')) {
            if (value_pos < 31) {
                token.value[value_pos++] = parser->input[parser->position];
            }
            parser->position++;
            parser->column++;
        }
        token.value[value_pos] = '\0';
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        if (value_pos < 31) {
            token.value[value_pos++] = current;
        }
        parser->position++;
        parser->column++;
        token.value[value_pos] = '\0';
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        parser->position++;
        parser->column++;
        
        while (parser->input[parser->position] != '\0' && parser->input[parser->position] != '"') {
            if (value_pos < 31) {
                token.value[value_pos++] = parser->input[parser->position];
            }
            parser->position++;
            parser->column++;
        }
        
        if (parser->input[parser->position] == '"') {
            parser->position++;
            parser->column++;
        }
        token.value[value_pos] = '\0';
    } else {
        token.type = TOKEN_ERROR;
        if (value_pos < 31) {
            token.value[value_pos++] = current;
        }
        parser->position++;
        parser->column++;
        token.value[value_pos] = '\0';
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
                printf("String: \"%s\" at line %d