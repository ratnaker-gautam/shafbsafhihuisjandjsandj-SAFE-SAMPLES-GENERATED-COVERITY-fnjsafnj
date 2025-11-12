//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
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
    TOKEN_KEYWORD,
    TOKEN_UNKNOWN
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
    int line;
    int column;
};

struct LexerState {
    char *input;
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

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int is_delimiter_char(char c) {
    return strchr("(),;{}[]", c) != NULL;
}

int is_keyword(const char *str) {
    const char *keywords[] = {"if", "else", "while", "for", "return", "int", "float", "void"};
    int num_keywords = sizeof(keywords) / sizeof(keywords[0]);
    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void init_lexer(struct LexerState *lexer, char *input) {
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->token_count = 0;
}

void skip_whitespace(struct LexerState *lexer) {
    while (lexer->input[lexer->position] != '\0' && isspace(lexer->input[lexer->position])) {
        if (lexer->input[lexer->position] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
    }
}

int scan_number(struct LexerState *lexer, struct Token *token) {
    int start = lexer->position;
    int len = 0;
    
    while (lexer->input[lexer->position] != '\0' && isdigit(lexer->input[lexer->position])) {
        lexer->position++;
        lexer->column++;
        len++;
    }
    
    if (len > 0 && len < MAX_TOKEN_LEN) {
        strncpy(token->value, &lexer->input[start], len);
        token->value[len] = '\0';
        token->type = TOKEN_NUMBER;
        token->line = lexer->line;
        token->column = lexer->column - len;
        return 1;
    }
    return 0;
}

int scan_identifier(struct LexerState *lexer, struct Token *token) {
    int start = lexer->position;
    int len = 0;
    
    if (lexer->input[lexer->position] != '\0' && (isalpha(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_')) {
        len++;
        lexer->position++;
        lexer->column++;
        
        while (lexer->input[lexer->position] != '\0' && (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_')) {
            len++;
            lexer->position++;
            lexer->column++;
        }
    }
    
    if (len > 0 && len < MAX_TOKEN_LEN) {
        strncpy(token->value, &lexer->input[start], len);
        token->value[len] = '\0';
        token->type = is_keyword(token->value) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
        token->line = lexer->line;
        token->column = lexer->column - len;
        return 1;
    }
    return 0;
}

int scan_operator(struct LexerState *lexer, struct Token *token) {
    if (lexer->input[lexer->position] != '\0' && is_operator_char(lexer->input[lexer->position])) {
        token->value[0] = lexer->input[lexer->position];
        token->value[1] = '\0';
        token->type = TOKEN_OPERATOR;
        token->line = lexer->line;
        token->column = lexer->column;
        lexer->position++;
        lexer->column++;
        return 1;
    }
    return 0;
}

int scan_delimiter(struct LexerState *lexer, struct Token *token) {
    if (lexer->input[lexer->position] != '\0' && is_delimiter_char(lexer->input[lexer->position])) {
        token->value[0] = lexer->input[lexer->position];
        token->value[1] = '\0';
        token->type = TOKEN_DELIMITER;
        token->line = lexer->line;
        token->column = lexer->column;
        lexer->position++;