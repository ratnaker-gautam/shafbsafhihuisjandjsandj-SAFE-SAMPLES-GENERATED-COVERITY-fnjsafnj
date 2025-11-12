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
    struct Token token = {TOKEN_ERROR, "", 0, 0};
    if (parser == NULL || parser->input == NULL) return token;
    
    skip_whitespace(parser);
    
    int start_pos = parser->position;
    int start_line = parser->line;
    int start_col = parser->column;
    
    if (parser->input[parser->position] == '\0') {
        token.type = TOKEN_EOF;
        token.line = start_line;
        token.column = start_col;
        return token;
    }
    
    char current = parser->input[parser->position];
    
    if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (parser->input[parser->position] != '\0' && 
               i < 31 && 
               isdigit(parser->input[parser->position])) {
            token.value[i++] = parser->input[parser->position++];
            parser->column++;
        }
        token.value[i] = '\0';
    } else if (isalpha(current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (parser->input[parser->position] != '\0' && 
               i < 31 && 
               (isalnum(parser->input[parser->position]) || 
                parser->input[parser->position] == '_')) {
            token.value[i++] = parser->input[parser->position++];
            parser->column++;
        }
        token.value[i] = '\0';
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (parser->input[parser->position] != '\0' && 
               i < 31 && 
               is_operator_char(parser->input[parser->position])) {
            token.value[i++] = parser->input[parser->position++];
            parser->column++;
        }
        token.value[i] = '\0';
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        token.value[0] = '"';
        parser->position++;
        parser->column++;
        int i = 1;
        
        while (parser->input[parser->position] != '\0' && 
               i < 30 && 
               parser->input[parser->position] != '"') {
            token.value[i++] = parser->input[parser->position++];
            parser->column++;
        }
        
        if (parser->input[parser->position] == '"') {
            token.value[i++] = '"';
            parser->position++;
            parser->column++;
            token.value[i] = '\0';
        } else {
            token.type = TOKEN_ERROR;
        }
    } else {
        token.type = TOKEN_ERROR;
        token.value[0] = current;
        token.value[1] = '\0';
        parser->position++;
        parser->column++;
    }
    
    token.line = start_line;
    token.column = start_col;
    return token;
}

const char* token_type_to_string(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

void parse_expression(struct Parser *parser) {
    if (parser == NULL) return;
    
    parser->current_token = get_next_token(parser);
    
    while (parser->current_token.type != TOKEN_EOF