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
    char value[64];
    int line;
    int column;
};

struct Lexer {
    const char *source;
    size_t position;
    size_t length;
    int line;
    int column;
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

void lexer_init(struct Lexer *lexer, const char *source) {
    lexer->source = source;
    lexer->position = 0;
    lexer->length = strlen(source);
    lexer->line = 1;
    lexer->column = 1;
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int is_keyword(const char *str) {
    const char *keywords[] = {"if", "else", "while", "for", "return", NULL};
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

struct Token lexer_next_token(struct Lexer *lexer) {
    struct Token token = {TOKEN_UNKNOWN, "", lexer->line, lexer->column};
    
    while (lexer->position < lexer->length) {
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
        
        if (isalpha(c) || c == '_') {
            size_t start = lexer->position;
            while (lexer->position < lexer->length && 
                   (isalnum(lexer->source[lexer->position]) || 
                    lexer->source[lexer->position] == '_')) {
                lexer->position++;
                lexer->column++;
            }
            
            size_t len = lexer->position - start;
            if (len >= sizeof(token.value)) len = sizeof(token.value) - 1;
            strncpy(token.value, lexer->source + start, len);
            token.value[len] = '\0';
            
            token.type = is_keyword(token.value) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
            return token;
        }
        
        if (isdigit(c)) {
            size_t start = lexer->position;
            while (lexer->position < lexer->length && 
                   isdigit(lexer->source[lexer->position])) {
                lexer->position++;
                lexer->column++;
            }
            
            size_t len = lexer->position - start;
            if (len >= sizeof(token.value)) len = sizeof(token.value) - 1;
            strncpy(token.value, lexer->source + start, len);
            token.value[len] = '\0';
            
            token.type = TOKEN_NUMBER;
            return token;
        }
        
        if (c == '"') {
            size_t start = ++lexer->position;
            lexer->column++;
            
            while (lexer->position < lexer->length && 
                   lexer->source[lexer->position] != '"') {
                lexer->position++;
                lexer->column++;
            }
            
            if (lexer->position < lexer->length) {
                size_t len = lexer->position - start;
                if (len >= sizeof(token.value)) len = sizeof(token.value) - 1;
                strncpy(token.value, lexer->source + start, len);
                token.value[len] = '\0';
                lexer->position++;
                lexer->column++;
            }
            
            token.type = TOKEN_STRING;
            return token;
        }
        
        if (is_operator_char(c)) {
            size_t start = lexer->position;
            while (lexer->position < lexer->length && 
                   is_operator_char(lexer->source[lexer->position])) {
                lexer->position++;
                lexer->column++;
            }
            
            size_t len = lexer->position - start;
            if (len >= sizeof(token.value)) len = sizeof(token.value) - 1;
            strncpy(token.value, lexer->source + start, len);
            token.value[len] = '\0';
            
            token.type = TOKEN_OPERATOR;
            return token;
        }
        
        if (strchr(".,;:(){}[]", c)) {
            token.value[0] = c;
            token.value[1] = '\0';
            token.type = TOKEN_PUNCTUATION;