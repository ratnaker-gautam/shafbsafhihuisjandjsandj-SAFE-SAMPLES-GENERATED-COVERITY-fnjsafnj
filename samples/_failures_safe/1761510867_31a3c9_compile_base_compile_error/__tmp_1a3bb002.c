//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
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

struct Lexer {
    const char *input;
    int position;
    int line;
    int column;
    int token_count;
};

struct Parser {
    struct Token *tokens;
    int current_token;
    int token_count;
};

void init_lexer(struct Lexer *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->token_count = 0;
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(struct Lexer *lexer) {
    if (lexer == NULL || lexer->input == NULL) return;
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

int read_token(struct Lexer *lexer, struct Token *token) {
    if (lexer == NULL || token == NULL || lexer->input == NULL) return 0;
    
    skip_whitespace(lexer);
    
    if (lexer->input[lexer->position] == '\0') {
        token->type = TOKEN_EOF;
        strcpy(token->value, "");
        token->line = lexer->line;
        token->column = lexer->column;
        return 1;
    }
    
    char current = lexer->input[lexer->position];
    int start_pos = lexer->position;
    int value_pos = 0;
    
    if (isdigit(current)) {
        token->type = TOKEN_NUMBER;
        while (lexer->input[lexer->position] != '\0' && isdigit(lexer->input[lexer->position])) {
            if (value_pos < MAX_TOKEN_LEN - 1) {
                token->value[value_pos++] = lexer->input[lexer->position];
            }
            lexer->position++;
            lexer->column++;
        }
    } else if (isalpha(current) || current == '_') {
        token->type = TOKEN_IDENTIFIER;
        while (lexer->input[lexer->position] != '\0' && (isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_')) {
            if (value_pos < MAX_TOKEN_LEN - 1) {
                token->value[value_pos++] = lexer->input[lexer->position];
            }
            lexer->position++;
            lexer->column++;
        }
    } else if (is_operator(current)) {
        token->type = TOKEN_OPERATOR;
        if (value_pos < MAX_TOKEN_LEN - 1) {
            token->value[value_pos++] = lexer->input[lexer->position];
        }
        lexer->position++;
        lexer->column++;
    } else if (current == '"') {
        token->type = TOKEN_STRING;
        lexer->position++;
        lexer->column++;
        while (lexer->input[lexer->position] != '\0' && lexer->input[lexer->position] != '"') {
            if (value_pos < MAX_TOKEN_LEN - 1) {
                token->value[value_pos++] = lexer->input[lexer->position];
            }
            lexer->position++;
            lexer->column++;
        }
        if (lexer->input[lexer->position] == '"') {
            lexer->position++;
            lexer->column++;
        }
    } else {
        token->type = TOKEN_UNKNOWN;
        if (value_pos < MAX_TOKEN_LEN - 1) {
            token->value[value_pos++] = lexer->input[lexer->position];
        }
        lexer->position++;
        lexer->column++;
    }
    
    token->value[value_pos] = '\0';
    token->line = lexer->line;
    token->column = lexer->column - (lexer->position - start_pos);
    
    return 1;
}

int tokenize(struct Lexer *lexer, struct Token *tokens, int max_tokens) {
    if (lexer == NULL || tokens == NULL || max_tokens <= 0) return 0;
    
    int count = 0;
    struct Token token;
    
    while (count < max_tokens) {
        if (!read_token(lexer, &token)) {
            return count;
        }
        
        if (token.type == TOKEN_EOF) {
            tokens[count] =