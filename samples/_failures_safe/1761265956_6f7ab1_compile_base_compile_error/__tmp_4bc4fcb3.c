//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

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
    char value[MAX_TOKEN_LEN];
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

struct ParserState {
    struct Token *tokens;
    int token_count;
    int current_token;
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

void skip_whitespace(struct LexerState *lexer) {
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

struct Token next_token(struct LexerState *lexer) {
    struct Token token = {TOKEN_UNKNOWN, "", lexer->line, lexer->column};
    
    if (lexer == NULL || lexer->input == NULL) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    skip_whitespace(lexer);
    
    if (lexer->input[lexer->position] == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char current = lexer->input[lexer->position];
    
    if (isalpha(current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while ((isalnum(current) || current == '_') && i < MAX_TOKEN_LEN - 1) {
            token.value[i++] = current;
            lexer->position++;
            lexer->column++;
            current = lexer->input[lexer->position];
        }
        token.value[i] = '\0';
    } else if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (isdigit(current) && i < MAX_TOKEN_LEN - 1) {
            token.value[i++] = current;
            lexer->position++;
            lexer->column++;
            current = lexer->input[lexer->position];
        }
        token.value[i] = '\0';
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        int i = 0;
        lexer->position++;
        lexer->column++;
        current = lexer->input[lexer->position];
        
        while (current != '"' && current != '\0' && i < MAX_TOKEN_LEN - 1) {
            token.value[i++] = current;
            lexer->position++;
            lexer->column++;
            current = lexer->input[lexer->position];
        }
        
        if (current == '"') {
            lexer->position++;
            lexer->column++;
        } else {
            lexer->error_count++;
        }
        token.value[i] = '\0';
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (is_operator_char(current) && i < MAX_TOKEN_LEN - 1) {
            token.value[i++] = current;
            lexer->position++;
            lexer->column++;
            current = lexer->input[lexer->position];
        }
        token.value[i] = '\0';
    } else if (is_delimiter(current)) {
        token.type = TOKEN_DELIMITER;
        token.value[0] = current;
        token.value[1] = '\0';
        lexer->position++;
        lexer->column++;
    } else {
        token.type = TOKEN_UNKNOWN;
        token.value[0] = current;
        token.value[1] = '\0';
        lexer->position++;
        lexer->column++;
        lexer->error_count++;
    }
    
    return token;
}

int tokenize(struct LexerState *lexer, struct Token *tokens, int max_tokens) {
    if (lexer == NULL || tokens == NULL || max_tokens <= 0) return 0;
    
    int count = 0;
    
    while (count < max_tokens) {
        struct Token token = next_token(lexer);
        tokens[count] = token;
        count++;
        
        if (token.type == TOKEN_EOF) {