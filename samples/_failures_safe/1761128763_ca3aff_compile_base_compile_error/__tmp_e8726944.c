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

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void init_parser(struct Parser *parser, const char *input) {
    parser->input = input;
    parser->position = 0;
    parser->line = 1;
    parser->column = 1;
    parser->current_token.type = TOKEN_ERROR;
    memset(parser->current_token.value, 0, sizeof(parser->current_token.value));
}

char peek_char(struct Parser *parser) {
    if (parser->input == NULL || parser->position < 0) {
        return '\0';
    }
    if (parser->input[parser->position] == '\0') {
        return '\0';
    }
    return parser->input[parser->position];
}

char next_char(struct Parser *parser) {
    char c = peek_char(parser);
    if (c != '\0') {
        parser->position++;
        if (c == '\n') {
            parser->line++;
            parser->column = 1;
        } else {
            parser->column++;
        }
    }
    return c;
}

void skip_whitespace(struct Parser *parser) {
    char c;
    while ((c = peek_char(parser)) != '\0' && isspace(c)) {
        next_char(parser);
    }
}

struct Token read_number(struct Parser *parser) {
    struct Token token;
    token.type = TOKEN_NUMBER;
    token.line = parser->line;
    token.column = parser->column;
    memset(token.value, 0, sizeof(token.value));
    
    int i = 0;
    char c;
    while ((c = peek_char(parser)) != '\0' && isdigit(c)) {
        if (i < 31) {
            token.value[i++] = next_char(parser);
        } else {
            next_char(parser);
        }
    }
    return token;
}

struct Token read_identifier(struct Parser *parser) {
    struct Token token;
    token.type = TOKEN_IDENTIFIER;
    token.line = parser->line;
    token.column = parser->column;
    memset(token.value, 0, sizeof(token.value));
    
    int i = 0;
    char c;
    while ((c = peek_char(parser)) != '\0' && isalnum(c)) {
        if (i < 31) {
            token.value[i++] = next_char(parser);
        } else {
            next_char(parser);
        }
    }
    return token;
}

struct Token read_string(struct Parser *parser) {
    struct Token token;
    token.type = TOKEN_STRING;
    token.line = parser->line;
    token.column = parser->column;
    memset(token.value, 0, sizeof(token.value));
    
    next_char(parser);
    int i = 0;
    char c;
    while ((c = peek_char(parser)) != '\0' && c != '"') {
        if (i < 31) {
            token.value[i++] = next_char(parser);
        } else {
            next_char(parser);
        }
    }
    if (peek_char(parser) == '"') {
        next_char(parser);
    }
    return token;
}

struct Token read_operator(struct Parser *parser) {
    struct Token token;
    token.type = TOKEN_OPERATOR;
    token.line = parser->line;
    token.column = parser->column;
    memset(token.value, 0, sizeof(token.value));
    
    int i = 0;
    char c;
    while ((c = peek_char(parser)) != '\0' && is_operator_char(c)) {
        if (i < 31) {
            token.value[i++] = next_char(parser);
        } else {
            next_char(parser);
        }
    }
    return token;
}

struct Token get_next_token(struct Parser *parser) {
    skip_whitespace(parser);
    
    char current = peek_char(parser);
    if (current == '\0') {
        struct Token token;
        token.type = TOKEN_EOF;
        token.line = parser->line;
        token.column = parser->column;
        memset(token.value, 0, sizeof(token.value));
        return token;
    }
    
    if (isdigit(current)) {
        return read_number(parser);
    } else if (isalpha(current)) {
        return read_identifier(parser);
    } else if (current == '"') {
        return read_string(parser);
    } else if (is_operator_char(current)) {
        return read_operator(parser);
    } else {
        struct Token token;
        token.type = TOKEN_ERROR;
        token.line = parser->line;
        token.column = parser->column;
        memset(token.value, 0, sizeof(token.value));
        if (current != '\0') {
            token.value[0] = next_char(parser);
        }
        return token;
    }
}

void print_token(struct Token