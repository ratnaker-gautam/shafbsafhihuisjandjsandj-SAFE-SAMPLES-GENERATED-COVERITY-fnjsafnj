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
    TOKEN_PUNCTUATION,
    TOKEN_KEYWORD,
    TOKEN_EOF,
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char *value;
    int line;
    int column;
};

struct Lexer {
    char *input;
    int position;
    int line;
    int column;
    int length;
};

struct Parser {
    struct Lexer *lexer;
    struct Token current_token;
};

struct ASTNode {
    char *type;
    char *value;
    struct ASTNode **children;
    int child_count;
};

const char *keywords[] = {
    "if", "else", "while", "for", "return", "int", "char", "void"
};
const int num_keywords = 8;

const char *operators = "+-*/=<>!&|";
const char *punctuation = "(){}[];,";

void init_lexer(struct Lexer *lexer, char *input) {
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->length = (int)strlen(input);
}

int is_keyword(char *str) {
    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

struct Token get_next_token(struct Lexer *lexer) {
    struct Token token;
    token.type = TOKEN_INVALID;
    token.value = NULL;
    token.line = lexer->line;
    token.column = lexer->column;

    if (lexer->position >= lexer->length) {
        token.type = TOKEN_EOF;
        return token;
    }

    char current = lexer->input[lexer->position];
    
    while (isspace(current)) {
        if (current == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
        if (lexer->position >= lexer->length) {
            token.type = TOKEN_EOF;
            return token;
        }
        current = lexer->input[lexer->position];
    }

    token.line = lexer->line;
    token.column = lexer->column;

    if (isalpha(current) || current == '_') {
        int start = lexer->position;
        while (lexer->position < lexer->length && 
               (isalnum(lexer->input[lexer->position]) || 
                lexer->input[lexer->position] == '_')) {
            lexer->position++;
            lexer->column++;
        }
        int len = lexer->position - start;
        token.value = malloc(len + 1);
        if (token.value == NULL) {
            token.type = TOKEN_INVALID;
            return token;
        }
        strncpy(token.value, lexer->input + start, len);
        token.value[len] = '\0';
        
        if (is_keyword(token.value)) {
            token.type = TOKEN_KEYWORD;
        } else {
            token.type = TOKEN_IDENTIFIER;
        }
    }
    else if (isdigit(current)) {
        int start = lexer->position;
        while (lexer->position < lexer->length && 
               isdigit(lexer->input[lexer->position])) {
            lexer->position++;
            lexer->column++;
        }
        int len = lexer->position - start;
        token.value = malloc(len + 1);
        if (token.value == NULL) {
            token.type = TOKEN_INVALID;
            return token;
        }
        strncpy(token.value, lexer->input + start, len);
        token.value[len] = '\0';
        token.type = TOKEN_NUMBER;
    }
    else if (current == '"') {
        int start = lexer->position;
        lexer->position++;
        lexer->column++;
        while (lexer->position < lexer->length && 
               lexer->input[lexer->position] != '"') {
            if (lexer->input[lexer->position] == '\n') {
                lexer->line++;
                lexer->column = 1;
            } else {
                lexer->column++;
            }
            lexer->position++;
        }
        if (lexer->position >= lexer->length) {
            token.type = TOKEN_INVALID;
            return token;
        }
        lexer->position++;
        lexer->column++;
        int len = lexer->position - start;
        token.value = malloc(len + 1);
        if (token.value == NULL) {
            token.type = TOKEN_INVALID;
            return token;
        }
        strncpy(token.value, lexer->input + start, len);
        token.value[len] = '\0';
        token.type = TOKEN_STRING;
    }
    else if (strchr(operators, current) != NULL) {
        token.value = malloc(2);
        if (token.value == NULL) {
            token.type = TOKEN_INVALID;
            return token;
        }
        token.value[0] = current