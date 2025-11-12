//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

enum TokenType {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
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
    enum TokenType node_type;
    char value[64];
    struct ParserNode *left;
    struct ParserNode *right;
};

struct ParseResult {
    int success;
    struct ParserNode *ast;
    char error_msg[128];
};

void init_lexer(struct LexerState *lexer, const char *input) {
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

void advance_lexer(struct LexerState *lexer) {
    if (lexer->position >= lexer->input_length) return;
    if (lexer->input[lexer->position] == '\n') {
        lexer->line++;
        lexer->column = 1;
    } else {
        lexer->column++;
    }
    lexer->position++;
}

char peek_lexer(const struct LexerState *lexer) {
    if (lexer->position >= lexer->input_length) return '\0';
    return lexer->input[lexer->position];
}

void skip_whitespace(struct LexerState *lexer) {
    while (lexer->position < lexer->input_length && isspace(lexer->input[lexer->position])) {
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
    
    char current = peek_lexer(lexer);
    
    if (isalpha(current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        size_t i = 0;
        while ((isalnum(current) || current == '_') && i < sizeof(token.value) - 1) {
            token.value[i++] = current;
            advance_lexer(lexer);
            current = peek_lexer(lexer);
        }
        token.value[i] = '\0';
    } else if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        size_t i = 0;
        while (isdigit(current) && i < sizeof(token.value) - 1) {
            token.value[i++] = current;
            advance_lexer(lexer);
            current = peek_lexer(lexer);
        }
        token.value[i] = '\0';
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        advance_lexer(lexer);
        size_t i = 0;
        current = peek_lexer(lexer);
        while (current != '"' && current != '\0' && i < sizeof(token.value) - 1) {
            token.value[i++] = current;
            advance_lexer(lexer);
            current = peek_lexer(lexer);
        }
        token.value[i] = '\0';
        if (current == '"') advance_lexer(lexer);
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        size_t i = 0;
        while (is_operator_char(current) && i < sizeof(token.value) - 1) {
            token.value[i++] = current;
            advance_lexer(lexer);
            current = peek_lexer(lexer);
        }
        token.value[i] = '\0';
    } else {
        token.type = TOKEN_INVALID;
        token.value[0] = current;
        token.value[1] = '\0';
        advance_lexer(lexer);
    }
    
    return token;
}

struct ParserNode *create_parser_node(enum TokenType type, const char *value) {
    struct ParserNode *node = malloc(sizeof(struct ParserNode));
    if (node == NULL) return NULL;
    node->node_type = type;
    strncpy(node->value, value, sizeof(node->value) - 1);
    node->value[sizeof(node->value) - 1] = '\0';
    node->left = NULL;
    node->right = NULL;
    return node;
}

void free_ast(struct ParserNode *node) {
    if (node == NULL) return;
    free_ast(node->left);
    free_ast(node->right);
    free(node);
}

struct ParseResult parse_expression(struct LexerState *lexer)