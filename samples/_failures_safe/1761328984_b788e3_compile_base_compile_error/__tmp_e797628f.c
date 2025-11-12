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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
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
    
    while (lexer->position < lexer->input_length && isspace(lexer->input[lexer->position])) {
        if (lexer->input[lexer->position] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
    }
    
    if (lexer->position >= lexer->input_length) {
        token_init(token, TOKEN_EOF, "", lexer->line, lexer->column);
        return 1;
    }
    
    char current = lexer->input[lexer->position];
    int start_pos = lexer->position;
    int start_line = lexer->line;
    int start_col = lexer->column;
    
    if (isalpha(current) || current == '_') {
        while (lexer->position < lexer->input_length && 
               (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_')) {
            lexer->position++;
            lexer->column++;
        }
        int length = lexer->position - start_pos;
        if (length >= (int)sizeof(token->value)) length = sizeof(token->value) - 1;
        char value[sizeof(token->value)];
        strncpy(value, lexer->input + start_pos, length);
        value[length] = '\0';
        token_init(token, TOKEN_IDENTIFIER, value, start_line, start_col);
        return 1;
    }
    
    if (isdigit(current)) {
        while (lexer->position < lexer->input_length && isdigit(lexer->input[lexer->position])) {
            lexer->position++;
            lexer->column++;
        }
        int length = lexer->position - start_pos;
        if (length >= (int)sizeof(token->value)) length = sizeof(token->value) - 1;
        char value[sizeof(token->value)];
        strncpy(value, lexer->input + start_pos, length);
        value[length] = '\0';
        token_init(token, TOKEN_NUMBER, value, start_line, start_col);
        return 1;
    }
    
    if (current == '"') {
        lexer->position++;
        lexer->column++;
        start_pos = lexer->position;
        start_col = lexer->column;
        
        while (lexer->position < lexer->input_length && lexer->input[lexer->position] != '"') {
            lexer->position++;
            lexer->column++;
        }
        
        if (lexer->position >= lexer->input_length) {
            token_init(token, TOKEN_UNKNOWN, "Unterminated string", start_line, start_col);
            return 1;
        }
        
        int length = lexer->position - start_pos;
        if (length >= (int)sizeof(token->value)) length = sizeof(token->value) - 1;
        char value[sizeof(token->value)];
        strncpy(value, lexer->input + start_pos, length);
        value[length] = '\0';
        token_init(token, TOKEN_STRING, value, start_line, start_col);
        
        lexer->position++;
        lexer->column++;
        return 1;
    }
    
    if (is_operator_char(current)) {
        char op[2] = {current, '\0'};
        token_init(token, TOKEN_OPERATOR, op, start_line, start_col);
        lexer->position++;
        lexer->column++;
        return 1;
    }
    
    char unknown[2] = {current, '\0'};
    token_init(token, TOKEN