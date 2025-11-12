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
    int length;
};

struct ParserNode {
    enum TokenType node_type;
    char value[64];
    struct ParserNode *left;
    struct ParserNode *right;
    int line;
    int column;
};

struct ParserState {
    struct Token *tokens;
    int token_count;
    int current_token;
};

int is_keyword(const char *str) {
    const char *keywords[] = {"if", "else", "while", "for", "return", "int", "float", "char", "void"};
    int num_keywords = 9;
    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int is_delimiter(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == ';' || c == ',' || c == '[';
}

void init_lexer(struct LexerState *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->length = strlen(input);
}

struct Token get_next_token(struct LexerState *lexer) {
    struct Token token = {TOKEN_UNKNOWN, "", lexer->line, lexer->column};
    
    if (lexer == NULL || lexer->position >= lexer->length) {
        token.type = TOKEN_EOF;
        return token;
    }
    
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
        token.type = TOKEN_EOF;
        return token;
    }
    
    char current = lexer->input[lexer->position];
    token.line = lexer->line;
    token.column = lexer->column;
    
    if (isalpha(current) || current == '_') {
        int i = 0;
        while (lexer->position < lexer->length && 
               (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') &&
               i < 63) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
        token.type = is_keyword(token.value) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
    }
    else if (isdigit(current)) {
        int i = 0;
        while (lexer->position < lexer->length && 
               (isdigit(lexer->input[lexer->position]) || lexer->input[lexer->position] == '.') &&
               i < 63) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
        token.type = TOKEN_NUMBER;
    }
    else if (current == '"') {
        int i = 0;
        lexer->position++;
        lexer->column++;
        while (lexer->position < lexer->length && 
               lexer->input[lexer->position] != '"' && i < 63) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
        if (lexer->position < lexer->length && lexer->input[lexer->position] == '"') {
            lexer->position++;
            lexer->column++;
        }
        token.type = TOKEN_STRING;
    }
    else if (is_operator(current)) {
        token.value[0] = current;
        token.value[1] = '\0';
        lexer->position++;
        lexer->column++;
        token.type = TOKEN_OPERATOR;
    }
    else if (is_delimiter(current)) {
        token.value[0] = current;
        token.value[1] = '\0';
        lexer->position++;
        lexer->column++;
        token.type = TOKEN_DELIMITER;
    }
    else {
        token.value[0] = current;
        token.value[