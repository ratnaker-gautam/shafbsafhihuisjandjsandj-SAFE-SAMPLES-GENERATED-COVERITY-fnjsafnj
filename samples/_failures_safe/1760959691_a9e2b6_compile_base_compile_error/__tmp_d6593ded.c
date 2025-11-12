//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_UNKNOWN
};

struct Token {
    enum TokenType type;
    char value[32];
    int line;
    int column;
};

struct Lexer {
    const char *input;
    int position;
    int line;
    int column;
    struct Token current_token;
};

struct Parser {
    struct Lexer *lexer;
    int error_count;
};

void lexer_init(struct Lexer *lexer, const char *input) {
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->current_token.type = TOKEN_UNKNOWN;
    memset(lexer->current_token.value, 0, sizeof(lexer->current_token.value));
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

struct Token lexer_next_token(struct Lexer *lexer) {
    struct Token token;
    token.type = TOKEN_UNKNOWN;
    memset(token.value, 0, sizeof(token.value));
    token.line = lexer->line;
    token.column = lexer->column;

    if (lexer->input[lexer->position] == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }

    while (isspace(lexer->input[lexer->position])) {
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
    int value_index = 0;

    if (isalpha(current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        while (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') {
            if (value_index < 31) {
                token.value[value_index++] = lexer->input[lexer->position];
            }
            lexer->position++;
            lexer->column++;
        }
    } else if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        while (isdigit(lexer->input[lexer->position])) {
            if (value_index < 31) {
                token.value[value_index++] = lexer->input[lexer->position];
            }
            lexer->position++;
            lexer->column++;
        }
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        while (is_operator_char(lexer->input[lexer->position]) && value_index < 31) {
            token.value[value_index++] = lexer->input[lexer->position];
            lexer->position++;
            lexer->column++;
        }
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        lexer->position++;
        lexer->column++;
        while (lexer->input[lexer->position] != '"' && lexer->input[lexer->position] != '\0') {
            if (value_index < 31) {
                token.value[value_index++] = lexer->input[lexer->position];
            }
            lexer->position++;
            lexer->column++;
        }
        if (lexer->input[lexer->position] == '"') {
            lexer->position++;
            lexer->column++;
        }
    } else {
        token.type = TOKEN_UNKNOWN;
        if (value_index < 31) {
            token.value[value_index++] = current;
        }
        lexer->position++;
        lexer->column++;
    }

    token.value[value_index] = '\0';
    lexer->current_token = token;
    return token;
}

void parser_init(struct Parser *parser, struct Lexer *lexer) {
    parser->lexer = lexer;
    parser->error_count = 0;
}

void parser_parse_expression(struct Parser *parser) {
    struct Token token = parser->lexer->current_token;
    
    if (token.type == TOKEN_IDENTIFIER || token.type == TOKEN_NUMBER) {
        lexer_next_token(parser->lexer);
        token = parser->lexer->current_token;
        
        if (token.type == TOKEN_OPERATOR) {
            lexer_next_token(parser->lexer);
            token = parser->lexer->current_token;
            
            if (token.type == TOKEN_IDENTIFIER || token.type == TOKEN_NUMBER) {
                lexer_next_token(parser->lexer);
            } else {
                parser->error_count++;
            }
        }
    } else {
        parser->error_count++;
    }
}

void parser_parse_statement(struct Parser *parser) {
    struct Token token = parser->lexer->current_token;
    
    if (token.type == TOKEN_IDENTIFIER) {