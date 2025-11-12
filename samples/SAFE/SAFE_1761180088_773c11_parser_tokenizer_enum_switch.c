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
        return;
    }
    parser->input = input;
    parser->position = 0;
    parser->current_token.type = TOKEN_INVALID;
    memset(parser->current_token.value, 0, sizeof(parser->current_token.value));
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

void skip_whitespace(struct Parser *parser) {
    if (parser == NULL || parser->input == NULL) {
        return;
    }
    while (parser->input[parser->position] != '\0' && isspace(parser->input[parser->position])) {
        parser->position++;
    }
}

struct Token get_next_token(struct Parser *parser) {
    struct Token token;
    token.type = TOKEN_INVALID;
    memset(token.value, 0, sizeof(token.value));
    
    if (parser == NULL || parser->input == NULL) {
        return token;
    }
    
    skip_whitespace(parser);
    
    if (parser->input[parser->position] == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char current = parser->input[parser->position];
    
    if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        size_t i = 0;
        while (isdigit(parser->input[parser->position]) && i < sizeof(token.value) - 1) {
            token.value[i++] = parser->input[parser->position++];
        }
        token.value[i] = '\0';
    } else if (isalpha(current)) {
        token.type = TOKEN_IDENTIFIER;
        size_t i = 0;
        while (isalnum(parser->input[parser->position]) && i < sizeof(token.value) - 1) {
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

void parse_expression(struct Parser *parser) {
    if (parser == NULL) {
        return;
    }
    
    parser->current_token = get_next_token(parser);
    
    while (parser->current_token.type != TOKEN_EOF) {
        switch (parser->current_token.type) {
            case TOKEN_NUMBER:
                printf("Number: %s\n", parser->current_token.value);
                break;
            case TOKEN_IDENTIFIER:
                printf("Identifier: %s\n", parser->current_token.value);
                break;
            case TOKEN_OPERATOR:
                printf("Operator: %s\n", parser->current_token.value);
                break;
            case TOKEN_PAREN:
                printf("Parenthesis: %s\n", parser->current_token.value);
                break;
            case TOKEN_INVALID:
                printf("Invalid token: %s\n", parser->current_token.value);
                break;
            case TOKEN_EOF:
                return;
        }
        parser->current_token = get_next_token(parser);
    }
}

int main(void) {
    char input[256];
    
    printf("Enter an expression to parse: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    struct Parser parser;
    init_parser(&parser, input);
    
    printf("Parsing tokens:\n");
    parse_expression(&parser);
    
    return 0;
}