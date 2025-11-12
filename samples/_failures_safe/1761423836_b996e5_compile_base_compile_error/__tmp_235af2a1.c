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
    char value[64];
    int line;
    int column;
};

struct Lexer {
    const char *input;
    int position;
    int line;
    int column;
    int length;
};

struct Parser {
    struct Lexer *lexer;
    struct Token current;
    int error_count;
};

struct ASTNode {
    enum { NODE_IDENTIFIER, NODE_NUMBER, NODE_STRING, NODE_BINARY_OP } type;
    char value[64];
    struct ASTNode *left;
    struct ASTNode *right;
    int line;
    int column;
};

void init_lexer(struct Lexer *lexer, const char *input) {
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->length = strlen(input);
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

struct Token get_next_token(struct Lexer *lexer) {
    struct Token token;
    token.type = TOKEN_ERROR;
    token.value[0] = '\0';
    token.line = lexer->line;
    token.column = lexer->column;

    while (lexer->position < lexer->length) {
        char current = lexer->input[lexer->position];

        if (isspace(current)) {
            if (current == '\n') {
                lexer->line++;
                lexer->column = 1;
            } else {
                lexer->column++;
            }
            lexer->position++;
            continue;
        }

        if (isalpha(current) || current == '_') {
            int i = 0;
            while ((lexer->position < lexer->length) && 
                   (isalnum(lexer->input[lexer->position]) || 
                    lexer->input[lexer->position] == '_') && i < 63) {
                token.value[i++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            token.value[i] = '\0';
            token.type = TOKEN_IDENTIFIER;
            return token;
        }

        if (isdigit(current)) {
            int i = 0;
            while ((lexer->position < lexer->length) && 
                   isdigit(lexer->input[lexer->position]) && i < 63) {
                token.value[i++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            token.value[i] = '\0';
            token.type = TOKEN_NUMBER;
            return token;
        }

        if (current == '"') {
            int i = 0;
            lexer->position++;
            lexer->column++;
            while ((lexer->position < lexer->length) && 
                   (lexer->input[lexer->position] != '"') && i < 63) {
                token.value[i++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            if (lexer->position < lexer->length && lexer->input[lexer->position] == '"') {
                lexer->position++;
                lexer->column++;
            }
            token.value[i] = '\0';
            token.type = TOKEN_STRING;
            return token;
        }

        if (is_operator_char(current)) {
            token.value[0] = current;
            token.value[1] = '\0';
            token.type = TOKEN_OPERATOR;
            lexer->position++;
            lexer->column++;
            return token;
        }

        break;
    }

    if (lexer->position >= lexer->length) {
        token.type = TOKEN_EOF;
        strcpy(token.value, "EOF");
    }
    return token;
}

void init_parser(struct Parser *parser, struct Lexer *lexer) {
    parser->lexer = lexer;
    parser->current = get_next_token(lexer);
    parser->error_count = 0;
}

void advance_parser(struct Parser *parser) {
    parser->current = get_next_token(parser->lexer);
}

struct ASTNode *parse_expression(struct Parser *parser);

struct ASTNode *parse_primary(struct Parser *parser) {
    struct ASTNode *node = malloc(sizeof(struct ASTNode));
    if (!node) return NULL;

    if (parser->current.type == TOKEN_IDENTIFIER) {
        node->type = NODE_IDENTIFIER;
        strncpy(node->value, parser->current.value, 63);
        node->value[63] = '\0';
        node->line = parser->current.line;
        node->column = parser->current.column;
        node->left = NULL;
        node->right = NULL;
        advance_parser(parser);
    } else if (parser->current.type == TOKEN_NUMBER) {
        node->type = NODE_NUMBER;
        strncpy(node->value, parser->current.value, 63);
        node->value[63] = '\0';
        node->line =