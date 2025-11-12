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
    int pos;
} TokenStream;

void skip_whitespace(const char *input, int *index) {
    while (input[*index] && isspace(input[*index])) {
        (*index)++;
    }
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

Token get_next_token(const char *input, int *index) {
    Token token = {TOKEN_INVALID, ""};
    int i = *index;
    
    skip_whitespace(input, &i);
    
    if (!input[i]) {
        token.type = TOKEN_EOF;
        *index = i;
        return token;
    }
    
    if (isdigit(input[i])) {
        token.type = TOKEN_NUMBER;
        int j = 0;
        while (input[i] && isdigit(input[i]) && j < MAX_TOKEN_LEN - 1) {
            token.value[j++] = input[i++];
        }
        token.value[j] = '\0';
    } else if (isalpha(input[i])) {
        token.type = TOKEN_IDENTIFIER;
        int j = 0;
        while (input[i] && (isalnum(input[i]) || input[i] == '_') && j < MAX_TOKEN_LEN - 1) {
            token.value[j++] = input[i++];
        }
        token.value[j] = '\0';
    } else if (is_operator_char(input[i])) {
        token.type = TOKEN_OPERATOR;
        token.value[0] = input[i++];
        token.value[1] = '\0';
    } else if (input[i] == '(' || input[i] == ')') {
        token.type = TOKEN_PAREN;
        token.value[0] = input[i++];
        token.value[1] = '\0';
    } else {
        token.type = TOKEN_INVALID;
        token.value[0] = input[i++];
        token.value[1] = '\0';
    }
    
    *index = i;
    return token;
}

int tokenize(const char *input, TokenStream *stream) {
    int index = 0;
    stream->count = 0;
    
    while (stream->count < MAX_TOKENS - 1) {
        Token token = get_next_token(input, &index);
        stream->tokens[stream->count] = token;
        stream->count++;
        
        if (token.type == TOKEN_EOF || token.type == TOKEN_INVALID) {
            break;
        }
    }
    
    stream->pos = 0;
    return stream->count > 0 ? 0 : -1;
}

Token current_token(TokenStream *stream) {
    if (stream->pos < stream->count) {
        return stream->tokens[stream->pos];
    }
    Token eof = {TOKEN_EOF, ""};
    return eof;
}

void next_token(TokenStream *stream) {
    if (stream->pos < stream->count) {
        stream->pos++;
    }
}

int parse_expression(TokenStream *stream);

int parse_factor(TokenStream *stream) {
    Token token = current_token(stream);
    
    if (token.type == TOKEN_NUMBER) {
        int value = atoi(token.value);
        next_token(stream);
        return value;
    } else if (token.type == TOKEN_IDENTIFIER) {
        next_token(stream);
        return 0;
    } else if (token.type == TOKEN_PAREN && token.value[0] == '(') {
        next_token(stream);
        int result = parse_expression(stream);
        token = current_token(stream);
        if (token.type == TOKEN_PAREN && token.value[0] == ')') {
            next_token(stream);
            return result;
        }
        return 0;
    }
    
    return 0;
}

int parse_term(TokenStream *stream) {
    int result = parse_factor(stream);
    Token token = current_token(stream);
    
    while (token.type == TOKEN_OPERATOR && (token.value[0] == '*' || token.value[0] == '/')) {
        next_token(stream);
        int right = parse_factor(stream);
        
        if (token.value[0] == '*') {
            result *= right;
        } else if (token.value[0] == '/') {
            if (right != 0) {
                result /= right;
            }
        }
        
        token = current_token(stream);
    }
    
    return result;
}

int parse_expression(TokenStream *stream) {
    int result = parse_term(stream);
    Token token = current_token(stream);
    
    while (token.type == TOKEN_OPERATOR && (token.value[0] == '+' || token.value[0] == '-')) {
        next_token(stream);
        int right = parse_term(stream);
        
        if (token.value[0] == '+') {
            result += right;
        } else if (token.value[