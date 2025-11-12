//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
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
    TOKEN_PUNCTUATION,
    TOKEN_KEYWORD,
    TOKEN_EOF,
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char *value;
    size_t line;
    size_t column;
};

struct LexerState {
    const char *input;
    size_t position;
    size_t line;
    size_t column;
    size_t input_length;
};

struct ParserState {
    struct Token *tokens;
    size_t token_count;
    size_t current_token;
};

struct ASTNode {
    enum {
        NODE_IDENTIFIER,
        NODE_NUMBER,
        NODE_STRING,
        NODE_BINARY_OP,
        NODE_ASSIGNMENT
    } type;
    union {
        char *identifier;
        double number;
        char *string;
        struct {
            struct ASTNode *left;
            struct ASTNode *right;
            char operator;
        } binary_op;
        struct {
            char *variable;
            struct ASTNode *value;
        } assignment;
    } data;
};

void init_lexer(struct LexerState *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->input_length = strlen(input);
}

void free_token(struct Token *token) {
    if (token == NULL) return;
    if (token->value != NULL) {
        free(token->value);
    }
}

void free_ast_node(struct ASTNode *node) {
    if (node == NULL) return;
    switch (node->type) {
        case NODE_IDENTIFIER:
            if (node->data.identifier != NULL) free(node->data.identifier);
            break;
        case NODE_STRING:
            if (node->data.string != NULL) free(node->data.string);
            break;
        case NODE_BINARY_OP:
            free_ast_node(node->data.binary_op.left);
            free_ast_node(node->data.binary_op.right);
            break;
        case NODE_ASSIGNMENT:
            if (node->data.assignment.variable != NULL) free(node->data.assignment.variable);
            free_ast_node(node->data.assignment.value);
            break;
        default:
            break;
    }
    free(node);
}

int is_keyword(const char *str) {
    static const char *keywords[] = {"if", "else", "while", "for", "return", NULL};
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0) return 1;
    }
    return 0;
}

struct Token next_token(struct LexerState *lexer) {
    struct Token token = {TOKEN_INVALID, NULL, lexer->line, lexer->column};
    if (lexer == NULL || lexer->input == NULL) return token;
    if (lexer->position >= lexer->input_length) {
        token.type = TOKEN_EOF;
        return token;
    }
    char current = lexer->input[lexer->position];
    while (isspace(current)) {
        if (current == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
        if (lexer->position >= lexer->input_length) {
            token.type = TOKEN_EOF;
            return token;
        }
        current = lexer->input[lexer->position];
    }
    token.line = lexer->line;
    token.column = lexer->column;
    if (isalpha(current) || current == '_') {
        size_t start = lexer->position;
        while (lexer->position < lexer->input_length && 
               (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_')) {
            lexer->position++;
            lexer->column++;
        }
        size_t length = lexer->position - start;
        if (length > 0) {
            token.value = malloc(length + 1);
            if (token.value != NULL) {
                strncpy(token.value, lexer->input + start, length);
                token.value[length] = '\0';
                token.type = is_keyword(token.value) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
            }
        }
    } else if (isdigit(current)) {
        size_t start = lexer->position;
        while (lexer->position < lexer->input_length && 
               (isdigit(lexer->input[lexer->position]) || lexer->input[lexer->position] == '.')) {
            lexer->position++;
            lexer->column++;
        }
        size_t length = lexer->position - start;
        if (length > 0) {
            token.value = malloc(length + 1);
            if (token.value != NULL) {
                strncpy(token.value, lexer->input + start, length);
                token.value[length] = '\0';
                token.type = TOKEN_NUMBER;
            }