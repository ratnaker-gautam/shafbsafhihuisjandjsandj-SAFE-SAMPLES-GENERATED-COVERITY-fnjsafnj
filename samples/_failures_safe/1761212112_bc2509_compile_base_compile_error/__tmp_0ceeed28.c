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

int is_delimiter(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ',' || c == ';';
}

struct Token create_token(enum TokenType type, const char *value, int line, int column) {
    struct Token token;
    token.type = type;
    token.line = line;
    token.column = column;
    strncpy(token.value, value, MAX_TOKEN_LEN - 1);
    token.value[MAX_TOKEN_LEN - 1] = '\0';
    return token;
}

int get_next_token(struct LexerState *lexer, struct Token *token) {
    if (lexer->input == NULL || token == NULL) return 0;
    
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
            char identifier[MAX_TOKEN_LEN] = {0};
            int i = 0;
            int start_col = lexer->column;
            
            while ((isalnum(current) || current == '_') && i < MAX_TOKEN_LEN - 1) {
                identifier[i++] = current;
                lexer->position++;
                lexer->column++;
                current = lexer->input[lexer->position];
            }
            identifier[i] = '\0';
            *token = create_token(TOKEN_IDENTIFIER, identifier, lexer->line, start_col);
            return 1;
        }
        
        if (isdigit(current)) {
            char number[MAX_TOKEN_LEN] = {0};
            int i = 0;
            int start_col = lexer->column;
            
            while (isdigit(current) && i < MAX_TOKEN_LEN - 1) {
                number[i++] = current;
                lexer->position++;
                lexer->column++;
                current = lexer->input[lexer->position];
            }
            number[i] = '\0';
            *token = create_token(TOKEN_NUMBER, number, lexer->line, start_col);
            return 1;
        }
        
        if (current == '"') {
            char string[MAX_TOKEN_LEN] = {0};
            int i = 0;
            int start_col = lexer->column;
            
            lexer->position++;
            lexer->column++;
            current = lexer->input[lexer->position];
            
            while (current != '"' && current != '\0' && i < MAX_TOKEN_LEN - 1) {
                string[i++] = current;
                lexer->position++;
                lexer->column++;
                current = lexer->input[lexer->position];
            }
            
            if (current == '"') {
                lexer->position++;
                lexer->column++;
            }
            string[i] = '\0';
            *token = create_token(TOKEN_STRING, string, lexer->line, start_col);
            return 1;
        }
        
        if (is_operator_char(current)) {
            char op[2] = {current, '\0'};
            int start_col = lexer->column;
            lexer->position++;
            lexer->column++;
            *token = create_token(TOKEN_OPERATOR, op, lexer->line, start_col);
            return 1;
        }
        
        if (is_delimiter(current)) {
            char delim[2] = {current, '\0'};
            int start_col = lexer->column;
            lexer->position++;
            lexer->column++;
            *token = create_token(TOKEN_DELIMITER, delim, lexer->line, start_col);
            return 1;
        }
        
        char unknown[2] = {current,