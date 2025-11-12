//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
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

void init_lexer(struct LexerState *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->token_count = 0;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_delimiter_char(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ',' || c == ';';
}

int get_next_token(struct LexerState *lexer, struct Token *token) {
    if (lexer == NULL || lexer->input == NULL || token == NULL) return 0;
    
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
        
        token->line = lexer->line;
        token->column = lexer->column;
        
        if (isalpha(current) || current == '_') {
            int i = 0;
            while ((isalnum(current) || current == '_') && i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = current;
                lexer->position++;
                lexer->column++;
                current = lexer->input[lexer->position];
            }
            token->value[i] = '\0';
            token->type = TOKEN_IDENTIFIER;
            lexer->token_count++;
            return 1;
        }
        
        if (isdigit(current)) {
            int i = 0;
            while (isdigit(current) && i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = current;
                lexer->position++;
                lexer->column++;
                current = lexer->input[lexer->position];
            }
            token->value[i] = '\0';
            token->type = TOKEN_NUMBER;
            lexer->token_count++;
            return 1;
        }
        
        if (is_operator_char(current)) {
            token->value[0] = current;
            token->value[1] = '\0';
            token->type = TOKEN_OPERATOR;
            lexer->position++;
            lexer->column++;
            lexer->token_count++;
            return 1;
        }
        
        if (is_delimiter_char(current)) {
            token->value[0] = current;
            token->value[1] = '\0';
            token->type = TOKEN_DELIMITER;
            lexer->position++;
            lexer->column++;
            lexer->token_count++;
            return 1;
        }
        
        if (current == '"') {
            int i = 0;
            token->value[i++] = current;
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
                token->value[i++] = current;
                lexer->position++;
                lexer->column++;
            }
            token->value[i] = '\0';
            token->type = TOKEN_STRING;
            lexer->token_count++;
            return 1;
        }
        
        token->value[0] = current;
        token->value[1] = '\0';
        token->type = TOKEN_UNKNOWN;
        lexer->position++;
        lexer->column++;
        lexer->token_count++;
        return 1;
    }
    
    token->type = TOKEN_EOF;
    token->value[0] = '\0';
    token->line = lexer->line;
    token->column = lexer->column;
    return 1;
}

void init_parser(struct ParserState *parser, struct Token *tokens, int count) {
    if (parser == NULL || tokens == NULL || count <= 0) return;
    parser->tokens = tokens;
    parser->current_token = 0;
    parser->token_count = count