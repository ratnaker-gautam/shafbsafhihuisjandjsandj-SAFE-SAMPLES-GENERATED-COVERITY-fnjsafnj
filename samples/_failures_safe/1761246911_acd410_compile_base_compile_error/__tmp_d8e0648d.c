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
    int error_count;
};

struct ASTNode {
    enum { NODE_IDENTIFIER, NODE_NUMBER, NODE_STRING, NODE_BINARY_OP } type;
    union {
        char identifier[32];
        double number;
        char string[32];
        struct {
            struct ASTNode *left;
            struct ASTNode *right;
            char operator[4];
        } binary_op;
    } value;
};

void init_lexer(struct Lexer *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->input_length = (int)strlen(input);
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

struct Token get_next_token(struct Lexer *lexer) {
    struct Token token = {TOKEN_ERROR, "", lexer->line, lexer->column};
    
    if (lexer == NULL || lexer->input == NULL) {
        return token;
    }
    
    while (lexer->position < lexer->input_length) {
        char current = lexer->input[lexer->position];
        
        if (isspace((unsigned char)current)) {
            if (current == '\n') {
                lexer->line++;
                lexer->column = 1;
            } else {
                lexer->column++;
            }
            lexer->position++;
            continue;
        }
        
        if (isalpha((unsigned char)current) || current == '_') {
            int value_pos = 0;
            token.type = TOKEN_IDENTIFIER;
            token.line = lexer->line;
            token.column = lexer->column;
            
            while (lexer->position < lexer->input_length && 
                   (isalnum((unsigned char)lexer->input[lexer->position]) || 
                    lexer->input[lexer->position] == '_') &&
                   value_pos < 31) {
                token.value[value_pos++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            token.value[value_pos] = '\0';
            return token;
        }
        
        if (isdigit((unsigned char)current)) {
            int value_pos = 0;
            token.type = TOKEN_NUMBER;
            token.line = lexer->line;
            token.column = lexer->column;
            
            while (lexer->position < lexer->input_length && 
                   (isdigit((unsigned char)lexer->input[lexer->position]) || 
                    lexer->input[lexer->position] == '.') &&
                   value_pos < 31) {
                token.value[value_pos++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            token.value[value_pos] = '\0';
            return token;
        }
        
        if (current == '"') {
            int value_pos = 0;
            token.type = TOKEN_STRING;
            token.line = lexer->line;
            token.column = lexer->column;
            
            lexer->position++;
            lexer->column++;
            
            while (lexer->position < lexer->input_length && 
                   lexer->input[lexer->position] != '"' &&
                   value_pos < 31) {
                token.value[value_pos++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            
            if (lexer->position < lexer->input_length && lexer->input[lexer->position] == '"') {
                lexer->position++;
                lexer->column++;
            }
            token.value[value_pos] = '\0';
            return token;
        }
        
        if (is_operator_char(current)) {
            token.type = TOKEN_OPERATOR;
            token.line = lexer->line;
            token.column = lexer->column;
            
            int value_pos = 0;
            while (lexer->position < lexer->input_length && 
                   is_operator_char(lexer->input[lexer->position]) &&
                   value_pos < 3) {
                token.value[value_pos++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            token.value[value_pos] = '\0';
            return token;
        }
        
        token.type = TOKEN_ERROR;
        strncpy(token.value, "Unexpected character", 31);
        token.value[31] = '\0';
        lexer->position++;
        lexer->column++;
        return token;
    }
    
    token.type = TOKEN_EOF;
    token.line = lexer->line;
    token.column =