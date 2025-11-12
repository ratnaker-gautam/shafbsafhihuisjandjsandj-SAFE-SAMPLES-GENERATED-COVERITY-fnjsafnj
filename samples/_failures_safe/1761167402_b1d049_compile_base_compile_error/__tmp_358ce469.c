//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_LEN 64
#define MAX_TOKENS 100

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

void init_token_stream(TokenStream *stream) {
    stream->count = 0;
    stream->position = 0;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

int tokenize_expression(const char *input, TokenStream *stream) {
    int i = 0;
    int token_count = 0;
    
    while (input[i] != '\0' && token_count < MAX_TOKENS) {
        while (isspace(input[i])) i++;
        
        if (input[i] == '\0') break;
        
        if (isdigit(input[i])) {
            int j = 0;
            while (isdigit(input[i]) && j < MAX_TOKEN_LEN - 1) {
                stream->tokens[token_count].value[j++] = input[i++];
            }
            stream->tokens[token_count].value[j] = '\0';
            stream->tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
        }
        else if (isalpha(input[i])) {
            int j = 0;
            while (isalnum(input[i]) && j < MAX_TOKEN_LEN - 1) {
                stream->tokens[token_count].value[j++] = input[i++];
            }
            stream->tokens[token_count].value[j] = '\0';
            stream->tokens[token_count].type = TOKEN_IDENTIFIER;
            token_count++;
        }
        else if (is_operator_char(input[i])) {
            stream->tokens[token_count].value[0] = input[i++];
            stream->tokens[token_count].value[1] = '\0';
            stream->tokens[token_count].type = TOKEN_OPERATOR;
            token_count++;
        }
        else if (input[i] == '(' || input[i] == ')') {
            stream->tokens[token_count].value[0] = input[i++];
            stream->tokens[token_count].value[1] = '\0';
            stream->tokens[token_count].type = TOKEN_PAREN;
            token_count++;
        }
        else {
            return -1;
        }
    }
    
    stream->tokens[token_count].type = TOKEN_EOF;
    stream->tokens[token_count].value[0] = '\0';
    stream->count = token_count + 1;
    return 0;
}

Token current_token(TokenStream *stream) {
    if (stream->position < stream->count) {
        return stream->tokens[stream->position];
    }
    Token eof_token = {TOKEN_EOF, ""};
    return eof_token;
}

void next_token(TokenStream *stream) {
    if (stream->position < stream->count - 1) {
        stream->position++;
    }
}

int parse_expression(TokenStream *stream);
int parse_term(TokenStream *stream);
int parse_factor(TokenStream *stream);

int parse_factor(TokenStream *stream) {
    Token token = current_token(stream);
    
    if (token.type == TOKEN_NUMBER) {
        int value = atoi(token.value);
        next_token(stream);
        return value;
    }
    else if (token.type == TOKEN_IDENTIFIER) {
        next_token(stream);
        return 0;
    }
    else if (token.type == TOKEN_PAREN && strcmp(token.value, "(") == 0) {
        next_token(stream);
        int result = parse_expression(stream);
        token = current_token(stream);
        if (token.type == TOKEN_PAREN && strcmp(token.value, ")") == 0) {
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
    
    while (token.type == TOKEN_OPERATOR && 
          (strcmp(token.value, "*") == 0 || strcmp(token.value, "/") == 0)) {
        char op = token.value[0];
        next_token(stream);
        int right = parse_factor(stream);
        
        if (op == '*') {
            result *= right;
        }
        else if (op == '/') {
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
    
    while (token.type == TOKEN_OPERATOR && 
          (strcmp(token.value, "+") == 0 || strcmp(token.value, "-") == 0)) {
        char op = token.value[0];
        next_token(stream);
        int right = parse_term(stream);
        
        if (op == '+') {
            result +=