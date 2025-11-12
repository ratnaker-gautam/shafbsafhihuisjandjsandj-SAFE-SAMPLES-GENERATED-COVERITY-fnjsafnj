//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
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
    TOKEN_ERROR
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
    int input_length;
};

struct Parser {
    struct Lexer *lexer;
    struct Token current_token;
    int has_error;
};

void init_lexer(struct Lexer *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->input_length = strlen(input);
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void next_token(struct Lexer *lexer, struct Token *token) {
    if (lexer == NULL || token == NULL) return;
    
    while (lexer->position < lexer->input_length && isspace(lexer->input[lexer->position])) {
        if (lexer->input[lexer->position] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
    }
    
    if (lexer->position >= lexer->input_length) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        token->line = lexer->line;
        token->column = lexer->column;
        return;
    }
    
    char current = lexer->input[lexer->position];
    token->line = lexer->line;
    token->column = lexer->column;
    
    if (isalpha(current) || current == '_') {
        int i = 0;
        while ((lexer->position < lexer->input_length) && 
               (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') &&
               i < 31) {
            token->value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_IDENTIFIER;
    } else if (isdigit(current)) {
        int i = 0;
        while ((lexer->position < lexer->input_length) && 
               (isdigit(lexer->input[lexer->position])) &&
               i < 31) {
            token->value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_NUMBER;
    } else if (is_operator_char(current)) {
        token->value[0] = current;
        token->value[1] = '\0';
        token->type = TOKEN_OPERATOR;
        lexer->position++;
        lexer->column++;
    } else if (current == '"') {
        int i = 0;
        lexer->position++;
        lexer->column++;
        while ((lexer->position < lexer->input_length) && 
               (lexer->input[lexer->position] != '"') &&
               i < 31) {
            token->value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        if (lexer->position < lexer->input_length && lexer->input[lexer->position] == '"') {
            lexer->position++;
            lexer->column++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_STRING;
    } else {
        token->value[0] = current;
        token->value[1] = '\0';
        token->type = TOKEN_ERROR;
        lexer->position++;
        lexer->column++;
    }
}

void init_parser(struct Parser *parser, struct Lexer *lexer) {
    if (parser == NULL || lexer == NULL) return;
    parser->lexer = lexer;
    parser->has_error = 0;
    next_token(parser->lexer, &parser->current_token);
}

void parse_expression(struct Parser *parser) {
    if (parser == NULL) return;
    
    if (parser->current_token.type == TOKEN_IDENTIFIER) {
        printf("Found identifier: %s\n", parser->current_token.value);
        next_token(parser->lexer, &parser->current_token);
    } else if (parser->current_token.type == TOKEN_NUMBER) {
        printf("Found number: %s\n", parser->current_token.value);
        next_token(parser->lexer, &parser->current_token);
    } else if (parser->current_token.type == TOKEN_STRING) {
        printf("Found string: \"%s\"\n", parser->current_token.value);
        next_token(parser->lexer, &parser->current_token);
    } else {
        printf("Unexpected token at line