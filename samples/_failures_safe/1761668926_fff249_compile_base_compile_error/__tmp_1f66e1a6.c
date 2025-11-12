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
    struct ASTNode **children;
    size_t child_count;
};

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int is_delimiter(char c) {
    return strchr("(),;{}[]", c) != NULL;
}

int is_keyword(const char *str) {
    const char *keywords[] = {"if", "else", "while", "for", "return", "int", "float", "char", "void"};
    size_t count = sizeof(keywords) / sizeof(keywords[0]);
    for (size_t i = 0; i < count; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void init_lexer(struct LexerState *lexer, const char *input) {
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->input_length = strlen(input);
}

void skip_whitespace(struct LexerState *lexer) {
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

struct Token *create_token(enum TokenType type, const char *value, size_t line, size_t column) {
    struct Token *token = malloc(sizeof(struct Token));
    if (!token) return NULL;
    
    token->type = type;
    token->value = malloc(strlen(value) + 1);
    if (!token->value) {
        free(token);
        return NULL;
    }
    strcpy(token->value, value);
    token->line = line;
    token->column = column;
    
    return token;
}

void free_token(struct Token *token) {
    if (token) {
        free(token->value);
        free(token);
    }
}

struct Token *next_token(struct LexerState *lexer) {
    skip_whitespace(lexer);
    
    if (lexer->position >= lexer->input_length) {
        return create_token(TOKEN_EOF, "", lexer->line, lexer->column);
    }
    
    char current = lexer->input[lexer->position];
    size_t start_line = lexer->line;
    size_t start_column = lexer->column;
    
    if (isalpha(current) || current == '_') {
        size_t start = lexer->position;
        while (lexer->position < lexer->input_length && 
               (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_')) {
            lexer->position++;
            lexer->column++;
        }
        
        size_t length = lexer->position - start;
        char *value = malloc(length + 1);
        if (!value) return NULL;
        strncpy(value, lexer->input + start, length);
        value[length] = '\0';
        
        enum TokenType type = is_keyword(value) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
        struct Token *token = create_token(type, value, start_line, start_column);
        free(value);
        return token;
    }
    
    if (isdigit(current)) {
        size_t start = lexer->position;
        while (lexer->position < lexer->input_length && 
               (isdigit(lexer->input[lexer->position]) || lexer->input[lexer->position] == '.')) {
            lexer->position++;
            lexer->column++;
        }
        
        size_t length = lexer->position - start;
        char *value = malloc(length + 1);
        if (!value) return NULL;
        strncpy(value, lexer->input + start, length);
        value[length] = '\0';
        
        struct Token *token = create_token(TOKEN_NUMBER, value, start_line, start_column);
        free(value);
        return token;
    }
    
    if (current == '"') {
        size_t start = lexer->position;
        lexer->position++;
        lexer->column++;
        
        while (lexer->position < lexer->input_length && lexer->input[lexer->position] != '"') {
            if (lexer->input[lexer->position] == '\n') {
                lexer->