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

struct ParserNode {
    enum TokenType node_type;
    char *value;
    struct ParserNode *left;
    struct ParserNode *right;
};

struct ParseResult {
    int success;
    struct ParserNode *ast;
    char *error_message;
};

const char *keywords[] = {"if", "else", "while", "for", "return", "int", "char", "float", NULL};

int is_keyword(const char *str) {
    for (int i = 0; keywords[i] != NULL; i++) {
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

void free_token(struct Token *token) {
    if (token != NULL && token->value != NULL) {
        free(token->value);
    }
}

void free_ast(struct ParserNode *node) {
    if (node == NULL) return;
    free_ast(node->left);
    free_ast(node->right);
    if (node->value != NULL) {
        free(node->value);
    }
    free(node);
}

int get_next_char(struct LexerState *lexer) {
    if (lexer->position >= lexer->input_length) {
        return EOF;
    }
    return lexer->input[lexer->position];
}

void consume_char(struct LexerState *lexer) {
    if (lexer->position < lexer->input_length) {
        if (lexer->input[lexer->position] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
    }
}

struct Token *next_token(struct LexerState *lexer) {
    struct Token *token = malloc(sizeof(struct Token));
    if (token == NULL) return NULL;
    token->value = NULL;
    token->line = lexer->line;
    token->column = lexer->column;

    while (1) {
        int ch = get_next_char(lexer);
        if (ch == EOF) {
            token->type = TOKEN_EOF;
            return token;
        }

        if (isspace(ch)) {
            consume_char(lexer);
            continue;
        }

        if (isalpha(ch) || ch == '_') {
            size_t start = lexer->position;
            while (isalnum(get_next_char(lexer)) || get_next_char(lexer) == '_') {
                consume_char(lexer);
            }
            size_t length = lexer->position - start;
            token->value = malloc(length + 1);
            if (token->value == NULL) {
                free(token);
                return NULL;
            }
            strncpy(token->value, lexer->input + start, length);
            token->value[length] = '\0';
            token->type = is_keyword(token->value) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
            return token;
        }

        if (isdigit(ch)) {
            size_t start = lexer->position;
            while (isdigit(get_next_char(lexer))) {
                consume_char(lexer);
            }
            size_t length = lexer->position - start;
            token->value = malloc(length + 1);
            if (token->value == NULL) {
                free(token);
                return NULL;
            }
            strncpy(token->value, lexer->input + start, length);
            token->value[length] = '\0';
            token->type = TOKEN_NUMBER;
            return token;
        }

        if (ch == '"') {
            consume_char(lexer);
            size_t start = lexer->position;
            while (get_next_char(lexer) != '"' && get_next_char(lexer) != EOF) {
                consume_char(lexer);
            }
            if (get_next_char(lexer) == EOF) {
                token->type = TOKEN_UNKNOWN;
                return token;
            }
            size_t length = lexer->position - start;
            token->value = malloc(length + 1);
            if (token->value == NULL) {
                free(token);
                return NULL;
            }
            strncpy(token->value, lexer->input + start, length);
            token->value[length] = '\0';
            consume_char(lexer);
            token->type = TOKEN_STRING;
            return token;
        }

        if (strchr("+-*/=<>!&|", ch) != NULL) {
            token->value = malloc(3);
            if (token->