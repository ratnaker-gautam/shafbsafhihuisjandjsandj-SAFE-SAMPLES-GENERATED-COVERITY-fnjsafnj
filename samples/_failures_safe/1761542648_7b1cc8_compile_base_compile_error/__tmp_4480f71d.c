//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 1000
#define MAX_TOKEN_LEN 256

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
    int line;
    int column;
} Token;

typedef struct {
    const char* input;
    int position;
    int line;
    int column;
    Token tokens[MAX_TOKENS];
    int token_count;
} Lexer;

void init_lexer(Lexer* lexer, const char* input) {
    if (lexer == NULL || input == NULL) return;
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->token_count = 0;
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int get_next_token(Lexer* lexer, Token* token) {
    if (lexer == NULL || token == NULL || lexer->input == NULL) return 0;
    
    size_t input_len = strlen(lexer->input);
    if (lexer->position >= (int)input_len) {
        token->type = TOKEN_EOF;
        strncpy(token->value, "", MAX_TOKEN_LEN - 1);
        token->value[MAX_TOKEN_LEN - 1] = '\0';
        token->line = lexer->line;
        token->column = lexer->column;
        return 1;
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
        if (lexer->position >= (int)strlen(lexer->input)) {
            token->type = TOKEN_EOF;
            strncpy(token->value, "", MAX_TOKEN_LEN - 1);
            token->value[MAX_TOKEN_LEN - 1] = '\0';
            token->line = lexer->line;
            token->column = lexer->column;
            return 1;
        }
        current = lexer->input[lexer->position];
    }

    token->line = lexer->line;
    token->column = lexer->column;

    if (isalpha((unsigned char)current) || current == '_') {
        int i = 0;
        while ((isalnum((unsigned char)current) || current == '_') && i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = current;
            lexer->position++;
            lexer->column++;
            if (lexer->position >= (int)strlen(lexer->input)) break;
            current = lexer->input[lexer->position];
        }
        token->value[i] = '\0';
        token->type = TOKEN_IDENTIFIER;
        return 1;
    }

    if (isdigit((unsigned char)current)) {
        int i = 0;
        while (isdigit((unsigned char)current) && i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = current;
            lexer->position++;
            lexer->column++;
            if (lexer->position >= (int)strlen(lexer->input)) break;
            current = lexer->input[lexer->position];
        }
        token->value[i] = '\0';
        token->type = TOKEN_NUMBER;
        return 1;
    }

    if (current == '"') {
        int i = 0;
        lexer->position++;
        lexer->column++;
        if (lexer->position >= (int)strlen(lexer->input)) {
            token->value[0] = '\0';
            token->type = TOKEN_STRING;
            return 1;
        }
        current = lexer->input[lexer->position];
        
        while (current != '"' && i < MAX_TOKEN_LEN - 1) {
            if (lexer->position >= (int)strlen(lexer->input)) break;
            token->value[i++] = current;
            lexer->position++;
            lexer->column++;
            current = lexer->input[lexer->position];
        }
        
        if (current == '"') {
            lexer->position++;
            lexer->column++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_STRING;
        return 1;
    }

    if (is_operator_char(current)) {
        int i = 0;
        while (is_operator_char(current) && i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = current;
            lexer->position++;
            lexer->column++;
            if (lexer->position >= (int)strlen(lexer->input)) break;
            current = lexer->input[lexer->position];
        }
        token->value[i] = '\0';
        token->type = TOKEN_OPERATOR;
        return 1;
    }

    return 0;
}

void tokenize(Lexer* lexer) {