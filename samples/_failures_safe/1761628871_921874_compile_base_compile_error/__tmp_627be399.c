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
    int length;
};

struct Parser {
    struct Lexer *lexer;
    struct Token current_token;
    int error_count;
};

struct ASTNode {
    enum { NODE_IDENTIFIER, NODE_NUMBER, NODE_STRING, NODE_BINARY_OP } type;
    union {
        char identifier[64];
        double number;
        char string[64];
        struct {
            struct ASTNode *left;
            struct ASTNode *right;
            char operator[8];
        } binary_op;
    } value;
};

void init_lexer(struct Lexer *lexer, const char *input) {
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->length = (int)strlen(input);
}

int is_keyword(const char *str) {
    const char *keywords[] = {"if", "else", "while", "for", "return", "int", "float", "char", "void"};
    for (size_t i = 0; i < sizeof(keywords)/sizeof(keywords[0]); i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void next_token(struct Lexer *lexer, struct Token *token) {
    while (lexer->position < lexer->length && isspace(lexer->input[lexer->position])) {
        if (lexer->input[lexer->position] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
    }

    if (lexer->position >= lexer->length) {
        token->type = TOKEN_EOF;
        strcpy(token->value, "");
        token->line = lexer->line;
        token->column = lexer->column;
        return;
    }

    char current = lexer->input[lexer->position];
    token->line = lexer->line;
    token->column = lexer->column;

    if (isalpha(current) || current == '_') {
        int i = 0;
        while ((lexer->position < lexer->length) && 
               (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') &&
               i < 63) {
            token->value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token->value[i] = '\0';
        token->type = is_keyword(token->value) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
    } else if (isdigit(current)) {
        int i = 0;
        while ((lexer->position < lexer->length) && 
               (isdigit(lexer->input[lexer->position]) || lexer->input[lexer->position] == '.') &&
               i < 63) {
            token->value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_NUMBER;
    } else if (current == '"') {
        int i = 0;
        lexer->position++;
        lexer->column++;
        while ((lexer->position < lexer->length) && 
               (lexer->input[lexer->position] != '"') && i < 63) {
            token->value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        if (lexer->position < lexer->length && lexer->input[lexer->position] == '"') {
            lexer->position++;
            lexer->column++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_STRING;
    } else if (strchr("+-*/=<>!&|", current)) {
        int i = 0;
        while ((lexer->position < lexer->length) && 
               strchr("+-*/=<>!&|", lexer->input[lexer->position]) && i < 63) {
            token->value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_OPERATOR;
    } else if (strchr("(),;{}[]", current)) {
        token->value[0] = lexer->input[lexer->position++];
        token->value[1] = '\0';
        lexer->column++;
        token->type = TOKEN_DELIMITER;
    } else {
        token->value[0] = lexer->input[lexer->position++];
        token->