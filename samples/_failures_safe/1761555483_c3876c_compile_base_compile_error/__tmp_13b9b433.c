//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
    TOKEN_STRING,
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

struct TokenList {
    struct Token *tokens;
    int count;
    int capacity;
};

void init_lexer(struct Lexer *lexer, const char *input) {
    if (!lexer || !input) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->length = (int)strlen(input);
}

void init_token_list(struct TokenList *list) {
    if (!list) return;
    list->capacity = 16;
    list->count = 0;
    list->tokens = malloc(list->capacity * sizeof(struct Token));
    if (!list->tokens) exit(1);
}

void free_token_list(struct TokenList *list) {
    if (list && list->tokens) {
        free(list->tokens);
        list->tokens = NULL;
        list->count = 0;
        list->capacity = 0;
    }
}

void add_token(struct TokenList *list, struct Token token) {
    if (!list) return;
    if (list->count >= list->capacity) {
        size_t new_capacity = list->capacity * 2;
        if (new_capacity > 1024) new_capacity = 1024;
        struct Token *new_tokens = realloc(list->tokens, new_capacity * sizeof(struct Token));
        if (!new_tokens) exit(1);
        list->tokens = new_tokens;
        list->capacity = (int)new_capacity;
    }
    if (list->count < list->capacity) {
        list->tokens[list->count] = token;
        list->count++;
    }
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int is_delimiter_char(char c) {
    return strchr("(){}[],;:", c) != NULL;
}

void skip_whitespace(struct Lexer *lexer) {
    if (!lexer || !lexer->input) return;
    while (lexer->position < lexer->length && isspace((unsigned char)lexer->input[lexer->position])) {
        if (lexer->input[lexer->position] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
    }
}

struct Token next_token(struct Lexer *lexer) {
    struct Token token = {TOKEN_UNKNOWN, "", lexer->line, lexer->column};
    if (!lexer || !lexer->input) return token;
    
    skip_whitespace(lexer);
    
    if (lexer->position >= lexer->length) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char current = lexer->input[lexer->position];
    
    if (isalpha((unsigned char)current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (lexer->position < lexer->length && 
               (isalnum((unsigned char)lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') &&
               i < 63) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
    } else if (isdigit((unsigned char)current)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (lexer->position < lexer->length && 
               isdigit((unsigned char)lexer->input[lexer->position]) && i < 63) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
    } else if (current == '"') {
        token.type = TOKEN_STRING;
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
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (lexer->position < lexer->length && 
               is_operator_char(lexer->input[lexer->position