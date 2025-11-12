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
    TOKEN_UNKNOWN
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
const int keyword_count = 8;

void init_lexer(struct Lexer *lexer, char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->length = strlen(input);
}

void free_token(struct Token *token) {
    if (token != NULL && token->value != NULL) {
        free(token->value);
        token->value = NULL;
    }
}

int is_keyword(char *str) {
    if (str == NULL) return 0;
    for (int i = 0; i < keyword_count; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

struct Token next_token(struct Lexer *lexer) {
    struct Token token = {TOKEN_UNKNOWN, NULL, lexer->line, lexer->column};
    
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
               (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_')) {
            lexer->position++;
            lexer->column++;
        }
        int length = lexer->position - start;
        token.value = malloc(length + 1);
        if (token.value != NULL) {
            strncpy(token.value, lexer->input + start, length);
            token.value[length] = '\0';
            token.type = is_keyword(token.value) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
        }
    } else if (isdigit(current)) {
        int start = lexer->position;
        while (lexer->position < lexer->length && isdigit(lexer->input[lexer->position])) {
            lexer->position++;
            lexer->column++;
        }
        int length = lexer->position - start;
        token.value = malloc(length + 1);
        if (token.value != NULL) {
            strncpy(token.value, lexer->input + start, length);
            token.value[length] = '\0';
            token.type = TOKEN_NUMBER;
        }
    } else if (current == '"') {
        int start = lexer->position;
        lexer->position++;
        lexer->column++;
        while (lexer->position < lexer->length && lexer->input[lexer->position] != '"') {
            if (lexer->input[lexer->position] == '\n') {
                lexer->line++;
                lexer->column = 1;
            } else {
                lexer->column++;
            }
            lexer->position++;
        }
        if (lexer->position < lexer->length && lexer->input[lexer->position] == '"') {
            lexer->position++;
            lexer->column++;
        }
        int length = lexer->position - start;
        token.value = malloc(length + 1);
        if (token.value != NULL) {
            strncpy(token.value, lexer->input + start, length);
            token.value[length] = '\0';
            token.type = TOKEN_STRING;
        }
    } else if (strchr("+-*/=<>!&|", current) != NULL) {
        token.value = malloc(3);
        if (token.value != NULL) {
            token.value[0] = current;
            token.value[1] = '\0';
            lexer->position++;
            lexer->column++;
            if (lexer->position < lexer->length) {
                char next = lexer->input[lexer->position];