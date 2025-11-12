//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
    TOKEN_KEYWORD,
    TOKEN_EOF,
    TOKEN_UNKNOWN
};

struct Token {
    enum TokenType type;
    char *value;
    int line;
    int column;
};

struct LexerState {
    const char *input;
    int position;
    int line;
    int column;
    int length;
};

struct ParserState {
    struct Token *tokens;
    int token_count;
    int current_token;
};

struct ASTNode {
    char *type;
    char *value;
    struct ASTNode **children;
    int child_count;
};

int is_keyword(const char *str) {
    const char *keywords[] = {"if", "else", "while", "for", "return", "int", "float", "char", "void"};
    int num_keywords = sizeof(keywords) / sizeof(keywords[0]);
    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

struct Token create_token(enum TokenType type, const char *value, int line, int column) {
    struct Token token;
    token.type = type;
    token.value = malloc(strlen(value) + 1);
    if (token.value) {
        strcpy(token.value, value);
    }
    token.line = line;
    token.column = column;
    return token;
}

void free_token(struct Token *token) {
    if (token && token->value) {
        free(token->value);
    }
}

struct LexerState create_lexer(const char *input) {
    struct LexerState lexer;
    lexer.input = input;
    lexer.position = 0;
    lexer.line = 1;
    lexer.column = 1;
    lexer.length = strlen(input);
    return lexer;
}

char lexer_peek(struct LexerState *lexer) {
    if (lexer->position >= lexer->length) {
        return '\0';
    }
    return lexer->input[lexer->position];
}

char lexer_advance(struct LexerState *lexer) {
    if (lexer->position >= lexer->length) {
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

void lexer_skip_whitespace(struct LexerState *lexer) {
    while (lexer->position < lexer->length && isspace(lexer_peek(lexer))) {
        lexer_advance(lexer);
    }
}

struct Token lexer_next_token(struct LexerState *lexer) {
    lexer_skip_whitespace(lexer);
    if (lexer->position >= lexer->length) {
        return create_token(TOKEN_EOF, "", lexer->line, lexer->column);
    }
    char c = lexer_peek(lexer);
    int start_line = lexer->line;
    int start_col = lexer->column;
    if (isalpha(c) || c == '_') {
        char buffer[256];
        int i = 0;
        while ((lexer->position < lexer->length) && (isalnum(c) || c == '_') && i < 255) {
            buffer[i++] = lexer_advance(lexer);
            c = lexer_peek(lexer);
        }
        buffer[i] = '\0';
        if (is_keyword(buffer)) {
            return create_token(TOKEN_KEYWORD, buffer, start_line, start_col);
        } else {
            return create_token(TOKEN_IDENTIFIER, buffer, start_line, start_col);
        }
    } else if (isdigit(c)) {
        char buffer[256];
        int i = 0;
        while ((lexer->position < lexer->length) && (isdigit(c) || c == '.') && i < 255) {
            buffer[i++] = lexer_advance(lexer);
            c = lexer_peek(lexer);
        }
        buffer[i] = '\0';
        return create_token(TOKEN_NUMBER, buffer, start_line, start_col);
    } else if (c == '"') {
        char buffer[256];
        int i = 0;
        lexer_advance(lexer);
        c = lexer_peek(lexer);
        while ((lexer->position < lexer->length) && (c != '"') && i < 255) {
            buffer[i++] = lexer_advance(lexer);
            c = lexer_peek(lexer);
        }
        if (c == '"') {
            lexer_advance(lexer);
        }
        buffer[i] = '\0';
        return create_token(TOKEN_STRING, buffer, start_line, start_col);
    } else if (strchr("+-*/=<>!&|", c)) {
        char buffer[3] = {0};
        buffer[0] = lexer_adv