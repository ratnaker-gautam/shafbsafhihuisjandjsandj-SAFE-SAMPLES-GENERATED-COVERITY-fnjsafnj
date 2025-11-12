//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_PAREN,
    TOKEN_EOF,
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char value[32];
};

struct Parser {
    const char *input;
    size_t pos;
    struct Token current;
};

void init_parser(struct Parser *parser, const char *input) {
    if (parser == NULL || input == NULL) return;
    parser->input = input;
    parser->pos = 0;
    parser->current.type = TOKEN_INVALID;
    parser->current.value[0] = '\0';
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

void skip_whitespace(struct Parser *parser) {
    if (parser == NULL) return;
    while (parser->input[parser->pos] != '\0' && isspace(parser->input[parser->pos])) {
        parser->pos++;
    }
}

struct Token get_next_token(struct Parser *parser) {
    if (parser == NULL) {
        struct Token invalid = {TOKEN_INVALID, ""};
        return invalid;
    }
    
    skip_whitespace(parser);
    
    if (parser->input[parser->pos] == '\0') {
        struct Token eof = {TOKEN_EOF, ""};
        return eof;
    }
    
    char current_char = parser->input[parser->pos];
    
    if (isdigit(current_char)) {
        struct Token token = {TOKEN_NUMBER, ""};
        size_t i = 0;
        while (isdigit(parser->input[parser->pos]) && i < 31) {
            token.value[i++] = parser->input[parser->pos++];
        }
        token.value[i] = '\0';
        return token;
    }
    
    if (isalpha(current_char)) {
        struct Token token = {TOKEN_IDENTIFIER, ""};
        size_t i = 0;
        while ((isalnum(parser->input[parser->pos]) || parser->input[parser->pos] == '_') && i < 31) {
            token.value[i++] = parser->input[parser->pos++];
        }
        token.value[i] = '\0';
        return token;
    }
    
    if (is_operator(current_char)) {
        struct Token token = {TOKEN_OPERATOR, ""};
        token.value[0] = parser->input[parser->pos++];
        token.value[1] = '\0';
        return token;
    }
    
    if (current_char == '(' || current_char == ')') {
        struct Token token = {TOKEN_PAREN, ""};
        token.value[0] = parser->input[parser->pos++];
        token.value[1] = '\0';
        return token;
    }
    
    struct Token invalid = {TOKEN_INVALID, ""};
    invalid.value[0] = parser->input[parser->pos++];
    invalid.value[1] = '\0';
    return invalid;
}

void advance(struct Parser *parser) {
    if (parser == NULL) return;
    parser->current = get_next_token(parser);
}

void parse_expression(struct Parser *parser);

void parse_factor(struct Parser *parser) {
    if (parser == NULL) return;
    
    switch (parser->current.type) {
        case TOKEN_NUMBER:
            printf("Number: %s\n", parser->current.value);
            advance(parser);
            break;
        case TOKEN_IDENTIFIER:
            printf("Identifier: %s\n", parser->current.value);
            advance(parser);
            break;
        case TOKEN_PAREN:
            if (strcmp(parser->current.value, "(") == 0) {
                advance(parser);
                parse_expression(parser);
                if (parser->current.type == TOKEN_PAREN && strcmp(parser->current.value, ")") == 0) {
                    advance(parser);
                } else {
                    printf("Error: Expected ')'\n");
                }
            } else {
                printf("Error: Expected '('\n");
            }
            break;
        default:
            printf("Error: Expected number, identifier, or '('\n");
            break;
    }
}

void parse_term(struct Parser *parser) {
    if (parser == NULL) return;
    
    parse_factor(parser);
    
    while (parser->current.type == TOKEN_OPERATOR && 
           (strcmp(parser->current.value, "*") == 0 || strcmp(parser->current.value, "/") == 0)) {
        printf("Operator: %s\n", parser->current.value);
        advance(parser);
        parse_factor(parser);
    }
}

void parse_expression(struct Parser *parser) {
    if (parser == NULL) return;
    
    parse_term(parser);
    
    while (parser->current.type == TOKEN_OPERATOR && 
           (strcmp(parser->current.value, "+") == 0 || strcmp(parser->current.value, "-") == 0)) {
        printf("Operator: %s\n", parser->current.value);
        advance(parser);
        parse_term(parser);
    }
}

int main(void) {
    char input[256];
    
    printf("Enter an arithmetic expression: ");
    if (fgets(input, sizeof(input), stdin