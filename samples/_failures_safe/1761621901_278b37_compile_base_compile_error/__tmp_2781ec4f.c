//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
    TOKEN_KEYWORD,
    TOKEN_EOF,
    TOKEN_INVALID
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

const char *keywords[] = {
    "if", "else", "while", "for", "return", "int", "float", "char", "void"
};
const int num_keywords = 9;

void init_lexer(struct Lexer *lexer, const char *input) {
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->input_length = strlen(input);
}

int is_keyword(const char *str) {
    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

struct Token get_next_token(struct Lexer *lexer) {
    struct Token token = {TOKEN_INVALID, "", lexer->line, lexer->column};
    
    if (lexer->position >= lexer->input_length) {
        token.type = TOKEN_EOF;
        strcpy(token.value, "");
        return token;
    }
    
    char current_char = lexer->input[lexer->position];
    
    while (isspace(current_char)) {
        if (current_char == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
        if (lexer->position >= lexer->input_length) {
            token.type = TOKEN_EOF;
            return token;
        }
        current_char = lexer->input[lexer->position];
    }
    
    token.line = lexer->line;
    token.column = lexer->column;
    
    if (isalpha(current_char) || current_char == '_') {
        int value_pos = 0;
        while ((isalnum(current_char) || current_char == '_') && 
               value_pos < 63 && lexer->position < lexer->input_length) {
            token.value[value_pos++] = current_char;
            lexer->position++;
            lexer->column++;
            if (lexer->position < lexer->input_length) {
                current_char = lexer->input[lexer->position];
            } else {
                break;
            }
        }
        token.value[value_pos] = '\0';
        token.type = is_keyword(token.value) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
    }
    else if (isdigit(current_char)) {
        int value_pos = 0;
        int has_dot = 0;
        while ((isdigit(current_char) || current_char == '.') && 
               value_pos < 63 && lexer->position < lexer->input_length) {
            if (current_char == '.') {
                if (has_dot) break;
                has_dot = 1;
            }
            token.value[value_pos++] = current_char;
            lexer->position++;
            lexer->column++;
            if (lexer->position < lexer->input_length) {
                current_char = lexer->input[lexer->position];
            } else {
                break;
            }
        }
        token.value[value_pos] = '\0';
        token.type = TOKEN_NUMBER;
    }
    else if (current_char == '"') {
        int value_pos = 0;
        lexer->position++;
        lexer->column++;
        while (lexer->position < lexer->input_length && 
               value_pos < 63) {
            current_char = lexer->input[lexer->position];
            if (current_char == '"') {
                lexer->position++;
                lexer->column++;
                break;
            }
            if (current_char == '\\') {
                lexer->position++;
                lexer->column++;
                if (lexer->position < lexer->input_length) {
                    current_char = lexer->input[lexer->position];
                }
            }
            token.value[value_pos++] = current_char;
            lexer->position++;
            lexer->column++;
        }
        token.value[value_pos] = '\0';
        token.type = TOKEN_STRING;
    }
    else if (strchr("+-*/=<>!&|", current_char)) {
        token.value[0] = current_char;
        token.value[1] = '\0';
        lexer->position++;
        lexer->column++;
        if (lexer->position < lexer->input_length) {
            char next_char = lexer->input[lexer->position];
            if ((current_char == '<' && next_char == '=') ||
                (current_char == '>' && next_char == '=') ||
                (current_char == '!' && next_char == '=') ||
                (current_char == '=' && next_char == '=') ||