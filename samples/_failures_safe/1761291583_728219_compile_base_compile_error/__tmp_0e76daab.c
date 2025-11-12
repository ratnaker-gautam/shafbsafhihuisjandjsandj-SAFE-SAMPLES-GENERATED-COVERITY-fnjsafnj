//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_LEN 256
#define MAX_TOKENS 1000

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_PAREN,
    TOKEN_EOF,
    TOKEN_INVALID
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

Token get_next_token(const char **input) {
    Token token = {TOKEN_INVALID, ""};
    skip_whitespace(input);
    
    if (!**input) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char c = **input;
    
    if (isdigit(c)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (**input && (isdigit(**input) || **input == '.')) {
            if (i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = **input;
            }
            (*input)++;
        }
        token.value[i] = '\0';
    } else if (isalpha(c) || c == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (**input && (isalnum(**input) || **input == '_')) {
            if (i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = **input;
            }
            (*input)++;
        }
        token.value[i] = '\0';
    } else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '=') {
        token.type = TOKEN_OPERATOR;
        token.value[0] = c;
        token.value[1] = '\0';
        (*input)++;
    } else if (c == '(' || c == ')') {
        token.type = TOKEN_PAREN;
        token.value[0] = c;
        token.value[1] = '\0';
        (*input)++;
    } else {
        token.type = TOKEN_INVALID;
        token.value[0] = c;
        token.value[1] = '\0';
        (*input)++;
    }
    
    return token;
}

int tokenize(const char *input, TokenStream *stream) {
    if (!input || !stream) return 0;
    
    stream->count = 0;
    stream->position = 0;
    
    while (*input && stream->count < MAX_TOKENS - 1) {
        Token token = get_next_token(&input);
        if (token.type == TOKEN_EOF) {
            break;
        }
        stream->tokens[stream->count] = token;
        stream->count++;
    }
    
    stream->tokens[stream->count].type = TOKEN_EOF;
    stream->tokens[stream->count].value[0] = '\0';
    return stream->count;
}

Token current_token(TokenStream *stream) {
    if (!stream) {
        Token invalid = {TOKEN_INVALID, ""};
        return invalid;
    }
    if (stream->position < stream->count) {
        return stream->tokens[stream->position];
    }
    return stream->tokens[stream->count];
}

void advance_token(TokenStream *stream) {
    if (stream && stream->position < stream->count) {
        stream->position++;
    }
}

void parse_expression(TokenStream *stream);
void parse_term(TokenStream *stream);
void parse_factor(TokenStream *stream);

void parse_factor(TokenStream *stream) {
    if (!stream) return;
    
    Token token = current_token(stream);
    
    if (token.type == TOKEN_NUMBER || token.type == TOKEN_IDENTIFIER) {
        printf("Factor: %s\n", token.value);
        advance_token(stream);
    } else if (token.type == TOKEN_PAREN && token.value[0] == '(') {
        advance_token(stream);
        parse_expression(stream);
        token = current_token(stream);
        if (token.type == TOKEN_PAREN && token.value[0] == ')') {
            advance_token(stream);
        } else {
            printf("Error: Expected ')'\n");
        }
    } else {
        printf("Error: Unexpected token in factor: %s\n", token.value);
    }
}

void parse_term(TokenStream *stream) {
    if (!stream) return;
    
    parse_factor(stream);
    
    Token token = current_token(stream);
    while (token.type == TOKEN_OPERATOR && 
           (token.value[0] == '*' || token.value[0] == '/')) {
        printf("Operator: %s\n", token.value);
        advance_token(stream);
        parse_factor(stream);
        token = current_token(stream);
    }
}

void parse_expression(TokenStream *stream) {
    if (!stream) return;
    
    parse_term(stream);
    
    Token token = current_token(stream);
    while (token.type == TOKEN_OPERATOR && 
           (token.value[0] == '+' || token.value[0] == '-')) {
        printf("Operator: %s\n", token.value);
        advance_token(stream);
        parse_term(stream);