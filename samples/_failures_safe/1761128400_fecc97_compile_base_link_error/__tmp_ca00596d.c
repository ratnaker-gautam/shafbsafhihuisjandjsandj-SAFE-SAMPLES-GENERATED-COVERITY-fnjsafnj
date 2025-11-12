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

void parser_init(struct Parser *parser, const char *input) {
    if (parser == NULL || input == NULL) return;
    parser->input = input;
    parser->position = 0;
    parser->line = 1;
    parser->column = 1;
    parser->current_token.type = TOKEN_INVALID;
    memset(parser->current_token.value, 0, sizeof(parser->current_token.value));
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(struct Parser *parser) {
    if (parser == NULL || parser->input == NULL) return;
    while (parser->input[parser->position] != '\0' && isspace(parser->input[parser->position])) {
        if (parser->input[parser->position] == '\n') {
            parser->line++;
            parser->column = 1;
        } else {
            parser->column++;
        }
        parser->position++;
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
    
    struct Token token;
    token.line = parser->line;
    token.column = parser->column;
    memset(token.value, 0, sizeof(token.value));
    
    char current = parser->input[parser->position];
    
    if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (i < 31 && parser->input[parser->position] != '\0' && 
               isdigit(parser->input[parser->position])) {
            token.value[i++] = parser->input[parser->position++];
            parser->column++;
        }
        token.value[i] = '\0';
    } else if (isalpha(current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (i < 31 && parser->input[parser->position] != '\0' && 
               (isalnum(parser->input[parser->position]) || parser->input[parser->position] == '_')) {
            token.value[i++] = parser->input[parser->position++];
            parser->column++;
        }
        token.value[i] = '\0';
    } else if (is_operator(current)) {
        token.type = TOKEN_OPERATOR;
        token.value[0] = parser->input[parser->position++];
        token.value[1] = '\0';
        parser->column++;
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        int i = 0;
        parser->position++;
        parser->column++;
        while (i < 31 && parser->input[parser->position] != '\0' && 
               parser->input[parser->position] != '"') {
            token.value[i++] = parser->input[parser->position++];
            parser->column++;
        }
        if (parser->input[parser->position] == '"') {
            parser->position++;
            parser->column++;
        }
        token.value[i] = '\0';
    } else {
        token.type = TOKEN_INVALID;
        token.value[0] = parser->input[parser->position++];
        token.value[1] = '\0';
        parser->column++;
    }
    
    return token;
}

void parse_expression(struct Parser *parser) {
    if (parser == NULL) return;
    
    struct Token token = parser->current_token;
    
    switch (token.type) {
        case TOKEN_NUMBER:
            printf("Found number: %s at line %d, column %d\n", token.value, token.line, token.column);
            break;
        case TOKEN_IDENTIFIER:
            printf("Found identifier: %s at line %d, column %d\n", token.value, token.line, token.column);
            break;
        case TOKEN_OPERATOR:
            printf("Found operator: %s at line %d, column %d\n", token.value, token.line, token.column);
            break;
        case TOKEN_STRING:
            printf("Found string: \"%s\" at line %d, column %d\n", token.value, token.line, token.column);
            break;
        case TOKEN_INVALID:
            printf("Invalid token: %s at line %d, column %d\n", token.value, token.line, token.column);
            break;
        case TOKEN_EOF:
            printf("End of input reached\n");
            break;
    }
}