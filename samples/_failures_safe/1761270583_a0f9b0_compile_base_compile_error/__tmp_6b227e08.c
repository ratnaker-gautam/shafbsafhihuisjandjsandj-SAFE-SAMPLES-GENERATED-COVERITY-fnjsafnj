//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

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
    char *value;
    size_t line;
    size_t column;
};

struct LexerState {
    const char *input;
    size_t position;
    size_t line;
    size_t column;
    size_t input_length;
};

struct ParserState {
    struct Token *tokens;
    size_t token_count;
    size_t current_token;
};

struct ASTNode {
    char *type;
    char *value;
    struct ASTNode *left;
    struct ASTNode *right;
};

void init_lexer(struct LexerState *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->input_length = strlen(input);
}

int is_keyword(const char *str) {
    const char *keywords[] = {"if", "else", "while", "for", "return", "int", "float", "char", NULL};
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0) return 1;
    }
    return 0;
}

struct Token create_token(enum TokenType type, const char *value, size_t line, size_t column) {
    struct Token token;
    token.type = type;
    token.line = line;
    token.column = column;
    if (value != NULL) {
        token.value = malloc(strlen(value) + 1);
        if (token.value != NULL) {
            strcpy(token.value, value);
        }
    } else {
        token.value = NULL;
    }
    return token;
}

void free_token(struct Token *token) {
    if (token != NULL && token->value != NULL) {
        free(token->value);
    }
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int is_delimiter(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ';' || c == ',';
}

int get_next_token(struct LexerState *lexer, struct Token *token) {
    if (lexer == NULL || token == NULL) return 0;
    if (lexer->position >= lexer->input_length) {
        *token = create_token(TOKEN_EOF, "EOF", lexer->line, lexer->column);
        return 1;
    }
    while (lexer->position < lexer->input_length) {
        char current = lexer->input[lexer->position];
        if (isspace(current)) {
            if (current == '\n') {
                lexer->line++;
                lexer->column = 1;
            } else {
                lexer->column++;
            }
            lexer->position++;
            continue;
        }
        if (isalpha(current) || current == '_') {
            size_t start = lexer->position;
            while (lexer->position < lexer->input_length && 
                   (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_')) {
                lexer->position++;
                lexer->column++;
            }
            size_t length = lexer->position - start;
            char *ident = malloc(length + 1);
            if (ident == NULL) return 0;
            strncpy(ident, lexer->input + start, length);
            ident[length] = '\0';
            enum TokenType type = is_keyword(ident) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
            *token = create_token(type, ident, lexer->line, lexer->column - length);
            free(ident);
            return 1;
        }
        if (isdigit(current)) {
            size_t start = lexer->position;
            while (lexer->position < lexer->input_length && isdigit(lexer->input[lexer->position])) {
                lexer->position++;
                lexer->column++;
            }
            size_t length = lexer->position - start;
            char *num = malloc(length + 1);
            if (num == NULL) return 0;
            strncpy(num, lexer->input + start, length);
            num[length] = '\0';
            *token = create_token(TOKEN_NUMBER, num, lexer->line, lexer->column - length);
            free(num);
            return 1;
        }
        if (current == '"') {
            size_t start = lexer->position;
            lexer->position++;
            lexer->column++;
            while (lexer->position < lexer->input_length && lexer->input[lexer->position] != '"') {
                if (lexer->input[lexer->position] == '\n') {
                    lexer->line