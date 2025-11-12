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

int next_char(struct LexerState *lexer) {
    if (lexer->input[lexer->position] == '\0') {
        return EOF;
    }
    char c = lexer->input[lexer->position++];
    if (c == '\n') {
        lexer->line++;
        lexer->column = 1;
    } else {
        lexer->column++;
    }
    return c;
}

void unget_char(struct LexerState *lexer) {
    if (lexer->position > 0) {
        lexer->position--;
        if (lexer->input[lexer->position] == '\n') {
            lexer->line--;
        } else {
            lexer->column--;
        }
    }
}

int scan_token(struct LexerState *lexer, struct Token *token) {
    int c;
    while ((c = next_char(lexer)) != EOF && isspace(c)) {
        continue;
    }
    
    if (c == EOF) {
        token->type = TOKEN_EOF;
        strcpy(token->value, "");
        token->line = lexer->line;
        token->column = lexer->column;
        return 1;
    }
    
    unget_char(lexer);
    token->line = lexer->line;
    token->column = lexer->column;
    
    if (isdigit(c)) {
        token->type = TOKEN_NUMBER;
        int i = 0;
        while ((c = next_char(lexer)) != EOF && isdigit(c)) {
            if (i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = c;
            }
        }
        unget_char(lexer);
        token->value[i] = '\0';
    } else if (isalpha(c) || c == '_') {
        token->type = TOKEN_IDENTIFIER;
        int i = 0;
        while ((c = next_char(lexer)) != EOF && (isalnum(c) || c == '_')) {
            if (i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = c;
            }
        }
        unget_char(lexer);
        token->value[i] = '\0';
    } else if (c == '"') {
        token->type = TOKEN_STRING;
        int i = 0;
        next_char(lexer);
        while ((c = next_char(lexer)) != EOF && c != '"') {
            if (i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = c;
            }
        }
        token->value[i] = '\0';
    } else if (is_operator_char(c)) {
        token->type = TOKEN_OPERATOR;
        int i = 0;
        while ((c = next_char(lexer)) != EOF && is_operator_char(c)) {
            if (i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = c;
            }
        }
        unget_char(lexer);
        token->value[i] = '\0';
    } else if (is_delimiter(c)) {
        token->type = TOKEN_DELIMITER;
        token->value[0] = next_char(lexer);
        token->value[1] = '\0';
    } else {
        token->type = TOKEN_UNKNOWN;
        token->value[0] = next_char(lexer);
        token->value[1] = '\0';
    }
    
    return 1;
}

int tokenize(struct LexerState *lexer, struct Token *tokens, int max_tokens) {
    int count = 0;
    struct Token token;
    
    while (count < max_tokens) {
        if (!scan_token(lexer, &token)) {
            break;
        }
        if (token.type == TOKEN_EOF) {
            tokens[count] = token;