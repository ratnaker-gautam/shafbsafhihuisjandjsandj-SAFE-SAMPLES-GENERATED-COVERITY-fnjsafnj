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
    char value[64];
    int line;
    int column;
};

struct Lexer {
    const char *input;
    int position;
    int line;
    int column;
    int length;
};

struct Parser {
    struct Lexer *lexer;
    struct Token current_token;
    int error_count;
};

struct ASTNode {
    enum { NODE_IDENTIFIER, NODE_NUMBER, NODE_STRING, NODE_BINARY_OP } type;
    union {
        char identifier[64];
        double number;
        char string[64];
        struct {
            struct ASTNode *left;
            struct ASTNode *right;
            char operator[8];
        } binary_op;
    } value;
};

void lexer_init(struct Lexer *lexer, const char *input) {
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->length = (int)strlen(input);
}

int lexer_is_eof(struct Lexer *lexer) {
    return lexer->position >= lexer->length;
}

char lexer_peek(struct Lexer *lexer) {
    if (lexer_is_eof(lexer)) return '\0';
    return lexer->input[lexer->position];
}

char lexer_advance(struct Lexer *lexer) {
    if (lexer_is_eof(lexer)) return '\0';
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
    while (!lexer_is_eof(lexer) && isspace(lexer_peek(lexer))) {
        lexer_advance(lexer);
    }
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int is_delimiter(char c) {
    return strchr("(),;{}[]", c) != NULL;
}

int is_keyword(const char *str) {
    const char *keywords[] = {"if", "else", "while", "for", "return", "int", "float", "char", "void"};
    for (size_t i = 0; i < sizeof(keywords)/sizeof(keywords[0]); i++) {
        if (strcmp(str, keywords[i]) == 0) return 1;
    }
    return 0;
}

struct Token lexer_next_token(struct Lexer *lexer) {
    struct Token token = {TOKEN_UNKNOWN, "", lexer->line, lexer->column};
    lexer_skip_whitespace(lexer);
    if (lexer_is_eof(lexer)) {
        token.type = TOKEN_EOF;
        return token;
    }
    char c = lexer_peek(lexer);
    if (isalpha(c) || c == '_') {
        int i = 0;
        while (!lexer_is_eof(lexer) && (isalnum(lexer_peek(lexer)) || lexer_peek(lexer) == '_')) {
            if (i < 63) token.value[i++] = lexer_advance(lexer);
            else lexer_advance(lexer);
        }
        token.value[i] = '\0';
        token.type = is_keyword(token.value) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
    } else if (isdigit(c)) {
        int i = 0;
        while (!lexer_is_eof(lexer) && (isdigit(lexer_peek(lexer)) || lexer_peek(lexer) == '.')) {
            if (i < 63) token.value[i++] = lexer_advance(lexer);
            else lexer_advance(lexer);
        }
        token.value[i] = '\0';
        token.type = TOKEN_NUMBER;
    } else if (c == '"') {
        lexer_advance(lexer);
        int i = 0;
        while (!lexer_is_eof(lexer) && lexer_peek(lexer) != '"') {
            if (i < 63) token.value[i++] = lexer_advance(lexer);
            else lexer_advance(lexer);
        }
        if (lexer_peek(lexer) == '"') lexer_advance(lexer);
        token.value[i] = '\0';
        token.type = TOKEN_STRING;
    } else if (is_operator_char(c)) {
        int i = 0;
        while (!lexer_is_eof(lexer) && is_operator_char(lexer_peek(lexer)) && i < 7) {
            token.value[i++] = lexer_advance(lexer);
        }
        token.value[i] = '\0