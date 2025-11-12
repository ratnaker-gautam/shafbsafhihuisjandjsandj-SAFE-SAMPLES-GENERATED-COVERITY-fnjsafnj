//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

enum TokenType {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
    TOKEN_KEYWORD,
    TOKEN_EOF,
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char value[64];
    uint32_t line;
    uint32_t column;
};

struct LexerState {
    const char *input;
    uint32_t position;
    uint32_t line;
    uint32_t column;
    uint32_t input_length;
};

struct ParserNode {
    enum { NODE_EXPRESSION, NODE_STATEMENT, NODE_LITERAL } node_type;
    struct Token token;
    struct ParserNode *left;
    struct ParserNode *right;
};

struct ParseResult {
    int success;
    struct ParserNode *ast;
    char error_msg[128];
};

const char *keywords[] = {"if", "else", "while", "for", "return", "int", "float", "char", NULL};

int is_keyword(const char *str) {
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void init_lexer(struct LexerState *lexer, const char *input) {
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->input_length = strlen(input);
}

void advance_lexer(struct LexerState *lexer) {
    if (lexer->position < lexer->input_length) {
        if (lexer->input[lexer->position] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
    }
}

char current_char(struct LexerState *lexer) {
    if (lexer->position < lexer->input_length) {
        return lexer->input[lexer->position];
    }
    return '\0';
}

void skip_whitespace(struct LexerState *lexer) {
    while (lexer->position < lexer->input_length && isspace(current_char(lexer))) {
        advance_lexer(lexer);
    }
}

struct Token next_token(struct LexerState *lexer) {
    struct Token token = {TOKEN_INVALID, "", lexer->line, lexer->column};
    
    skip_whitespace(lexer);
    
    if (lexer->position >= lexer->input_length) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char c = current_char(lexer);
    
    if (isalpha(c) || c == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while ((isalnum(c) || c == '_') && i < 63) {
            token.value[i++] = c;
            advance_lexer(lexer);
            c = current_char(lexer);
        }
        token.value[i] = '\0';
        if (is_keyword(token.value)) {
            token.type = TOKEN_KEYWORD;
        }
    } else if (isdigit(c)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while ((isdigit(c) || c == '.') && i < 63) {
            token.value[i++] = c;
            advance_lexer(lexer);
            c = current_char(lexer);
        }
        token.value[i] = '\0';
    } else if (c == '"') {
        token.type = TOKEN_STRING;
        int i = 0;
        advance_lexer(lexer);
        c = current_char(lexer);
        while (c != '"' && c != '\0' && i < 63) {
            token.value[i++] = c;
            advance_lexer(lexer);
            c = current_char(lexer);
        }
        token.value[i] = '\0';
        if (c == '"') {
            advance_lexer(lexer);
        }
    } else if (strchr("+-*/=<>!&|", c)) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (strchr("+-*/=<>!&|", c) && i < 63) {
            token.value[i++] = c;
            advance_lexer(lexer);
            c = current_char(lexer);
        }
        token.value[i] = '\0';
    } else if (strchr("(),;{}[]", c)) {
        token.type = TOKEN_DELIMITER;
        token.value[0] = c;
        token.value[1] = '\0';
        advance_lexer(lexer);
    } else {
        token.type = TOKEN_INVALID;
        token.value[0] = c;
        token.value[1] = '\0';
        advance_lexer(lexer);
    }
    
    return token;
}

struct ParserNode *create_node(struct Token token) {
    struct ParserNode *node = malloc(sizeof(struct ParserNode));
    if (node == NULL) return NULL;
    node->token