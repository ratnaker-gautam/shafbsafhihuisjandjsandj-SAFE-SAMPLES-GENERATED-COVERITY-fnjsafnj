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
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->token_count = 0;
}

void init_parser(struct ParserState *parser, struct Token *tokens, int count) {
    parser->tokens = tokens;
    parser->current_token = 0;
    parser->token_count = count;
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int is_delimiter(char c) {
    return strchr("(),;{}[]", c) != NULL;
}

struct Token create_token(enum TokenType type, const char *value, int line, int column) {
    struct Token token;
    token.type = type;
    strncpy(token.value, value, MAX_TOKEN_LEN - 1);
    token.value[MAX_TOKEN_LEN - 1] = '\0';
    token.line = line;
    token.column = column;
    return token;
}

int lex(struct LexerState *lexer, struct Token *tokens) {
    int token_index = 0;
    int input_len = strlen(lexer->input);
    
    while (lexer->position < input_len && token_index < MAX_TOKENS - 1) {
        char current = lexer->input[lexer->position];
        
        if (isspace(current)) {
            if (current == '\n') {
                lexer->line++;
                lexer->column = 1;
            } else {
                lexer->column++;
            }
            lexer->position++;
            continue;
        }
        
        if (isalpha(current) || current == '_') {
            char buffer[MAX_TOKEN_LEN] = {0};
            int buf_pos = 0;
            int start_col = lexer->column;
            
            while ((isalnum(current) || current == '_') && buf_pos < MAX_TOKEN_LEN - 1) {
                buffer[buf_pos++] = current;
                lexer->position++;
                lexer->column++;
                current = lexer->position < input_len ? lexer->input[lexer->position] : '\0';
            }
            buffer[buf_pos] = '\0';
            tokens[token_index++] = create_token(TOKEN_IDENTIFIER, buffer, lexer->line, start_col);
            continue;
        }
        
        if (isdigit(current)) {
            char buffer[MAX_TOKEN_LEN] = {0};
            int buf_pos = 0;
            int start_col = lexer->column;
            
            while (isdigit(current) && buf_pos < MAX_TOKEN_LEN - 1) {
                buffer[buf_pos++] = current;
                lexer->position++;
                lexer->column++;
                current = lexer->position < input_len ? lexer->input[lexer->position] : '\0';
            }
            buffer[buf_pos] = '\0';
            tokens[token_index++] = create_token(TOKEN_NUMBER, buffer, lexer->line, start_col);
            continue;
        }
        
        if (current == '"') {
            char buffer[MAX_TOKEN_LEN] = {0};
            int buf_pos = 0;
            int start_col = lexer->column;
            
            lexer->position++;
            lexer->column++;
            current = lexer->position < input_len ? lexer->input[lexer->position] : '\0';
            
            while (current != '"' && current != '\0' && buf_pos < MAX_TOKEN_LEN - 1) {
                buffer[buf_pos++] = current;
                lexer->position++;
                lexer->column++;
                current = lexer->position < input_len ? lexer->input[lexer->position] : '\0';
            }
            
            if (current == '"') {
                lexer->position++;
                lexer->column++;
            }
            buffer[buf_pos] = '\0';
            tokens[token_index++] = create_token(TOKEN_STRING, buffer, lexer->line, start_col);
            continue;
        }
        
        if (is_operator_char(current)) {
            char buffer[3] = {0};
            int buf_pos = 0;
            int start_col = lexer->column;
            
            while (is_operator_char(current) && buf_pos < 2) {
                buffer[buf_pos++] = current;
                lexer->position++;
                lexer->column++;
                current = lexer->position < input_len ? lexer->input[lexer->position] : '\0';
            }
            buffer[buf_pos] = '\0';
            tokens[token_index++] = create_token(TOKEN_OPERATOR,