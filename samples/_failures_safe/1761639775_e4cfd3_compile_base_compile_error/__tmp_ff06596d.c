//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_UNKNOWN
};

struct Token {
    enum TokenType type;
    char value[64];
    int line;
    int column;
};

struct Lexer {
    const char *input;
    int position;
    int line;
    int column;
    int input_length;
};

struct Parser {
    struct Lexer *lexer;
    struct Token current_token;
    int error_count;
};

void lexer_init(struct Lexer *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->input_length = (int)strlen(input);
}

int lexer_is_at_end(struct Lexer *lexer) {
    if (lexer == NULL) return 1;
    return lexer->position >= lexer->input_length;
}

char lexer_peek(struct Lexer *lexer) {
    if (lexer == NULL || lexer_is_at_end(lexer)) {
        return '\0';
    }
    return lexer->input[lexer->position];
}

char lexer_advance(struct Lexer *lexer) {
    if (lexer == NULL || lexer_is_at_end(lexer)) {
        return '\0';
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

void lexer_skip_whitespace(struct Lexer *lexer) {
    if (lexer == NULL) return;
    while (!lexer_is_at_end(lexer) && isspace(lexer_peek(lexer))) {
        lexer_advance(lexer);
    }
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

void lexer_read_string(struct Lexer *lexer, struct Token *token) {
    if (lexer == NULL || token == NULL) return;
    token->type = TOKEN_STRING;
    int i = 0;
    char quote = lexer_advance(lexer);
    while (!lexer_is_at_end(lexer) && lexer_peek(lexer) != quote && i < 63) {
        token->value[i++] = lexer_advance(lexer);
    }
    if (lexer_peek(lexer) == quote) {
        lexer_advance(lexer);
    }
    token->value[i] = '\0';
}

void lexer_read_number(struct Lexer *lexer, struct Token *token) {
    if (lexer == NULL || token == NULL) return;
    token->type = TOKEN_NUMBER;
    int i = 0;
    while (!lexer_is_at_end(lexer) && isdigit(lexer_peek(lexer)) && i < 63) {
        token->value[i++] = lexer_advance(lexer);
    }
    token->value[i] = '\0';
}

void lexer_read_identifier(struct Lexer *lexer, struct Token *token) {
    if (lexer == NULL || token == NULL) return;
    token->type = TOKEN_IDENTIFIER;
    int i = 0;
    while (!lexer_is_at_end(lexer) && (isalnum(lexer_peek(lexer)) || lexer_peek(lexer) == '_') && i < 63) {
        token->value[i++] = lexer_advance(lexer);
    }
    token->value[i] = '\0';
}

void lexer_read_operator(struct Lexer *lexer, struct Token *token) {
    if (lexer == NULL || token == NULL) return;
    token->type = TOKEN_OPERATOR;
    int i = 0;
    while (!lexer_is_at_end(lexer) && is_operator_char(lexer_peek(lexer)) && i < 63) {
        token->value[i++] = lexer_advance(lexer);
    }
    token->value[i] = '\0';
}

struct Token lexer_next_token(struct Lexer *lexer) {
    struct Token token;
    if (lexer == NULL) {
        token.type = TOKEN_EOF;
        token.value[0] = '\0';
        token.line = 1;
        token.column = 1;
        return token;
    }
    
    token.line = lexer->line;
    token.column = lexer->column;
    token.value[0] = '\0';
    
    lexer_skip_whitespace(lexer);
    
    if (lexer_is_at_end(lexer)) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char c = lexer_peek(lexer);
    
    if (c == '"' || c == '\'') {
        lexer_read_string(lexer, &token);
    } else if (isdigit(c)) {
        lexer_read