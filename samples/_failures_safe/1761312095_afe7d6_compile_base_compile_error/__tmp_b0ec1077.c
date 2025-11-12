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
    struct Token tokens[MAX_TOKENS];
    int current_token;
    int token_count;
};

void init_lexer(struct Lexer *lexer, const char *input) {
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->token_count = 0;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_delimiter(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == ';' || c == ',';
}

struct Token next_token(struct Lexer *lexer) {
    struct Token token = {TOKEN_UNKNOWN, "", lexer->line, lexer->column};
    
    if (lexer->input[lexer->position] == '\0') {
        token.type = TOKEN_EOF;
        strncpy(token.value, "EOF", MAX_TOKEN_LEN - 1);
        token.value[MAX_TOKEN_LEN - 1] = '\0';
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
    
    token.line = lexer->line;
    token.column = lexer->column;
    
    char current = lexer->input[lexer->position];
    
    if (isalpha(current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while ((isalnum(current) || current == '_') && i < MAX_TOKEN_LEN - 1) {
            token.value[i++] = current;
            lexer->position++;
            lexer->column++;
            current = lexer->input[lexer->position];
        }
        token.value[i] = '\0';
    } else if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (isdigit(current) && i < MAX_TOKEN_LEN - 1) {
            token.value[i++] = current;
            lexer->position++;
            lexer->column++;
            current = lexer->input[lexer->position];
        }
        token.value[i] = '\0';
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        int i = 0;
        lexer->position++;
        lexer->column++;
        current = lexer->input[lexer->position];
        
        while (current != '"' && current != '\0' && i < MAX_TOKEN_LEN - 1) {
            token.value[i++] = current;
            lexer->position++;
            lexer->column++;
            current = lexer->input[lexer->position];
        }
        token.value[i] = '\0';
        
        if (current == '"') {
            lexer->position++;
            lexer->column++;
        }
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (is_operator_char(current) && i < MAX_TOKEN_LEN - 1) {
            token.value[i++] = current;
            lexer->position++;
            lexer->column++;
            current = lexer->input[lexer->position];
        }
        token.value[i] = '\0';
    } else if (is_delimiter(current)) {
        token.type = TOKEN_DELIMITER;
        token.value[0] = current;
        token.value[1] = '\0';
        lexer->position++;
        lexer->column++;
    } else {
        token.type = TOKEN_UNKNOWN;
        token.value[0] = current;
        token.value[1] = '\0';
        lexer->position++;
        lexer->column++;
    }
    
    lexer->token_count++;
    return token;
}

void init_parser(struct Parser *parser, struct Token *tokens, int count) {
    if (count > MAX_TOKENS) {
        count = MAX_TOKENS;
    }
    memcpy(parser->tokens, tokens, count * sizeof(struct Token));
    parser->token_count = count;
    parser->current_token = 0;
}

struct Token peek_token(struct Parser *parser) {
    if (parser->current_token < parser->token_count) {
        return parser->tokens[parser->current_token];
    }
    struct Token eof = {TOKEN_EOF, "EOF", 0, 0};
    return e