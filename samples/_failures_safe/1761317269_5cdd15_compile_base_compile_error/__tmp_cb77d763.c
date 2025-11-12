//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64
#define MAX_INPUT_LEN 1024

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_UNKNOWN
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
    int line;
    int column;
};

struct LexerState {
    const char *input;
    int position;
    int line;
    int column;
    int token_count;
};

struct ParserState {
    struct Token *tokens;
    int current_token;
    int token_count;
};

void init_lexer(struct LexerState *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->token_count = 0;
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int is_delimiter(char c) {
    return strchr("(),;{}[]", c) != NULL;
}

void skip_whitespace(struct LexerState *lexer) {
    if (lexer == NULL || lexer->input == NULL) return;
    while (lexer->input[lexer->position] != '\0') {
        char c = lexer->input[lexer->position];
        if (c == ' ' || c == '\t') {
            lexer->position++;
            lexer->column++;
        } else if (c == '\n') {
            lexer->position++;
            lexer->line++;
            lexer->column = 1;
        } else {
            break;
        }
    }
}

int get_next_token(struct LexerState *lexer, struct Token *token) {
    if (lexer == NULL || token == NULL || lexer->input == NULL) return 0;
    
    skip_whitespace(lexer);
    
    if (lexer->input[lexer->position] == '\0') {
        token->type = TOKEN_EOF;
        strncpy(token->value, "", MAX_TOKEN_LEN - 1);
        token->value[MAX_TOKEN_LEN - 1] = '\0';
        token->line = lexer->line;
        token->column = lexer->column;
        return 1;
    }
    
    char current = lexer->input[lexer->position];
    int start_col = lexer->column;
    
    if (isdigit(current)) {
        int len = 0;
        while (isdigit(lexer->input[lexer->position]) && len < MAX_TOKEN_LEN - 1) {
            token->value[len++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token->value[len] = '\0';
        token->type = TOKEN_NUMBER;
    } else if (isalpha(current) || current == '_') {
        int len = 0;
        while ((isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') && len < MAX_TOKEN_LEN - 1) {
            token->value[len++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token->value[len] = '\0';
        token->type = TOKEN_IDENTIFIER;
    } else if (current == '"') {
        int len = 0;
        lexer->position++;
        lexer->column++;
        while (lexer->input[lexer->position] != '"' && lexer->input[lexer->position] != '\0' && len < MAX_TOKEN_LEN - 1) {
            token->value[len++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        if (lexer->input[lexer->position] == '"') {
            lexer->position++;
            lexer->column++;
        }
        token->value[len] = '\0';
        token->type = TOKEN_STRING;
    } else if (is_operator_char(current)) {
        int len = 0;
        while (is_operator_char(lexer->input[lexer->position]) && len < MAX_TOKEN_LEN - 1) {
            token->value[len++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token->value[len] = '\0';
        token->type = TOKEN_OPERATOR;
    } else if (is_delimiter(current)) {
        token->value[0] = lexer->input[lexer->position++];
        token->value[1] = '\0';
        lexer->column++;
        token->type = TOKEN_DELIMITER;
    } else {
        token->value[0] = lexer->input[lexer->position++];
        token->value[1] = '\0';
        lexer->column++;
        token->type = TOKEN_UNKNOWN;
    }
    
    token->line = lexer->line;
    token->column = start_col;
    return 1;
}

void