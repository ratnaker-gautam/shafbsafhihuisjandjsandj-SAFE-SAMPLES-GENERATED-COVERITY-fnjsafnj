//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: parser_tokenizer
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
    while (lexer->input[lexer->position] != '\0') {
        char current = lexer->input[lexer->position];
        
        if (isspace(current)) {
            if (current == '\n') {
                lexer->line++;
                lexer->column = 1;
            } else {
                lexer->column++;
            }
            lexer->position++;
            continue;
        }
        
        token->line = lexer->line;
        token->column = lexer->column;
        
        if (isalpha(current) || current == '_') {
            int len = 0;
            while ((isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') && len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            token->value[len] = '\0';
            token->type = TOKEN_IDENTIFIER;
            return 1;
        }
        
        if (isdigit(current)) {
            int len = 0;
            while (isdigit(lexer->input[lexer->position]) && len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            token->value[len] = '\0';
            token->type = TOKEN_NUMBER;
            return 1;
        }
        
        if (current == '"') {
            int len = 0;
            lexer->position++;
            lexer->column++;
            while (lexer->input[lexer->position] != '"' && lexer->input[lexer->position] != '\0' && len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            if (lexer->input[lexer->position] == '"') {
                lexer->position++;
                lexer->column++;
            }
            token->value[len] = '\0';
            token->type = TOKEN_STRING;
            return 1;
        }
        
        if (is_operator_char(current)) {
            int len = 0;
            while (is_operator_char(lexer->input[lexer->position]) && len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            token->value[len] = '\0';
            token->type = TOKEN_OPERATOR;
            return 1;
        }
        
        lexer->position++;
        lexer->column++;
    }
    
    return 0;
}

void tokenize(Lexer* lexer) {
    Token token;
    while (get_next_token(lexer, &token) && lexer->token_count < MAX_TOKENS - 1) {
        lexer->tokens[lexer->token_count] = token;
        lexer->token_count++;
    }
    
    Token eof_token;
    eof_token.type = TOKEN_EOF;
    eof_token.value[0] = '\0';
    eof_token.line = lexer->line;
    eof_token.column = lexer->column;
    lexer->tokens[lexer->token_count] = eof_token;
    lexer->token_count++;
}

const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

void print_tokens(Lexer* lexer) {
    for (int i = 0; i < lexer->token_count; i++) {
        printf("Token %d: %s '%s' at line %d, column %d\n", 
               i, token_type_to_string(lexer->tokens[i].type), 
               lexer->tokens[i].value, lexer->tokens[i].line, lexer->tokens[i].column);
    }
}

int main