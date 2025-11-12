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

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void parser_init(struct Parser *parser, const char *input) {
    parser->input = input;
    parser->position = 0;
    parser->line = 1;
    parser->column = 1;
    parser->current_token.type = TOKEN_INVALID;
    memset(parser->current_token.value, 0, sizeof(parser->current_token.value));
}

char parser_peek(struct Parser *parser) {
    if (parser->input == NULL) return '\0';
    if (parser->position < 0) return '\0';
    size_t len = strlen(parser->input);
    if (parser->position >= (int)len) return '\0';
    return parser->input[parser->position];
}

char parser_advance(struct Parser *parser) {
    char c = parser_peek(parser);
    if (c == '\0') return c;
    parser->position++;
    if (c == '\n') {
        parser->line++;
        parser->column = 1;
    } else {
        parser->column++;
    }
    return c;
}

void skip_whitespace(struct Parser *parser) {
    while (isspace(parser_peek(parser))) {
        parser_advance(parser);
    }
}

struct Token make_token(enum TokenType type, const char *value, int line, int column) {
    struct Token token;
    token.type = type;
    token.line = line;
    token.column = column;
    strncpy(token.value, value, sizeof(token.value) - 1);
    token.value[sizeof(token.value) - 1] = '\0';
    return token;
}

struct Token parse_number(struct Parser *parser) {
    char buffer[32] = {0};
    int i = 0;
    int line = parser->line;
    int column = parser->column;
    
    while (isdigit(parser_peek(parser)) && i < 31) {
        buffer[i++] = parser_advance(parser);
    }
    
    if (parser_peek(parser) == '.' && i < 31) {
        buffer[i++] = parser_advance(parser);
        while (isdigit(parser_peek(parser)) && i < 31) {
            buffer[i++] = parser_advance(parser);
        }
    }
    
    buffer[i] = '\0';
    return make_token(TOKEN_NUMBER, buffer, line, column);
}

struct Token parse_identifier(struct Parser *parser) {
    char buffer[32] = {0};
    int i = 0;
    int line = parser->line;
    int column = parser->column;
    
    while ((isalnum(parser_peek(parser)) || parser_peek(parser) == '_') && i < 31) {
        buffer[i++] = parser_advance(parser);
    }
    
    buffer[i] = '\0';
    return make_token(TOKEN_IDENTIFIER, buffer, line, column);
}

struct Token parse_string(struct Parser *parser) {
    char buffer[32] = {0};
    int i = 0;
    int line = parser->line;
    int column = parser->column;
    
    if (parser_peek(parser) != '"') {
        return make_token(TOKEN_INVALID, "", line, column);
    }
    
    parser_advance(parser);
    
    while (parser_peek(parser) != '"' && parser_peek(parser) != '\0' && i < 31) {
        buffer[i++] = parser_advance(parser);
    }
    
    if (parser_peek(parser) == '"') {
        parser_advance(parser);
    } else {
        return make_token(TOKEN_INVALID, "", line, column);
    }
    
    buffer[i] = '\0';
    return make_token(TOKEN_STRING, buffer, line, column);
}

struct Token parse_operator(struct Parser *parser) {
    char buffer[3] = {0};
    int i = 0;
    int line = parser->line;
    int column = parser->column;
    
    while (is_operator_char(parser_peek(parser)) && i < 2) {
        buffer[i++] = parser_advance(parser);
    }
    
    buffer[i] = '\0';
    return make_token(TOKEN_OPERATOR, buffer, line, column);
}

struct Token get_next_token(struct Parser *parser) {
    skip_whitespace(parser);
    
    char c = parser_peek(parser);
    if (c == '\0') {
        return make_token(TOKEN_EOF, "", parser->line, parser->column);
    }
    
    if (isdigit(c)) {
        return parse_number(parser);
    }