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

struct ExpressionNode {
    enum { NODE_NUMBER, NODE_IDENTIFIER, NODE_BINARY_OP } type;
    union {
        double number_value;
        char identifier[MAX_TOKEN_LEN];
        struct {
            struct ExpressionNode *left;
            struct ExpressionNode *right;
            char operator[8];
        } binary_op;
    } data;
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

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
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

int read_token(struct LexerState *lexer, struct Token *token) {
    skip_whitespace(lexer);
    
    if (lexer->input[lexer->position] == '\0') {
        token->type = TOKEN_EOF;
        strcpy(token->value, "");
        token->line = lexer->line;
        token->column = lexer->column;
        return 1;
    }
    
    char current = lexer->input[lexer->position];
    int start_pos = lexer->position;
    int value_index = 0;
    
    token->line = lexer->line;
    token->column = lexer->column;
    
    if (isalpha(current) || current == '_') {
        token->type = TOKEN_IDENTIFIER;
        while (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') {
            if (value_index < MAX_TOKEN_LEN - 1) {
                token->value[value_index++] = lexer->input[lexer->position];
            }
            lexer->position++;
            lexer->column++;
        }
    } else if (isdigit(current)) {
        token->type = TOKEN_NUMBER;
        while (isdigit(lexer->input[lexer->position]) || lexer->input[lexer->position] == '.') {
            if (value_index < MAX_TOKEN_LEN - 1) {
                token->value[value_index++] = lexer->input[lexer->position];
            }
            lexer->position++;
            lexer->column++;
        }
    } else if (is_operator_char(current)) {
        token->type = TOKEN_OPERATOR;
        while (is_operator_char(lexer->input[lexer->position])) {
            if (value_index < MAX_TOKEN_LEN - 1) {
                token->value[value_index++] = lexer->input[lexer->position];
            }
            lexer->position++;
            lexer->column++;
        }
    } else if (current == '"') {
        token->type = TOKEN_STRING;
        token->value[value_index++] = '"';
        lexer->position++;
        lexer->column++;
        
        while (lexer->input[lexer->position] != '"' && lexer->input[lexer->position] != '\0') {
            if (value_index < MAX_TOKEN_LEN - 1) {
                token->value[value_index++] = lexer->input[lexer->position];
            }
            lexer->position++;
            lexer->column++;
        }
        
        if (lexer->input[lexer->position] == '"') {
            if (value_index < MAX_TOKEN_LEN - 1) {
                token->value[value_index++] = '"';
            }
            lexer->position++;
            lexer->column++;
        }
    } else {
        token->type = TOKEN_DELIMITER;
        if (value_index < MAX_TOKEN_LEN - 1) {
            token->value[value_index++] = lexer->input[lexer->position];
        }
        lexer->position++;
        lexer->column++;
    }
    
    token->