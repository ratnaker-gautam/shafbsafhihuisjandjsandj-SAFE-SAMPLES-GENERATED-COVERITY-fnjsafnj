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
    TOKEN_INVALID
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
    parser->current_token.type = TOKEN_INVALID;
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
        struct Token invalid = {TOKEN_INVALID, "", 0, 0};
        return invalid;
    }
    
    skip_whitespace(parser);
    
    if (parser->input[parser->position] == '\0') {
        struct Token eof = {TOKEN_EOF, "", parser->line, parser->column};
        return eof;
    }
    
    char current = parser->input[parser->position];
    int start_pos = parser->position;
    int start_line = parser->line;
    int start_col = parser->column;
    
    if (isdigit(current)) {
        struct Token token = {TOKEN_NUMBER, "", start_line, start_col};
        int i = 0;
        
        while (parser->input[parser->position] != '\0' && 
               i < (int)sizeof(token.value) - 1 && 
               isdigit(parser->input[parser->position])) {
            token.value[i++] = parser->input[parser->position++];
            parser->column++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    if (isalpha(current) || current == '_') {
        struct Token token = {TOKEN_IDENTIFIER, "", start_line, start_col};
        int i = 0;
        
        while (parser->input[parser->position] != '\0' && 
               i < (int)sizeof(token.value) - 1 && 
               (isalnum(parser->input[parser->position]) || parser->input[parser->position] == '_')) {
            token.value[i++] = parser->input[parser->position++];
            parser->column++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    if (is_operator_char(current)) {
        struct Token token = {TOKEN_OPERATOR, "", start_line, start_col};
        token.value[0] = current;
        token.value[1] = '\0';
        parser->position++;
        parser->column++;
        return token;
    }
    
    if (current == '"') {
        struct Token token = {TOKEN_STRING, "", start_line, start_col};
        int i = 0;
        parser->position++;
        parser->column++;
        
        while (parser->input[parser->position] != '\0' && 
               i < (int)sizeof(token.value) - 1 && 
               parser->input[parser->position] != '"') {
            token.value[i++] = parser->input[parser->position++];
            parser->column++;
        }
        
        if (parser->input[parser->position] == '"') {
            parser->position++;
            parser->column++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    struct Token invalid = {TOKEN_INVALID, "", start_line, start_col};
    invalid.value[0] = current;
    invalid.value[1] = '\0';
    parser->position++;
    parser->column++;
    return invalid;
}

void parse_expression(struct Parser *parser) {
    if (parser == NULL) return;
    
    parser->current_token = get_next_token(parser);
    
    while (parser->current_token.type != TOKEN_EOF) {
        switch (parser->current_token.type) {
            case TOKEN_NUMBER:
                printf("Number: %s at line %d, column %d\n", 
                       parser->current_token.value, 
                       parser->current_token.line, 
                       parser->current_token.column);
                break;
            case TOKEN_IDENTIFIER:
                printf("Identifier: %s at line %d, column %d\n", 
                       parser->current_token.value, 
                       parser->current_token.line,