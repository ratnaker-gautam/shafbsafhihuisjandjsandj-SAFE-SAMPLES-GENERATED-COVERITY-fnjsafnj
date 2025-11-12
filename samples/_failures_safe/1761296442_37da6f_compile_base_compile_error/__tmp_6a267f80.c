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

struct Lexer {
    const char *input;
    int position;
    int line;
    int column;
    int error_count;
};

struct Parser {
    struct Token *tokens;
    int token_count;
    int current_token;
    int error_count;
};

void lexer_init(struct Lexer *lexer, const char *input) {
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
    return strchr("(),;{}[]", c) != NULL;
}

struct Token lexer_next_token(struct Lexer *lexer) {
    struct Token token = {TOKEN_UNKNOWN, "", lexer->line, lexer->column};
    
    while (lexer->input[lexer->position] != '\0') {
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
            int len = 0;
            while ((isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') && len < MAX_TOKEN_LEN - 1) {
                token.value[len++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            token.value[len] = '\0';
            token.type = TOKEN_IDENTIFIER;
            return token;
        }
        
        if (isdigit(current)) {
            int len = 0;
            while (isdigit(lexer->input[lexer->position]) && len < MAX_TOKEN_LEN - 1) {
                token.value[len++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            token.value[len] = '\0';
            token.type = TOKEN_NUMBER;
            return token;
        }
        
        if (current == '"') {
            int len = 0;
            lexer->position++;
            lexer->column++;
            while (lexer->input[lexer->position] != '"' && lexer->input[lexer->position] != '\0' && len < MAX_TOKEN_LEN - 1) {
                token.value[len++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            if (lexer->input[lexer->position] == '"') {
                lexer->position++;
                lexer->column++;
            } else {
                lexer->error_count++;
            }
            token.value[len] = '\0';
            token.type = TOKEN_STRING;
            return token;
        }
        
        if (is_operator_char(current)) {
            int len = 0;
            while (is_operator_char(lexer->input[lexer->position]) && len < MAX_TOKEN_LEN - 1) {
                token.value[len++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            token.value[len] = '\0';
            token.type = TOKEN_OPERATOR;
            return token;
        }
        
        if (is_delimiter(current)) {
            token.value[0] = current;
            token.value[1] = '\0';
            token.type = TOKEN_DELIMITER;
            lexer->position++;
            lexer->column++;
            return token;
        }
        
        lexer->error_count++;
        token.value[0] = current;
        token.value[1] = '\0';
        lexer->position++;
        lexer->column++;
        return token;
    }
    
    token.type = TOKEN_EOF;
    strcpy(token.value, "EOF");
    return token;
}

int lexer_tokenize(struct Lexer *lexer, struct Token *tokens, int max_tokens) {
    int count = 0;
    
    while (count < max_tokens - 1) {
        struct Token token = lexer_next_token(lexer);
        tokens[count] = token;
        count++;
        
        if (token.type == TOKEN_EOF) {
            break;
        }
    }
    
    tokens[count].type = TOKEN_EOF;
    strcpy(tokens[count].value, "EOF");
    return count;
}

void parser_init(struct Parser *parser, struct Token *tokens, int token_count) {
    parser->tokens = tokens;
    parser->token_count = token_count;
    parser->current_token = 0;
    parser->error_count = 0;
}

struct Token parser_current_token(struct Parser *parser) {
    if (parser->current