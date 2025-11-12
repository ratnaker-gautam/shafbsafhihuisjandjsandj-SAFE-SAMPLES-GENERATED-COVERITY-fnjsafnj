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
    int length;
};

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

void init_lexer(struct Lexer *lexer, const char *input) {
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->length = (int)strlen(input);
}

struct Token get_next_token(struct Lexer *lexer) {
    struct Token token = {TOKEN_ERROR, "", lexer->line, lexer->column};
    
    if (lexer->position >= lexer->length) {
        token.type = TOKEN_EOF;
        strcpy(token.value, "");
        return token;
    }
    
    char current = lexer->input[lexer->position];
    
    while (isspace((unsigned char)current)) {
        if (current == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
        if (lexer->position >= lexer->length) {
            token.type = TOKEN_EOF;
            strcpy(token.value, "");
            return token;
        }
        current = lexer->input[lexer->position];
    }
    
    token.line = lexer->line;
    token.column = lexer->column;
    
    if (isdigit((unsigned char)current)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (lexer->position < lexer->length && 
               i < 31 && 
               isdigit((unsigned char)lexer->input[lexer->position])) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
    } else if (isalpha((unsigned char)current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (lexer->position < lexer->length && 
               i < 31 && 
               (isalnum((unsigned char)lexer->input[lexer->position]) || 
                lexer->input[lexer->position] == '_')) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        int i = 0;
        lexer->position++;
        lexer->column++;
        while (lexer->position < lexer->length && 
               i < 31 && 
               lexer->input[lexer->position] != '"') {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
        if (lexer->position < lexer->length && lexer->input[lexer->position] == '"') {
            lexer->position++;
            lexer->column++;
        }
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (lexer->position < lexer->length && 
               i < 31 && 
               is_operator_char(lexer->input[lexer->position])) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
    } else {
        token.type = TOKEN_ERROR;
        token.value[0] = current;
        token.value[1] = '\0';
        lexer->position++;
        lexer->column++;
    }
    
    return token;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "ERROR"
    };
    
    printf("Line %d, Column %d: %s", token.line, token.column, type_names[token.type]);
    if (token.type != TOKEN_EOF) {
        printf(" ('%s')", token.value);
    }
    printf("\n");
}

int main(void) {
    char input[1024];
    
    printf("Enter text to tokenize (max 1023 characters):\n");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len <= 1) {
        printf("No input provided\n");
        return 1;
    }
    
    struct Lexer lexer;
    init_lexer(&lexer