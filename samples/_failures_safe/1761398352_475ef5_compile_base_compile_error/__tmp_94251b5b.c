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

struct ParserNode {
    enum TokenType node_type;
    char value[MAX_TOKEN_LEN];
    struct ParserNode *left;
    struct ParserNode *right;
};

struct ParseResult {
    int success;
    struct ParserNode *ast;
    char error_msg[100];
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_delimiter_char(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ',' || c == ';';
}

void init_lexer(struct LexerState *lexer, const char *input) {
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->token_count = 0;
}

void skip_whitespace(struct LexerState *lexer) {
    while (lexer->input[lexer->position] != '\0' && isspace(lexer->input[lexer->position])) {
        if (lexer->input[lexer->position] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
    }
}

int next_token(struct LexerState *lexer, struct Token *token) {
    if (lexer->token_count >= MAX_TOKENS) {
        return 0;
    }
    
    skip_whitespace(lexer);
    
    if (lexer->input[lexer->position] == '\0') {
        token->type = TOKEN_EOF;
        strcpy(token->value, "");
        token->line = lexer->line;
        token->column = lexer->column;
        lexer->token_count++;
        return 1;
    }
    
    char current = lexer->input[lexer->position];
    token->line = lexer->line;
    token->column = lexer->column;
    
    if (isalpha(current) || current == '_') {
        token->type = TOKEN_IDENTIFIER;
        int i = 0;
        while ((isalnum(current) || current == '_') && i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = current;
            lexer->position++;
            lexer->column++;
            current = lexer->input[lexer->position];
        }
        token->value[i] = '\0';
    } else if (isdigit(current)) {
        token->type = TOKEN_NUMBER;
        int i = 0;
        while (isdigit(current) && i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = current;
            lexer->position++;
            lexer->column++;
            current = lexer->input[lexer->position];
        }
        token->value[i] = '\0';
    } else if (current == '"') {
        token->type = TOKEN_STRING;
        int i = 0;
        lexer->position++;
        lexer->column++;
        current = lexer->input[lexer->position];
        
        while (current != '"' && current != '\0' && i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = current;
            lexer->position++;
            lexer->column++;
            current = lexer->input[lexer->position];
        }
        
        if (current == '"') {
            lexer->position++;
            lexer->column++;
        }
        token->value[i] = '\0';
    } else if (is_operator_char(current)) {
        token->type = TOKEN_OPERATOR;
        token->value[0] = current;
        token->value[1] = '\0';
        lexer->position++;
        lexer->column++;
    } else if (is_delimiter_char(current)) {
        token->type = TOKEN_DELIMITER;
        token->value[0] = current;
        token->value[1] = '\0';
        lexer->position++;
        lexer->column++;
    } else {
        token->type = TOKEN_UNKNOWN;
        token->value[0] = current;
        token->value[1] = '\0';
        lexer->position++;
        lexer->column++;
    }
    
    lexer->token_count++;
    return 1;
}

struct ParserNode* create_parser_node(enum TokenType type, const char *value) {
    struct ParserNode *node = malloc(sizeof(struct ParserNode));
    if (node == NULL) {
        return NULL;
    }
    node->node_type = type;
    if