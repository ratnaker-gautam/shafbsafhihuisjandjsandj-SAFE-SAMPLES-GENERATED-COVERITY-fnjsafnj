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
    size_t position;
    struct Token current_token;
};

void init_parser(struct Parser *parser, const char *input) {
    if (parser == NULL || input == NULL) {
        exit(EXIT_FAILURE);
    }
    parser->input = input;
    parser->position = 0;
    parser->current_token.type = TOKEN_INVALID;
    parser->current_token.value[0] = '\0';
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

void skip_whitespace(struct Parser *parser) {
    if (parser == NULL) return;
    while (parser->input[parser->position] != '\0' && 
           isspace((unsigned char)parser->input[parser->position])) {
        parser->position++;
    }
}

struct Token get_next_token(struct Parser *parser) {
    if (parser == NULL) {
        struct Token invalid = {TOKEN_INVALID, ""};
        return invalid;
    }
    
    skip_whitespace(parser);
    
    if (parser->input[parser->position] == '\0') {
        struct Token eof = {TOKEN_EOF, ""};
        return eof;
    }
    
    char current = parser->input[parser->position];
    struct Token token = {TOKEN_INVALID, ""};
    
    if (isdigit((unsigned char)current)) {
        token.type = TOKEN_NUMBER;
        size_t i = 0;
        while (isdigit((unsigned char)parser->input[parser->position]) && i < 31) {
            token.value[i++] = parser->input[parser->position++];
        }
        token.value[i] = '\0';
    } else if (isalpha((unsigned char)current)) {
        token.type = TOKEN_IDENTIFIER;
        size_t i = 0;
        while (isalnum((unsigned char)parser->input[parser->position]) && i < 31) {
            token.value[i++] = parser->input[parser->position++];
        }
        token.value[i] = '\0';
    } else if (is_operator(current)) {
        token.type = TOKEN_OPERATOR;
        token.value[0] = parser->input[parser->position++];
        token.value[1] = '\0';
    } else if (current == '(' || current == ')') {
        token.type = TOKEN_PAREN;
        token.value[0] = parser->input[parser->position++];
        token.value[1] = '\0';
    } else {
        token.type = TOKEN_INVALID;
        token.value[0] = parser->input[parser->position++];
        token.value[1] = '\0';
    }
    
    return token;
}

void advance_token(struct Parser *parser) {
    if (parser == NULL) return;
    parser->current_token = get_next_token(parser);
}

void parse_expression(struct Parser *parser) {
    if (parser == NULL) return;
    
    switch (parser->current_token.type) {
        case TOKEN_NUMBER:
            printf("Number: %s\n", parser->current_token.value);
            advance_token(parser);
            break;
        case TOKEN_IDENTIFIER:
            printf("Identifier: %s\n", parser->current_token.value);
            advance_token(parser);
            break;
        case TOKEN_PAREN:
            if (parser->current_token.value[0] == '(') {
                printf("Open parenthesis\n");
                advance_token(parser);
                parse_expression(parser);
                if (parser->current_token.type == TOKEN_PAREN && 
                    parser->current_token.value[0] == ')') {
                    printf("Close parenthesis\n");
                    advance_token(parser);
                }
            }
            break;
        default:
            printf("Unexpected token\n");
            break;
    }
    
    if (parser->current_token.type == TOKEN_OPERATOR) {
        printf("Operator: %s\n", parser->current_token.value);
        advance_token(parser);
        parse_expression(parser);
    }
}

int main(void) {
    char input[256];
    
    printf("Enter an expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return EXIT_FAILURE;
    }
    
    struct Parser parser;
    init_parser(&parser, input);
    advance_token(&parser);
    
    printf("Parsing tokens:\n");
    parse_expression(&parser);
    
    if (parser.current_token.type != TOKEN_EOF) {
        printf("Unexpected token at end: ");
        switch (parser.current_token.type) {
            case TOKEN_NUMBER: printf("Number %s\n", parser.current_token.value); break;