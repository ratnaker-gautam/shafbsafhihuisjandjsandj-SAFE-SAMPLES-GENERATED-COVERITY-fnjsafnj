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
    TOKEN_PUNCTUATION,
    TOKEN_KEYWORD,
    TOKEN_EOF,
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char value[32];
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
    struct Token current_token;
    struct LexerState *lexer;
};

struct ASTNode {
    enum { NODE_IDENTIFIER, NODE_NUMBER, NODE_STRING, NODE_BINARY_OP } type;
    union {
        char identifier[32];
        double number;
        char string[32];
        struct {
            struct ASTNode *left;
            struct ASTNode *right;
            char operator[4];
        } binary_op;
    } value;
};

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int is_punctuation_char(char c) {
    return strchr("(),;{}[]", c) != NULL;
}

int is_keyword(const char *str) {
    const char *keywords[] = {"if", "else", "while", "for", "return", "int", "float", "char", "void"};
    for (size_t i = 0; i < sizeof(keywords)/sizeof(keywords[0]); i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void init_lexer(struct LexerState *lexer, const char *input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->length = (int)strlen(input);
}

char peek_char(struct LexerState *lexer) {
    if (lexer == NULL || lexer->position >= lexer->length) return '\0';
    return lexer->input[lexer->position];
}

char next_char(struct LexerState *lexer) {
    if (lexer == NULL || lexer->position >= lexer->length) return '\0';
    char c = lexer->input[lexer->position++];
    if (c == '\n') {
        lexer->line++;
        lexer->column = 1;
    } else {
        lexer->column++;
    }
    return c;
}

void skip_whitespace(struct LexerState *lexer) {
    if (lexer == NULL) return;
    while (lexer->position < lexer->length && isspace(peek_char(lexer))) {
        next_char(lexer);
    }
}

struct Token read_string(struct LexerState *lexer) {
    struct Token token = {TOKEN_INVALID, "", lexer->line, lexer->column};
    if (lexer == NULL) return token;
    
    char c = next_char(lexer);
    int i = 0;
    
    while (c != '"' && c != '\0' && i < 31) {
        token.value[i++] = c;
        c = next_char(lexer);
    }
    
    if (c == '"') {
        token.type = TOKEN_STRING;
    }
    token.value[i] = '\0';
    return token;
}

struct Token read_number(struct LexerState *lexer) {
    struct Token token = {TOKEN_NUMBER, "", lexer->line, lexer->column};
    if (lexer == NULL) return token;
    
    int i = 0;
    char c = peek_char(lexer);
    
    while (lexer->position < lexer->length && i < 31 && 
           (isdigit(c) || c == '.')) {
        token.value[i++] = next_char(lexer);
        c = peek_char(lexer);
    }
    token.value[i] = '\0';
    return token;
}

struct Token read_identifier(struct LexerState *lexer) {
    struct Token token = {TOKEN_IDENTIFIER, "", lexer->line, lexer->column};
    if (lexer == NULL) return token;
    
    int i = 0;
    char c = peek_char(lexer);
    
    while (lexer->position < lexer->length && i < 31 && 
           (isalnum(c) || c == '_')) {
        token.value[i++] = next_char(lexer);
        c = peek_char(lexer);
    }
    token.value[i] = '\0';
    
    if (is_keyword(token.value)) {
        token.type = TOKEN_KEYWORD;
    }
    return token;
}

struct Token read_operator(struct LexerState *lexer) {
    struct Token token = {TOKEN_OPERATOR, "", lexer->line, lexer->column};
    if (lexer == NULL) return token;
    
    int i = 0;
    char c = peek_char(lexer);
    
    while (lexer->position < lexer->length && i < 3 && is_operator_char(c)) {
        token.value[i++] = next_char(lex