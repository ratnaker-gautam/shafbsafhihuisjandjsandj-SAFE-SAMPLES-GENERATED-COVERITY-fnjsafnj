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
    int length;
};

struct Parser {
    struct Lexer *lexer;
    struct Token current_token;
    int error_count;
};

struct ASTNode {
    enum { NODE_IDENTIFIER, NODE_NUMBER, NODE_STRING, NODE_BINARY_OP } type;
    char value[32];
    struct ASTNode *left;
    struct ASTNode *right;
    int line;
    int column;
};

void init_lexer(struct Lexer *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->length = strlen(input);
}

int is_keyword(const char *str) {
    if (str == NULL) return 0;
    static const char *keywords[] = {"if", "else", "while", "for", "return", NULL};
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

struct Token get_next_token(struct Lexer *lexer) {
    struct Token token = {TOKEN_ERROR, "", 0, 0};
    if (lexer == NULL || lexer->input == NULL) return token;
    
    token.line = lexer->line;
    token.column = lexer->column;
    
    if (lexer->position >= lexer->length) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char current = lexer->input[lexer->position];
    
    while (isspace((unsigned char)current)) {
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
    
    if (isalpha((unsigned char)current) || current == '_') {
        int i = 0;
        while ((isalnum((unsigned char)current) || current == '_') && i < 31) {
            token.value[i++] = current;
            lexer->position++;
            lexer->column++;
            if (lexer->position >= lexer->length) break;
            current = lexer->input[lexer->position];
        }
        token.value[i] = '\0';
        token.type = is_keyword(token.value) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
    } else if (isdigit((unsigned char)current)) {
        int i = 0;
        while (isdigit((unsigned char)current) && i < 31) {
            token.value[i++] = current;
            lexer->position++;
            lexer->column++;
            if (lexer->position >= lexer->length) break;
            current = lexer->input[lexer->position];
        }
        token.value[i] = '\0';
        token.type = TOKEN_NUMBER;
    } else if (current == '"') {
        int i = 0;
        lexer->position++;
        lexer->column++;
        if (lexer->position >= lexer->length) {
            token.type = TOKEN_ERROR;
            return token;
        }
        current = lexer->input[lexer->position];
        while (current != '"' && i < 31) {
            if (lexer->position >= lexer->length) {
                token.type = TOKEN_ERROR;
                return token;
            }
            token.value[i++] = current;
            lexer->position++;
            lexer->column++;
            if (lexer->position >= lexer->length) break;
            current = lexer->input[lexer->position];
        }
        token.value[i] = '\0';
        if (current == '"') {
            lexer->position++;
            lexer->column++;
            token.type = TOKEN_STRING;
        } else {
            token.type = TOKEN_ERROR;
        }
    } else if (strchr("+-*/=<>!&|", current) != NULL) {
        token.value[0] = current;
        token.value[1] = '\0';
        token.type = TOKEN_OPERATOR;
        lexer->position++;
        lexer->column++;
    } else if (strchr("();{},[]", current) != NULL) {
        token.value[0] = current;
        token.value[1] = '\0';
        token.type = TOKEN_PUNCTUATION;
        lexer->position++;
        lexer->column++;
    } else {
        token