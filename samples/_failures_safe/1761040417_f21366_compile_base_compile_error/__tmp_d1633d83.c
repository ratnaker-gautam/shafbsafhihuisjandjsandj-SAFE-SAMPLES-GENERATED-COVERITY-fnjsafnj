//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
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

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int is_delimiter(char c) {
    return strchr("(),;{}[]", c) != NULL;
}

struct Token next_token(struct LexerState *lexer) {
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
            token.value[len++] = lexer->input[lexer->position++];
            lexer->column++;
            
            while (lexer->input[lexer->position] != '"' && lexer->input[lexer->position] != '\0' && len < MAX_TOKEN_LEN - 1) {
                token.value[len++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            
            if (lexer->input[lexer->position] == '"') {
                token.value[len++] = lexer->input[lexer->position++];
                lexer->column++;
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
            token.value[0] = lexer->input[lexer->position++];
            token.value[1] = '\0';
            lexer->column++;
            token.type = TOKEN_DELIMITER;
            return token;
        }
        
        token.value[0] = lexer->input[lexer->position++];
        token.value[1] = '\0';
        lexer->column++;
        token.type = TOKEN_UNKNOWN;
        return token;
    }
    
    token.type = TOKEN_EOF;
    strcpy(token.value, "EOF");
    return token;
}

int tokenize(struct LexerState *lexer, struct Token *tokens, int max_tokens) {
    int count = 0;
    
    while (count < max_tokens) {
        struct Token token = next_token(lexer);
        tokens[count] = token;
        count++;
        
        if (token.type == TOKEN_EOF) {
            break;
        }
    }
    
    return count;
}

void init_parser(struct ParserState *parser, struct Token *tokens, int token_count) {
    parser->tokens = tokens;
    parser->current_token = 0;
    parser->token_count = token_count;
}

struct Token current_token(struct ParserState *parser) {
    if (parser->current_token < parser->token_count) {
        return parser->tokens[parser->current_token