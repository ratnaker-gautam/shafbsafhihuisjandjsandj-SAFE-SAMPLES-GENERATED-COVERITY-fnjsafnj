//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[32];
    int line;
    int column;
} Token;

typedef struct {
    const char *input;
    int position;
    int line;
    int column;
    Token current;
} Lexer;

typedef void (*TokenHandler)(Token);

void handle_number(Token token) {
    printf("Number: %s at line %d, column %d\n", token.value, token.line, token.column);
}

void handle_identifier(Token token) {
    printf("Identifier: %s at line %d, column %d\n", token.value, token.line, token.column);
}

void handle_operator(Token token) {
    printf("Operator: %s at line %d, column %d\n", token.value, token.line, token.column);
}

void handle_string(Token token) {
    printf("String: %s at line %d, column %d\n", token.value, token.line, token.column);
}

void handle_unknown(Token token) {
    printf("Unknown: %s at line %d, column %d\n", token.value, token.line, token.column);
}

void init_lexer(Lexer *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->current.type = TOKEN_UNKNOWN;
    memset(lexer->current.value, 0, sizeof(lexer->current.value));
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void next_token(Lexer *lexer) {
    if (lexer == NULL || lexer->input == NULL) return;
    
    while (lexer->input[lexer->position] != '\0' && isspace(lexer->input[lexer->position])) {
        if (lexer->input[lexer->position] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
    }
    
    if (lexer->input[lexer->position] == '\0') {
        lexer->current.type = TOKEN_EOF;
        memset(lexer->current.value, 0, sizeof(lexer->current.value));
        return;
    }
    
    char current_char = lexer->input[lexer->position];
    int value_index = 0;
    
    if (isdigit(current_char)) {
        lexer->current.type = TOKEN_NUMBER;
        while (lexer->input[lexer->position] != '\0' && isdigit(lexer->input[lexer->position])) {
            if (value_index < 31) {
                lexer->current.value[value_index++] = lexer->input[lexer->position];
            }
            lexer->position++;
            lexer->column++;
        }
    } else if (isalpha(current_char) || current_char == '_') {
        lexer->current.type = TOKEN_IDENTIFIER;
        while (lexer->input[lexer->position] != '\0' && 
               (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_')) {
            if (value_index < 31) {
                lexer->current.value[value_index++] = lexer->input[lexer->position];
            }
            lexer->position++;
            lexer->column++;
        }
    } else if (is_operator(current_char)) {
        lexer->current.type = TOKEN_OPERATOR;
        if (value_index < 31) {
            lexer->current.value[value_index++] = lexer->input[lexer->position];
        }
        lexer->position++;
        lexer->column++;
    } else if (current_char == '"') {
        lexer->current.type = TOKEN_STRING;
        lexer->position++;
        lexer->column++;
        while (lexer->input[lexer->position] != '\0' && lexer->input[lexer->position] != '"') {
            if (value_index < 31) {
                lexer->current.value[value_index++] = lexer->input[lexer->position];
            }
            lexer->position++;
            lexer->column++;
        }
        if (lexer->input[lexer->position] == '"') {
            lexer->position++;
            lexer->column++;
        }
    } else {
        lexer->current.type = TOKEN_UNKNOWN;
        if (value_index < 31) {
            lexer->current.value[value_index++] = lexer->input[lexer->position];
        }
        lexer->position++;
        lexer->column++;
    }
    
    lexer->current.value[value_index] = '\0';
    lexer->current.line = lexer->line;
    lexer->current.column = lexer->column;
}

void process_tokens(const char