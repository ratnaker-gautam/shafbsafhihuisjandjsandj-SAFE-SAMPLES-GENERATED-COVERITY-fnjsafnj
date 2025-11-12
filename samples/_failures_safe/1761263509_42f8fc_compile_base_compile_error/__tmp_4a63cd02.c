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

struct ParseResult {
    int success;
    char error_message[100];
    int error_line;
    int error_column;
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

void init_parse_result(struct ParseResult *result) {
    result->success = 1;
    result->error_message[0] = '\0';
    result->error_line = 0;
    result->error_column = 0;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_delimiter_char(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ';' || c == ',';
}

int get_next_token(struct LexerState *lexer, struct Token *token) {
    if (lexer->input[lexer->position] == '\0') {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        token->line = lexer->line;
        token->column = lexer->column;
        return 1;
    }

    while (isspace(lexer->input[lexer->position])) {
        if (lexer->input[lexer->position] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
    }

    if (lexer->input[lexer->position] == '\0') {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        token->line = lexer->line;
        token->column = lexer->column;
        return 1;
    }

    token->line = lexer->line;
    token->column = lexer->column;

    if (isalpha(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') {
        token->type = TOKEN_IDENTIFIER;
        int i = 0;
        while ((isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') && i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token->value[i] = '\0';
    } else if (isdigit(lexer->input[lexer->position])) {
        token->type = TOKEN_NUMBER;
        int i = 0;
        while (isdigit(lexer->input[lexer->position]) && i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token->value[i] = '\0';
    } else if (lexer->input[lexer->position] == '"') {
        token->type = TOKEN_STRING;
        int i = 0;
        lexer->position++;
        lexer->column++;
        while (lexer->input[lexer->position] != '"' && lexer->input[lexer->position] != '\0' && i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token->value[i] = '\0';
        if (lexer->input[lexer->position] == '"') {
            lexer->position++;
            lexer->column++;
        }
    } else if (is_operator_char(lexer->input[lexer->position])) {
        token->type = TOKEN_OPERATOR;
        token->value[0] = lexer->input[lexer->position++];
        token->value[1] = '\0';
        lexer->column++;
    } else if (is_delimiter_char(lexer->input[lexer->position])) {
        token->type = TOKEN_DELIMITER;
        token->value[0] = lexer->input[lexer->position++];
        token