//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

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

void init_lexer(struct LexerState *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->token_count = 0;
}

void init_parser(struct ParserState *parser, struct Token *tokens, int count) {
    if (parser == NULL || tokens == NULL || count < 0) return;
    parser->tokens = tokens;
    parser->current_token = 0;
    parser->token_count = count;
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int is_delimiter(char c) {
    return strchr("(){}[],;", c) != NULL;
}

struct Token next_token(struct LexerState *lexer) {
    struct Token token;
    token.type = TOKEN_UNKNOWN;
    token.line = lexer->line;
    token.column = lexer->column;
    memset(token.value, 0, MAX_TOKEN_LEN);
    
    if (lexer->input == NULL || lexer->input[lexer->position] == '\0') {
        token.type = TOKEN_EOF;
        strncpy(token.value, "EOF", MAX_TOKEN_LEN - 1);
        return token;
    }
    
    while (isspace((unsigned char)lexer->input[lexer->position])) {
        if (lexer->input[lexer->position] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
    }
    
    if (lexer->input[lexer->position] == '\0') {
        token.type = TOKEN_EOF;
        strncpy(token.value, "EOF", MAX_TOKEN_LEN - 1);
        return token;
    }
    
    char current = lexer->input[lexer->position];
    int value_pos = 0;
    
    if (isalpha((unsigned char)current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        while (isalnum((unsigned char)lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') {
            if (value_pos < MAX_TOKEN_LEN - 1) {
                token.value[value_pos++] = lexer->input[lexer->position];
            }
            lexer->position++;
            lexer->column++;
        }
    } else if (isdigit((unsigned char)current)) {
        token.type = TOKEN_NUMBER;
        while (isdigit((unsigned char)lexer->input[lexer->position])) {
            if (value_pos < MAX_TOKEN_LEN - 1) {
                token.value[value_pos++] = lexer->input[lexer->position];
            }
            lexer->position++;
            lexer->column++;
        }
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        lexer->position++;
        lexer->column++;
        while (lexer->input[lexer->position] != '"' && lexer->input[lexer->position] != '\0') {
            if (value_pos < MAX_TOKEN_LEN - 1) {
                token.value[value_pos++] = lexer->input[lexer->position];
            }
            lexer->position++;
            lexer->column++;
        }
        if (lexer->input[lexer->position] == '"') {
            lexer->position++;
            lexer->column++;
        }
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        while (is_operator_char(lexer->input[lexer->position])) {
            if (value_pos < MAX_TOKEN_LEN - 1) {
                token.value[value_pos++] = lexer->input[lexer->position];
            }
            lexer->position++;
            lexer->column++;
        }
    } else if (is_delimiter(current)) {
        token.type = TOKEN_DELIMITER;
        token.value[0] = lexer->input[lexer->position];
        lexer->position++;
        lexer->column++;
    } else {
        token.type = TOKEN_UNKNOWN;
        token.value[0] = lexer->input[lexer->position];
        lexer->position++;
        lexer->column++;
    }
    
    token.value[value_pos] = '\0';
    return token;
}

int tokenize(struct LexerState *lexer, struct Token *tokens, int max