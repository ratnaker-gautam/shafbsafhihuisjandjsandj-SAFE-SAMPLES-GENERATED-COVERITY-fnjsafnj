//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_ERROR
};

struct Token {
    enum TokenType type;
    char value[32];
    int line;
    int column;
};

struct Lexer {
    const char *input;
    int position;
    int line;
    int column;
};

void lexer_init(struct Lexer *lexer, const char *input) {
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
}

int lexer_peek(struct Lexer *lexer) {
    if (lexer->input == NULL) {
        return -1;
    }
    if (lexer->input[lexer->position] == '\0') {
        return -1;
    }
    return lexer->input[lexer->position];
}

int lexer_advance(struct Lexer *lexer) {
    int c = lexer_peek(lexer);
    if (c != -1) {
        lexer->position++;
        if (c == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
    }
    return c;
}

void lexer_skip_whitespace(struct Lexer *lexer) {
    while (1) {
        int c = lexer_peek(lexer);
        if (c == -1) break;
        if (!isspace(c)) break;
        lexer_advance(lexer);
    }
}

int is_operator_char(int c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

struct Token lexer_next_token(struct Lexer *lexer) {
    struct Token token = {TOKEN_ERROR, "", lexer->line, lexer->column};
    
    lexer_skip_whitespace(lexer);
    
    token.line = lexer->line;
    token.column = lexer->column;
    
    int c = lexer_peek(lexer);
    if (c == -1) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    if (isdigit(c)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (isdigit(c) && i < 31) {
            token.value[i++] = c;
            lexer_advance(lexer);
            c = lexer_peek(lexer);
        }
        token.value[i] = '\0';
    } else if (isalpha(c) || c == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while ((isalnum(c) || c == '_') && i < 31) {
            token.value[i++] = c;
            lexer_advance(lexer);
            c = lexer_peek(lexer);
        }
        token.value[i] = '\0';
    } else if (is_operator_char(c)) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (is_operator_char(c) && i < 31) {
            token.value[i++] = c;
            lexer_advance(lexer);
            c = lexer_peek(lexer);
        }
        token.value[i] = '\0';
    } else if (c == '"') {
        token.type = TOKEN_STRING;
        lexer_advance(lexer);
        int i = 0;
        c = lexer_peek(lexer);
        while (c != '"' && c != -1 && i < 31) {
            token.value[i++] = c;
            lexer_advance(lexer);
            c = lexer_peek(lexer);
        }
        token.value[i] = '\0';
        if (c == '"') {
            lexer_advance(lexer);
        } else {
            token.type = TOKEN_ERROR;
        }
    } else {
        token.type = TOKEN_ERROR;
        token.value[0] = c;
        token.value[1] = '\0';
        lexer_advance(lexer);
    }
    
    return token;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "ERROR"
    };
    
    printf("Line %d, Column %d: %s", token.line, token.column, type_names[token.type]);
    if (token.type != TOKEN_EOF) {
        printf(" '%s'", token.value);
    }
    printf("\n");
}

int main(void) {
    char input[1024];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len <= 1) {
        fprintf(stderr, "Empty input