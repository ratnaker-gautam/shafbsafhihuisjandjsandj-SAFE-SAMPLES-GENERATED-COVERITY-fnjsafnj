//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_LEN 128
#define MAX_TOKENS 256

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_PAREN,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

typedef struct {
    Token tokens[MAX_TOKENS];
    int count;
    int position;
} TokenStream;

void skip_whitespace(const char **input) {
    while (**input && isspace(**input)) {
        (*input)++;
    }
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

int parse_number(const char **input, Token *token) {
    char *end;
    double val = strtod(*input, &end);
    if (end == *input) return 0;
    
    int len = snprintf(token->value, MAX_TOKEN_LEN, "%.6g", val);
    if (len < 0 || len >= MAX_TOKEN_LEN) return 0;
    
    token->type = TOKEN_NUMBER;
    *input = end;
    return 1;
}

int parse_identifier(const char **input, Token *token) {
    int i = 0;
    while (**input && (isalnum(**input) || **input == '_')) {
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        token->value[i++] = *(*input)++;
    }
    token->value[i] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_operator(const char **input, Token *token) {
    if (!is_operator_char(**input)) return 0;
    
    token->value[0] = *(*input)++;
    token->value[1] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

int parse_paren(const char **input, Token *token) {
    if (**input != '(' && **input != ')') return 0;
    
    token->value[0] = *(*input)++;
    token->value[1] = '\0';
    token->type = TOKEN_PAREN;
    return 1;
}

int tokenize(const char *input, TokenStream *stream) {
    stream->count = 0;
    stream->position = 0;
    
    while (*input && stream->count < MAX_TOKENS) {
        skip_whitespace(&input);
        if (!*input) break;
        
        Token token;
        if (parse_number(&input, &token) ||
            parse_paren(&input, &token) ||
            parse_operator(&input, &token) ||
            parse_identifier(&input, &token)) {
            if (stream->count < MAX_TOKENS) {
                stream->tokens[stream->count++] = token;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    }
    
    if (stream->count >= MAX_TOKENS) return 0;
    
    Token eof_token = {TOKEN_EOF, ""};
    stream->tokens[stream->count++] = eof_token;
    return 1;
}

Token *peek_token(TokenStream *stream) {
    if (stream->position >= stream->count) return NULL;
    return &stream->tokens[stream->position];
}

Token *next_token(TokenStream *stream) {
    if (stream->position >= stream->count) return NULL;
    return &stream->tokens[stream->position++];
}

int parse_expression(TokenStream *stream);
int parse_term(TokenStream *stream);
int parse_factor(TokenStream *stream);

int parse_factor(TokenStream *stream) {
    Token *token = peek_token(stream);
    if (!token) return 0;
    
    if (token->type == TOKEN_NUMBER || token->type == TOKEN_IDENTIFIER) {
        next_token(stream);
        return 1;
    } else if (token->type == TOKEN_PAREN && token->value[0] == '(') {
        next_token(stream);
        if (!parse_expression(stream)) return 0;
        token = next_token(stream);
        if (!token || token->type != TOKEN_PAREN || token->value[0] != ')') return 0;
        return 1;
    }
    return 0;
}

int parse_term(TokenStream *stream) {
    if (!parse_factor(stream)) return 0;
    
    Token *token = peek_token(stream);
    while (token && token->type == TOKEN_OPERATOR && 
          (token->value[0] == '*' || token->value[0] == '/')) {
        next_token(stream);
        if (!parse_factor(stream)) return 0;
        token = peek_token(stream);
    }
    return 1;
}

int parse_expression(TokenStream *stream) {
    if (!parse_term(stream)) return 0;
    
    Token *token = peek_token(stream);
    while (token && token->type == TOKEN_OPERATOR && 
          (token->value[0] == '+' || token->value[0] == '-')) {
        next_token(stream);
        if (!parse_term(stream)) return 0;