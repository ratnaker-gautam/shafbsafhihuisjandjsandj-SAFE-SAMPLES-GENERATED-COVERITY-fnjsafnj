//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

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

struct LexerState {
    const char *input;
    int position;
    int line;
    int column;
    int token_count;
};

struct ParserState {
    struct Token *tokens;
    int current_token;
    int token_count;
};

struct ParseTree {
    char node_type[20];
    char value[MAX_TOKEN_LEN];
    struct ParseTree *children[10];
    int child_count;
};

void init_lexer(struct LexerState *lexer, const char *input) {
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->token_count = 0;
}

void init_parser(struct ParserState *parser, struct Token *tokens, int count) {
    parser->tokens = tokens;
    parser->current_token = 0;
    parser->token_count = count;
}

struct ParseTree *create_parse_node(const char *type, const char *value) {
    struct ParseTree *node = malloc(sizeof(struct ParseTree));
    if (node == NULL) return NULL;
    strncpy(node->node_type, type, 19);
    node->node_type[19] = '\0';
    strncpy(node->value, value, MAX_TOKEN_LEN - 1);
    node->value[MAX_TOKEN_LEN - 1] = '\0';
    node->child_count = 0;
    for (int i = 0; i < 10; i++) node->children[i] = NULL;
    return node;
}

void add_child(struct ParseTree *parent, struct ParseTree *child) {
    if (parent == NULL || child == NULL) return;
    if (parent->child_count < 10) {
        parent->children[parent->child_count++] = child;
    }
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int is_delimiter(char c) {
    return strchr("(),;{}[]", c) != NULL;
}

struct Token get_next_token(struct LexerState *lexer) {
    struct Token token = {TOKEN_UNKNOWN, "", lexer->line, lexer->column};
    
    while (lexer->input[lexer->position] != '\0') {
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
            int len = 0;
            while (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') {
                if (len < MAX_TOKEN_LEN - 1) {
                    token.value[len++] = lexer->input[lexer->position];
                }
                lexer->position++;
                lexer->column++;
            }
            token.value[len] = '\0';
            token.type = TOKEN_IDENTIFIER;
            return token;
        }
        
        if (isdigit(current)) {
            int len = 0;
            while (isdigit(lexer->input[lexer->position])) {
                if (len < MAX_TOKEN_LEN - 1) {
                    token.value[len++] = lexer->input[lexer->position];
                }
                lexer->position++;
                lexer->column++;
            }
            token.value[len] = '\0';
            token.type = TOKEN_NUMBER;
            return token;
        }
        
        if (current == '"') {
            int len = 0;
            lexer->position++;
            lexer->column++;
            while (lexer->input[lexer->position] != '"' && lexer->input[lexer->position] != '\0') {
                if (len < MAX_TOKEN_LEN - 1) {
                    token.value[len++] = lexer->input[lexer->position];
                }
                lexer->position++;
                lexer->column++;
            }
            if (lexer->input[lexer->position] == '"') {
                lexer->position++;
                lexer->column++;
            }
            token.value[len] = '\0';
            token.type = TOKEN_STRING;
            return token;
        }
        
        if (is_operator_char(current)) {
            int len = 0;
            while (is_operator_char(lexer->input[lexer->position])) {
                if (len < MAX_TOKEN_LEN - 1) {
                    token.value[len++] = lexer->input[lexer->position];
                }
                lexer->position++;
                lexer->column++;
            }
            token.value[len] = '\0';
            token.type = TOKEN_OPERATOR;
            return token;
        }
        
        if (is_delimiter(current)) {
            token.value[0