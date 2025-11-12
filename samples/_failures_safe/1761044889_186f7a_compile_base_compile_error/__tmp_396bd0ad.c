//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
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

struct LexerState {
    const char *input;
    int position;
    int line;
    int column;
    int error_count;
};

struct ParserNode {
    enum { NODE_EXPRESSION, NODE_STATEMENT, NODE_DECLARATION } type;
    struct Token token;
    struct ParserNode *left;
    struct ParserNode *right;
};

struct ParserState {
    struct Token current_token;
    struct LexerState *lexer;
    int error_count;
};

void init_lexer(struct LexerState *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->error_count = 0;
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int is_delimiter(char c) {
    return strchr("(){}[],;", c) != NULL;
}

int is_keyword(const char *str) {
    if (str == NULL) return 0;
    const char *keywords[] = {"if", "else", "while", "for", "return", "int", "char", "void"};
    for (size_t i = 0; i < sizeof(keywords)/sizeof(keywords[0]); i++) {
        if (strcmp(str, keywords[i]) == 0) return 1;
    }
    return 0;
}

struct Token get_next_token(struct LexerState *lexer) {
    struct Token token = {TOKEN_UNKNOWN, "", 0, 0};
    if (lexer == NULL || lexer->input == NULL) return token;
    
    token.line = lexer->line;
    token.column = lexer->column;
    
    if (lexer->input[lexer->position] == '\0') {
        token.type = TOKEN_EOF;
        strncpy(token.value, "", sizeof(token.value)-1);
        return token;
    }
    
    while (isspace((unsigned char)lexer->input[lexer->position])) {
        if (lexer->input[lexer->position] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
    }
    
    token.line = lexer->line;
    token.column = lexer->column;
    
    char current = lexer->input[lexer->position];
    
    if (isalpha((unsigned char)current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (i < 63 && (isalnum((unsigned char)lexer->input[lexer->position]) || lexer->input[lexer->position] == '_')) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
        if (is_keyword(token.value)) {
            token.type = TOKEN_KEYWORD;
        }
    } else if (isdigit((unsigned char)current)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (i < 63 && isdigit((unsigned char)lexer->input[lexer->position])) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        int i = 0;
        lexer->position++;
        lexer->column++;
        while (i < 63 && lexer->input[lexer->position] != '"' && lexer->input[lexer->position] != '\0') {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
        if (lexer->input[lexer->position] == '"') {
            lexer->position++;
            lexer->column++;
        } else {
            lexer->error_count++;
        }
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (i < 63 && is_operator_char(lexer->input[lexer->position])) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
    } else if (is_delimiter(current)) {
        token.type = TOKEN_DELIMITER;
        token.value[0] = lexer->input[lexer->position++];
        token.value[1] = '\0';
        lexer->column++;
    } else {
        token.type = TOKEN_UNKNOWN;