//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

typedef struct {
    const char* input;
    size_t pos;
    size_t len;
} Lexer;

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(Lexer* lexer) {
    while (lexer->pos < lexer->len && isspace(lexer->input[lexer->pos])) {
        lexer->pos++;
    }
}

Token get_next_token(Lexer* lexer) {
    Token token = {TOKEN_UNKNOWN, ""};
    skip_whitespace(lexer);
    
    if (lexer->pos >= lexer->len) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char current = lexer->input[lexer->pos];
    
    if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        size_t i = 0;
        while (lexer->pos < lexer->len && i < MAX_TOKEN_LEN - 1 && 
               (isdigit(lexer->input[lexer->pos]) || lexer->input[lexer->pos] == '.')) {
            token.value[i++] = lexer->input[lexer->pos++];
        }
        token.value[i] = '\0';
    } else if (isalpha(current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        size_t i = 0;
        while (lexer->pos < lexer->len && i < MAX_TOKEN_LEN - 1 && 
               (isalnum(lexer->input[lexer->pos]) || lexer->input[lexer->pos] == '_')) {
            token.value[i++] = lexer->input[lexer->pos++];
        }
        token.value[i] = '\0';
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        size_t i = 0;
        while (lexer->pos < lexer->len && i < MAX_TOKEN_LEN - 1 && 
               is_operator_char(lexer->input[lexer->pos])) {
            token.value[i++] = lexer->input[lexer->pos++];
        }
        token.value[i] = '\0';
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        size_t i = 0;
        lexer->pos++;
        while (lexer->pos < lexer->len && i < MAX_TOKEN_LEN - 1 && 
               lexer->input[lexer->pos] != '"') {
            token.value[i++] = lexer->input[lexer->pos++];
        }
        token.value[i] = '\0';
        if (lexer->pos < lexer->len && lexer->input[lexer->pos] == '"') {
            lexer->pos++;
        }
    } else {
        token.type = TOKEN_UNKNOWN;
        token.value[0] = lexer->input[lexer->pos++];
        token.value[1] = '\0';
    }
    
    return token;
}

int tokenize_input(const char* input, Token* tokens, size_t max_tokens) {
    if (input == NULL || tokens == NULL || max_tokens == 0) {
        return -1;
    }
    
    Lexer lexer = {input, 0, strlen(input)};
    size_t token_count = 0;
    
    while (token_count < max_tokens) {
        Token token = get_next_token(&lexer);
        if (token.type == TOKEN_EOF) {
            break;
        }
        tokens[token_count++] = token;
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
    }
    
    return (int)token_count;
}

void print_token_type(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_EOF: printf("EOF"); break;
        default: printf("UNKNOWN"); break;
    }
}

void print_tokens(const Token* tokens, int count) {
    if (tokens == NULL || count <= 0) {
        return;
    }
    
    printf("Tokenized output:\n");
    for (int i = 0; i < count; i++) {
        printf("Token %d: Type=", i + 1);
        print_token_type(tokens[i].type);
        printf(", Value='%s'\n", tokens[i].value);
    }
}

int main(void) {
    char input[1024];
    Token tokens[MAX_TOKENS];