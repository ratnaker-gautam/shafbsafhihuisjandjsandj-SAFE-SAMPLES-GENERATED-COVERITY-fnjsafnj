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
    int position;
    int line;
    int column;
    int source_length;
};

struct Parser {
    struct Lexer *lexer;
    struct Token current_token;
    int error_count;
};

struct ASTNode {
    enum { NODE_IDENTIFIER, NODE_NUMBER, NODE_STRING, NODE_BINARY_OP } type;
    union {
        char identifier[32];
        double number;
        char string[32];
        struct {
            struct ASTNode *left;
            struct ASTNode *right;
            char operator[4];
        } binary_op;
    } value;
};

void init_lexer(struct Lexer *lexer, const char *source) {
    lexer->source = source;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->source_length = (int)strlen(source);
}

int is_keyword(const char *str) {
    static const char *keywords[] = {"if", "else", "while", "for", "return", NULL};
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

struct Token get_next_token(struct Lexer *lexer) {
    struct Token token = {TOKEN_ERROR, "", lexer->line, lexer->column};
    
    while (lexer->position < lexer->source_length) {
        char c = lexer->source[lexer->position];
        
        if (isspace(c)) {
            if (c == '\n') {
                lexer->line++;
                lexer->column = 1;
            } else {
                lexer->column++;
            }
            lexer->position++;
            continue;
        }
        
        token.line = lexer->line;
        token.column = lexer->column;
        
        if (isalpha(c) || c == '_') {
            int i = 0;
            while ((lexer->position < lexer->source_length) && 
                   (isalnum(lexer->source[lexer->position]) || 
                    lexer->source[lexer->position] == '_') && i < 31) {
                token.value[i++] = lexer->source[lexer->position++];
                lexer->column++;
            }
            token.value[i] = '\0';
            token.type = is_keyword(token.value) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
            return token;
        }
        
        if (isdigit(c)) {
            int i = 0;
            while ((lexer->position < lexer->source_length) && 
                   (isdigit(lexer->source[lexer->position]) || 
                    lexer->source[lexer->position] == '.') && i < 31) {
                token.value[i++] = lexer->source[lexer->position++];
                lexer->column++;
            }
            token.value[i] = '\0';
            token.type = TOKEN_NUMBER;
            return token;
        }
        
        if (c == '"') {
            int i = 0;
            lexer->position++;
            lexer->column++;
            while ((lexer->position < lexer->source_length) && 
                   (lexer->source[lexer->position] != '"') && i < 31) {
                token.value[i++] = lexer->source[lexer->position++];
                lexer->column++;
            }
            token.value[i] = '\0';
            if (lexer->position < lexer->source_length && lexer->source[lexer->position] == '"') {
                lexer->position++;
                lexer->column++;
            }
            token.type = TOKEN_STRING;
            return token;
        }
        
        if (strchr("+-*/=<>!&|", c)) {
            int i = 0;
            token.value[i++] = c;
            lexer->position++;
            lexer->column++;
            if (lexer->position < lexer->source_length && strchr("=+-", lexer->source[lexer->position])) {
                token.value[i++] = lexer->source[lexer->position++];
                lexer->column++;
            }
            token.value[i] = '\0';
            token.type = TOKEN_OPERATOR;
            return token;
        }
        
        if (strchr("(),;{}[]", c)) {
            token.value[0] = c;
            token.value[1] = '\0';
            token.type = TOKEN_PUNCTUATION;
            lexer->position++;
            lexer->column++;
            return token;
        }
        
        token.value[0] = c;
        token.value[1] = '\0';
        lexer->position++;
        lexer->column++;
        return token;
    }
    
    token.type = TOKEN_EOF;
    token.value[0] = '\0';
    return token;
}

void init