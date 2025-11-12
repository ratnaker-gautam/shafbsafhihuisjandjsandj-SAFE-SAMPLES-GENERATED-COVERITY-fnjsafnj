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

struct Lexer {
    const char *input;
    int position;
    int line;
    int column;
    int error_count;
};

struct Parser {
    struct Token *tokens;
    int token_count;
    int current_token;
    int error_count;
};

void lexer_init(struct Lexer *lexer, const char *input) {
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->error_count = 0;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void token_init(struct Token *token, enum TokenType type, const char *value, int line, int column) {
    token->type = type;
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
    token->line = line;
    token->column = column;
}

int lexer_next_token(struct Lexer *lexer, struct Token *token) {
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
            char buffer[MAX_TOKEN_LEN] = {0};
            int buf_pos = 0;
            int start_col = lexer->column;
            
            while ((isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') && 
                   buf_pos < MAX_TOKEN_LEN - 1) {
                buffer[buf_pos++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            buffer[buf_pos] = '\0';
            token_init(token, TOKEN_IDENTIFIER, buffer, lexer->line, start_col);
            return 1;
        }
        
        if (isdigit(current)) {
            char buffer[MAX_TOKEN_LEN] = {0};
            int buf_pos = 0;
            int start_col = lexer->column;
            
            while (isdigit(lexer->input[lexer->position]) && buf_pos < MAX_TOKEN_LEN - 1) {
                buffer[buf_pos++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            buffer[buf_pos] = '\0';
            token_init(token, TOKEN_NUMBER, buffer, lexer->line, start_col);
            return 1;
        }
        
        if (is_operator_char(current)) {
            char buffer[3] = {0};
            int start_col = lexer->column;
            buffer[0] = current;
            lexer->position++;
            lexer->column++;
            
            if (is_operator_char(lexer->input[lexer->position]) && 
                lexer->input[lexer->position] != '\0') {
                buffer[1] = lexer->input[lexer->position];
                lexer->position++;
                lexer->column++;
            }
            token_init(token, TOKEN_OPERATOR, buffer, lexer->line, start_col);
            return 1;
        }
        
        if (current == '"') {
            char buffer[MAX_TOKEN_LEN] = {0};
            int buf_pos = 0;
            int start_col = lexer->column;
            
            buffer[buf_pos++] = current;
            lexer->position++;
            lexer->column++;
            
            while (lexer->input[lexer->position] != '"' && lexer->input[lexer->position] != '\0' && 
                   buf_pos < MAX_TOKEN_LEN - 1) {
                buffer[buf_pos++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            
            if (lexer->input[lexer->position] == '"') {
                buffer[buf_pos++] = lexer->input[lexer->position++];
                lexer->column++;
                buffer[buf_pos] = '\0';
                token_init(token, TOKEN_STRING, buffer, lexer->line, start_col);
                return 1;
            } else {
                lexer->error_count++;
                return 0;
            }
        }
        
        if (current == '(' || current == ')' || current == ';' || current == ',' || current == '{' || current == '}') {
            char buffer[2] = {current, '\0'};
            int start_col = lexer