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
    const char *source;
    size_t position;
    size_t length;
    int line;
    int column;
    struct Token current_token;
};

struct Parser {
    struct Lexer *lexer;
    int error_count;
};

struct ASTNode {
    enum {
        NODE_IDENTIFIER,
        NODE_NUMBER,
        NODE_STRING,
        NODE_BINARY_OP,
        NODE_ASSIGNMENT
    } type;
    union {
        char identifier[32];
        double number;
        char string[32];
        struct {
            struct ASTNode *left;
            struct ASTNode *right;
            char operator[4];
        } binary_op;
        struct {
            char identifier[32];
            struct ASTNode *value;
        } assignment;
    } data;
};

void init_lexer(struct Lexer *lexer, const char *source) {
    if (lexer == NULL || source == NULL) return;
    lexer->source = source;
    lexer->position = 0;
    lexer->length = strlen(source);
    lexer->line = 1;
    lexer->column = 1;
    lexer->current_token.type = TOKEN_ERROR;
    memset(lexer->current_token.value, 0, sizeof(lexer->current_token.value));
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

void next_token(struct Lexer *lexer) {
    if (lexer == NULL || lexer->source == NULL) return;
    
    while (lexer->position < lexer->length && isspace(lexer->source[lexer->position])) {
        if (lexer->source[lexer->position] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
    }

    if (lexer->position >= lexer->length) {
        lexer->current_token.type = TOKEN_EOF;
        memset(lexer->current_token.value, 0, sizeof(lexer->current_token.value));
        return;
    }

    char current = lexer->source[lexer->position];
    lexer->current_token.line = lexer->line;
    lexer->current_token.column = lexer->column;
    memset(lexer->current_token.value, 0, sizeof(lexer->current_token.value));

    if (isalpha(current) || current == '_') {
        int i = 0;
        while (lexer->position < lexer->length && 
               (isalnum(lexer->source[lexer->position]) || lexer->source[lexer->position] == '_') &&
               i < 31) {
            lexer->current_token.value[i++] = lexer->source[lexer->position++];
            lexer->column++;
        }
        lexer->current_token.value[i] = '\0';
        lexer->current_token.type = is_keyword(lexer->current_token.value) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
    } else if (isdigit(current)) {
        int i = 0;
        while (lexer->position < lexer->length && 
               (isdigit(lexer->source[lexer->position]) || lexer->source[lexer->position] == '.') &&
               i < 31) {
            lexer->current_token.value[i++] = lexer->source[lexer->position++];
            lexer->column++;
        }
        lexer->current_token.value[i] = '\0';
        lexer->current_token.type = TOKEN_NUMBER;
    } else if (current == '"') {
        int i = 0;
        lexer->position++;
        lexer->column++;
        while (lexer->position < lexer->length && lexer->source[lexer->position] != '"' && i < 31) {
            lexer->current_token.value[i++] = lexer->source[lexer->position++];
            lexer->column++;
        }
        if (lexer->position < lexer->length && lexer->source[lexer->position] == '"') {
            lexer->position++;
            lexer->column++;
        } else {
            lexer->current_token.type = TOKEN_ERROR;
        }
        lexer->current_token.value[i] = '\0';
        lexer->current_token.type = TOKEN_STRING;
    } else if (strchr("+-*/=<>!&|", current) != NULL) {
        int i = 0;
        while (lexer->position < lexer->length && 
               strchr("+-*/=<>!&