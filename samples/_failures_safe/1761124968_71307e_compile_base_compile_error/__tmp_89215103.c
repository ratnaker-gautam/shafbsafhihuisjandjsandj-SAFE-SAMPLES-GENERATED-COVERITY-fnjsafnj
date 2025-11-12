//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
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
    const char* input;
    size_t pos;
    size_t len;
    Token tokens[MAX_TOKENS];
    size_t token_count;
    size_t current_token;
} Parser;

void init_parser(Parser* parser, const char* input) {
    parser->input = input;
    parser->pos = 0;
    parser->len = strlen(input);
    parser->token_count = 0;
    parser->current_token = 0;
    memset(parser->tokens, 0, sizeof(parser->tokens));
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

void skip_whitespace(Parser* parser) {
    while (parser->pos < parser->len && isspace(parser->input[parser->pos])) {
        parser->pos++;
    }
}

Token get_next_token(Parser* parser) {
    Token token = {TOKEN_INVALID, ""};
    
    if (parser->pos >= parser->len) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char current = parser->input[parser->pos];
    
    if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        size_t i = 0;
        while (parser->pos < parser->len && i < MAX_TOKEN_LEN - 1 && 
               isdigit(parser->input[parser->pos])) {
            token.value[i++] = parser->input[parser->pos++];
        }
        token.value[i] = '\0';
    } else if (isalpha(current)) {
        token.type = TOKEN_IDENTIFIER;
        size_t i = 0;
        while (parser->pos < parser->len && i < MAX_TOKEN_LEN - 1 && 
               (isalnum(parser->input[parser->pos]) || parser->input[parser->pos] == '_')) {
            token.value[i++] = parser->input[parser->pos++];
        }
        token.value[i] = '\0';
    } else if (is_operator(current)) {
        token.type = TOKEN_OPERATOR;
        token.value[0] = current;
        token.value[1] = '\0';
        parser->pos++;
    } else if (current == '(' || current == ')') {
        token.type = TOKEN_PAREN;
        token.value[0] = current;
        token.value[1] = '\0';
        parser->pos++;
    } else {
        token.type = TOKEN_INVALID;
        token.value[0] = current;
        token.value[1] = '\0';
        parser->pos++;
    }
    
    return token;
}

int tokenize(Parser* parser) {
    while (parser->token_count < MAX_TOKENS) {
        skip_whitespace(parser);
        Token token = get_next_token(parser);
        
        if (token.type == TOKEN_EOF) {
            parser->tokens[parser->token_count++] = token;
            break;
        }
        
        if (token.type == TOKEN_INVALID) {
            return 0;
        }
        
        parser->tokens[parser->token_count++] = token;
    }
    
    return 1;
}

Token current_token(Parser* parser) {
    if (parser->current_token < parser->token_count) {
        return parser->tokens[parser->current_token];
    }
    Token eof = {TOKEN_EOF, ""};
    return eof;
}

void consume_token(Parser* parser) {
    if (parser->current_token < parser->token_count) {
        parser->current_token++;
    }
}

int parse_expression(Parser* parser);
int parse_term(Parser* parser);
int parse_factor(Parser* parser);

int parse_factor(Parser* parser) {
    Token token = current_token(parser);
    
    if (token.type == TOKEN_NUMBER) {
        consume_token(parser);
        return atoi(token.value);
    } else if (token.type == TOKEN_IDENTIFIER) {
        consume_token(parser);
        return 0;
    } else if (token.type == TOKEN_PAREN && token.value[0] == '(') {
        consume_token(parser);
        int result = parse_expression(parser);
        token = current_token(parser);
        if (token.type == TOKEN_PAREN && token.value[0] == ')') {
            consume_token(parser);
            return result;
        }
        return 0;
    }
    
    return 0;
}

int parse_term(Parser* parser) {
    int result = parse_factor(parser);
    
    while (1) {
        Token token = current_token(parser);
        if (token.type == TOKEN_OPERATOR && 
            (token.value[0] == '*' || token.value[0] == '/')) {
            consume_token(parser);
            int right = parse_factor(parser);
            if (token.value[0] == '*') {
                result *= right;
            } else if (right != 0) {
                result /= right;
            }
        } else {