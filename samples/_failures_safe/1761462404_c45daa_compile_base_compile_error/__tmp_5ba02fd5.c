//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_ASSIGN,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
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

void init_lexer(struct Lexer *lexer, const char *input) {
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
}

char current_char(struct Lexer *lexer) {
    if (lexer->input == NULL) return '\0';
    if (lexer->position < 0) return '\0';
    size_t len = strlen(lexer->input);
    if (lexer->position >= (int)len) return '\0';
    return lexer->input[lexer->position];
}

void advance(struct Lexer *lexer) {
    if (current_char(lexer) == '\n') {
        lexer->line++;
        lexer->column = 1;
    } else {
        lexer->column++;
    }
    lexer->position++;
}

void skip_whitespace(struct Lexer *lexer) {
    while (current_char(lexer) != '\0' && isspace(current_char(lexer))) {
        advance(lexer);
    }
}

struct Token next_token(struct Lexer *lexer) {
    struct Token token = {TOKEN_ERROR, "", lexer->line, lexer->column};
    
    skip_whitespace(lexer);
    
    char c = current_char(lexer);
    if (c == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    switch (c) {
        case '+':
            token.type = TOKEN_PLUS;
            token.value[0] = c;
            token.value[1] = '\0';
            advance(lexer);
            break;
        case '-':
            token.type = TOKEN_MINUS;
            token.value[0] = c;
            token.value[1] = '\0';
            advance(lexer);
            break;
        case '*':
            token.type = TOKEN_MULTIPLY;
            token.value[0] = c;
            token.value[1] = '\0';
            advance(lexer);
            break;
        case '/':
            token.type = TOKEN_DIVIDE;
            token.value[0] = c;
            token.value[1] = '\0';
            advance(lexer);
            break;
        case '=':
            token.type = TOKEN_ASSIGN;
            token.value[0] = c;
            token.value[1] = '\0';
            advance(lexer);
            break;
        case '(':
            token.type = TOKEN_LPAREN;
            token.value[0] = c;
            token.value[1] = '\0';
            advance(lexer);
            break;
        case ')':
            token.type = TOKEN_RPAREN;
            token.value[0] = c;
            token.value[1] = '\0';
            advance(lexer);
            break;
        default:
            if (isdigit(c)) {
                int i = 0;
                while (current_char(lexer) != '\0' && isdigit(current_char(lexer))) {
                    if (i < 31) {
                        token.value[i++] = current_char(lexer);
                    }
                    advance(lexer);
                }
                token.value[i] = '\0';
                token.type = TOKEN_NUMBER;
            } else if (isalpha(c)) {
                int i = 0;
                while (current_char(lexer) != '\0' && (isalnum(current_char(lexer)) || current_char(lexer) == '_')) {
                    if (i < 31) {
                        token.value[i++] = current_char(lexer);
                    }
                    advance(lexer);
                }
                token.value[i] = '\0';
                token.type = TOKEN_IDENTIFIER;
            } else {
                token.value[0] = c;
                token.value[1] = '\0';
                advance(lexer);
            }
            break;
    }
    
    return token;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "PLUS", "MINUS", "MULTIPLY", 
        "DIVIDE", "ASSIGN", "LPAREN", "RPAREN", "EOF", "ERROR"
    };
    
    printf("Line %d, Column %d: %s", token.line, token.column, type_names[token.type]);
    if (token.type == TOKEN_NUMBER || token.type == TOKEN_IDENTIFIER || token.type == TOKEN_ERROR) {
        printf(" ('%s')", token.value);
    }
    printf("\n");
}

int main(void) {
    char input[1024];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1