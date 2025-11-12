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

void init_lexer(struct LexerState *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->token_count = 0;
}

void skip_whitespace(struct LexerState *lexer) {
    if (lexer == NULL || lexer->input == NULL) return;
    while (lexer->input[lexer->position] != '\0' && 
           isspace((unsigned char)lexer->input[lexer->position])) {
        if (lexer->input[lexer->position] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
    }
}

struct Token get_next_token(struct LexerState *lexer) {
    struct Token token = {TOKEN_UNKNOWN, "", lexer->line, lexer->column};
    if (lexer == NULL || lexer->input == NULL) return token;
    
    skip_whitespace(lexer);
    
    if (lexer->input[lexer->position] == '\0') {
        token.type = TOKEN_EOF;
        strncpy(token.value, "EOF", MAX_TOKEN_LEN - 1);
        token.value[MAX_TOKEN_LEN - 1] = '\0';
        return token;
    }
    
    char current = lexer->input[lexer->position];
    int value_pos = 0;
    
    if (isalpha((unsigned char)current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        while ((isalnum((unsigned char)current) || current == '_') && value_pos < MAX_TOKEN_LEN - 1) {
            token.value[value_pos++] = current;
            lexer->position++;
            lexer->column++;
            current = lexer->input[lexer->position];
        }
        token.value[value_pos] = '\0';
    } else if (isdigit((unsigned char)current)) {
        token.type = TOKEN_NUMBER;
        while (isdigit((unsigned char)current) && value_pos < MAX_TOKEN_LEN - 1) {
            token.value[value_pos++] = current;
            lexer->position++;
            lexer->column++;
            current = lexer->input[lexer->position];
        }
        token.value[value_pos] = '\0';
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        token.value[value_pos++] = current;
        lexer->position++;
        lexer->column++;
        current = lexer->input[lexer->position];
        
        while (current != '"' && current != '\0' && value_pos < MAX_TOKEN_LEN - 1) {
            token.value[value_pos++] = current;
            lexer->position++;
            lexer->column++;
            current = lexer->input[lexer->position];
        }
        
        if (current == '"') {
            token.value[value_pos++] = current;
            lexer->position++;
            lexer->column++;
        }
        token.value[value_pos] = '\0';
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        while (is_operator_char(current) && value_pos < MAX_TOKEN_LEN - 1) {
            token.value[value_pos++] = current;
            lexer->position++;
            lexer->column++;
            current = lexer->input[lexer->position];
        }
        token.value[value_pos] = '\0';
    } else if (is_delimiter_char(current)) {
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
    }
    
    return token;
}

int tokenize_input(struct LexerState *lexer, struct Token *tokens, int max_tokens) {
    if (lexer == NULL || tokens == NULL || max_tokens <= 0) return 0;
    int count = 0;
    
    while (count <