//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_EOF,
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char value[32];
    int line;
    int column;
};

struct Parser {
    const char *input;
    int position;
    int line;
    int column;
    struct Token current_token;
};

void init_parser(struct Parser *parser, const char *input) {
    parser->input = input;
    parser->position = 0;
    parser->line = 1;
    parser->column = 1;
    parser->current_token.type = TOKEN_INVALID;
    memset(parser->current_token.value, 0, sizeof(parser->current_token.value));
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '(' || c == ')';
}

void skip_whitespace(struct Parser *parser) {
    while (parser->input[parser->position] != '\0') {
        char c = parser->input[parser->position];
        if (c == ' ') {
            parser->position++;
            parser->column++;
        } else if (c == '\n') {
            parser->position++;
            parser->line++;
            parser->column = 1;
        } else {
            break;
        }
    }
}

struct Token get_next_token(struct Parser *parser) {
    skip_whitespace(parser);
    
    struct Token token;
    token.line = parser->line;
    token.column = parser->column;
    memset(token.value, 0, sizeof(token.value));
    
    if (parser->input[parser->position] == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char current = parser->input[parser->position];
    
    if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (parser->input[parser->position] != '\0' && 
               isdigit(parser->input[parser->position]) && 
               i < 31) {
            token.value[i++] = parser->input[parser->position++];
            parser->column++;
        }
        token.value[i] = '\0';
    } else if (isalpha(current)) {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (parser->input[parser->position] != '\0' && 
               (isalnum(parser->input[parser->position]) || 
                parser->input[parser->position] == '_') && 
               i < 31) {
            token.value[i++] = parser->input[parser->position++];
            parser->column++;
        }
        token.value[i] = '\0';
    } else if (is_operator(current)) {
        token.type = TOKEN_OPERATOR;
        token.value[0] = parser->input[parser->position++];
        token.value[1] = '\0';
        parser->column++;
    } else {
        token.type = TOKEN_INVALID;
        token.value[0] = parser->input[parser->position++];
        token.value[1] = '\0';
        parser->column++;
    }
    
    return token;
}

void consume_token(struct Parser *parser, enum TokenType expected_type) {
    if (parser->current_token.type == expected_type) {
        parser->current_token = get_next_token(parser);
    }
}

int parse_expression(struct Parser *parser);
int parse_term(struct Parser *parser);
int parse_factor(struct Parser *parser);

int parse_factor(struct Parser *parser) {
    if (parser->current_token.type == TOKEN_NUMBER) {
        int value = atoi(parser->current_token.value);
        consume_token(parser, TOKEN_NUMBER);
        return value;
    } else if (parser->current_token.type == TOKEN_IDENTIFIER) {
        printf("Variable '%s' at line %d, column %d\n", 
               parser->current_token.value, parser->current_token.line, parser->current_token.column);
        consume_token(parser, TOKEN_IDENTIFIER);
        return 0;
    } else if (parser->current_token.type == TOKEN_OPERATOR && parser->current_token.value[0] == '(') {
        consume_token(parser, TOKEN_OPERATOR);
        int result = parse_expression(parser);
        if (parser->current_token.type == TOKEN_OPERATOR && parser->current_token.value[0] == ')') {
            consume_token(parser, TOKEN_OPERATOR);
        }
        return result;
    }
    return 0;
}

int parse_term(struct Parser *parser) {
    int result = parse_factor(parser);
    
    while (parser->current_token.type == TOKEN_OPERATOR && 
           (parser->current_token.value[0] == '*' || parser->current_token.value[0] == '/')) {
        char op = parser->current_token.value[0];
        consume_token(parser, TOKEN_OPERATOR);
        int right = parse_factor(parser);
        
        if (op == '*') {
            result *= right;
        } else if (op == '/') {
            if (right != 0) {
                result /= right;
            }
        }
    }
    
    return result;
}

int parse_expression(struct Parser *parser