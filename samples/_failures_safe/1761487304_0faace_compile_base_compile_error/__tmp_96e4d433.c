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
    uint32_t line;
    uint32_t column;
};

struct LexerState {
    const char *input;
    uint32_t position;
    uint32_t line;
    uint32_t column;
    uint32_t token_count;
};

struct ParserState {
    struct Token *tokens;
    uint32_t token_index;
    uint32_t token_count;
};

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int is_delimiter_char(char c) {
    return strchr("(){}[],;:", c) != NULL;
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

struct Token get_next_token(struct LexerState *lexer) {
    struct Token token = {TOKEN_UNKNOWN, "", lexer->line, lexer->column};
    if (lexer == NULL || lexer->input == NULL || lexer->token_count >= MAX_TOKENS) {
        token.type = TOKEN_EOF;
        return token;
    }

    skip_whitespace(lexer);
    if (lexer->input[lexer->position] == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }

    char current = lexer->input[lexer->position];
    uint32_t value_pos = 0;

    if (isalpha(current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        while ((isalnum(current) || current == '_') && value_pos < MAX_TOKEN_LEN - 1) {
            token.value[value_pos++] = current;
            lexer->position++;
            lexer->column++;
            current = lexer->input[lexer->position];
        }
    } else if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        while (isdigit(current) && value_pos < MAX_TOKEN_LEN - 1) {
            token.value[value_pos++] = current;
            lexer->position++;
            lexer->column++;
            current = lexer->input[lexer->position];
        }
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
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        while (is_operator_char(current) && value_pos < MAX_TOKEN_LEN - 1) {
            token.value[value_pos++] = current;
            lexer->position++;
            lexer->column++;
            current = lexer->input[lexer->position];
        }
    } else if (is_delimiter_char(current)) {
        token.type = TOKEN_DELIMITER;
        token.value[value_pos++] = current;
        lexer->position++;
        lexer->column++;
    } else {
        token.type = TOKEN_UNKNOWN;
        token.value[value_pos++] = current;
        lexer->position++;
        lexer->column++;
    }

    token.value[value_pos] = '\0';
    lexer->token_count++;
    return token;
}

int tokenize_input(struct LexerState *lexer, struct Token *tokens, uint32_t max_tokens) {
    if (lexer == NULL || tokens == NULL || max_tokens == 0) return 0;
    
    uint32_t count = 0;
    while (count < max_tokens) {
        struct Token token = get_next_token(lexer);
        tokens[count] = token;
        count++;
        if (token.type == TOKEN_EOF) break;