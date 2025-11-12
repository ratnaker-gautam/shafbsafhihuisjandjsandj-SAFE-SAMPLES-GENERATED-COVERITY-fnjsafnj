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
    int input_length;
};

struct Parser {
    struct Lexer *lexer;
    struct Token current_token;
    int has_error;
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

void init_lexer(struct Lexer *lexer, const char *input) {
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->input_length = (int)strlen(input);
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
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

void skip_whitespace(struct Lexer *lexer) {
    while (lexer->position < lexer->input_length && isspace(lexer->input[lexer->position])) {
        if (lexer->input[lexer->position] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
    }
}

struct Token get_next_token(struct Lexer *lexer) {
    struct Token token = {TOKEN_ERROR, "", lexer->line, lexer->column};
    
    skip_whitespace(lexer);
    
    if (lexer->position >= lexer->input_length) {
        token.type = TOKEN_EOF;
        strcpy(token.value, "");
        return token;
    }
    
    char current = lexer->input[lexer->position];
    
    if (isalpha(current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (lexer->position < lexer->input_length && 
               (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') &&
               i < 31) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
        
        if (is_keyword(token.value)) {
            token.type = TOKEN_KEYWORD;
        }
    }
    else if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        int dot_count = 0;
        while (lexer->position < lexer->input_length && 
               (isdigit(lexer->input[lexer->position]) || lexer->input[lexer->position] == '.') &&
               i < 31) {
            if (lexer->input[lexer->position] == '.') {
                dot_count++;
            }
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
    }
    else if (current == '"') {
        token.type = TOKEN_STRING;
        int i = 0;
        lexer->position++;
        lexer->column++;
        
        while (lexer->position < lexer->input_length && 
               lexer->input[lexer->position] != '"' && i < 31) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        
        if (lexer->position < lexer->input_length && lexer->input[lexer->position] == '"') {
            lexer->position++;
            lexer->column++;
        }
        token.value[i] = '\0';
    }
    else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (lexer->position < lexer->input_length && 
               is_operator_char(lexer->input[lexer->position]) && i < 3) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
    }
    else if (strchr("(){}[],;:", current)) {
        token.type = TOKEN_PUNCTUATION;
        token.value[0] = current;
        token.value[1] = '\0';