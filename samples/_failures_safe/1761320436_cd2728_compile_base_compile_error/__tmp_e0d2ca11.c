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
    TOKEN_UNKNOWN
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
    int input_length;
};

struct Parser {
    struct Lexer *lexer;
    struct Token current_token;
    int has_error;
};

void lexer_init(struct Lexer *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->input_length = strlen(input);
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

void token_init(struct Token *token, enum TokenType type, const char *value, int line, int column) {
    if (token == NULL || value == NULL) return;
    token->type = type;
    strncpy(token->value, value, sizeof(token->value) - 1);
    token->value[sizeof(token->value) - 1] = '\0';
    token->line = line;
    token->column = column;
}

int lexer_get_next_token(struct Lexer *lexer, struct Token *token) {
    if (lexer == NULL || token == NULL) return 0;
    
    while (lexer->position < lexer->input_length) {
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
            int start = lexer->position;
            while (lexer->position < lexer->input_length && 
                   (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_')) {
                lexer->position++;
                lexer->column++;
            }
            
            int length = lexer->position - start;
            if (length >= (int)sizeof(token->value)) length = sizeof(token->value) - 1;
            
            char identifier[64];
            strncpy(identifier, lexer->input + start, length);
            identifier[length] = '\0';
            token_init(token, TOKEN_IDENTIFIER, identifier, lexer->line, lexer->column - length);
            return 1;
        }
        
        if (isdigit(current)) {
            int start = lexer->position;
            while (lexer->position < lexer->input_length && isdigit(lexer->input[lexer->position])) {
                lexer->position++;
                lexer->column++;
            }
            
            int length = lexer->position - start;
            if (length >= (int)sizeof(token->value)) length = sizeof(token->value) - 1;
            
            char number[64];
            strncpy(number, lexer->input + start, length);
            number[length] = '\0';
            token_init(token, TOKEN_NUMBER, number, lexer->line, lexer->column - length);
            return 1;
        }
        
        if (current == '"') {
            int start = lexer->position;
            lexer->position++;
            lexer->column++;
            
            while (lexer->position < lexer->input_length && lexer->input[lexer->position] != '"') {
                if (lexer->input[lexer->position] == '\n') {
                    lexer->line++;
                    lexer->column = 1;
                } else {
                    lexer->column++;
                }
                lexer->position++;
            }
            
            if (lexer->position < lexer->input_length && lexer->input[lexer->position] == '"') {
                lexer->position++;
                lexer->column++;
            }
            
            int length = lexer->position - start;
            if (length >= (int)sizeof(token->value)) length = sizeof(token->value) - 1;
            
            char string[64];
            strncpy(string, lexer->input + start, length);
            string[length] = '\0';
            token_init(token, TOKEN_STRING, string, lexer->line, lexer->column - length);
            return 1;
        }
        
        if (is_operator_char(current)) {
            char op[2] = {current, '\0'};
            token_init(token, TOKEN_OPERATOR, op, lexer->line, lexer->column);
            lexer->position++;
            lexer->column++;
            return 1;
        }
        
        char unknown[2] = {current, '\0'};
        token_init(token, TOKEN_UNKNOWN, unknown, lexer->line, lexer->column);
        lexer->position++;
        lexer->column++;
        return 1;