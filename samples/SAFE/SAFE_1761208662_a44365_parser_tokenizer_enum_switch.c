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

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

void parser_init(struct Parser *parser, const char *input) {
    if (parser == NULL || input == NULL) return;
    parser->input = input;
    parser->position = 0;
    parser->current_token.type = TOKEN_INVALID;
    parser->current_token.value[0] = '\0';
}

void next_token(struct Parser *parser) {
    if (parser == NULL || parser->input == NULL) return;
    
    while (parser->input[parser->position] != '\0' && 
           isspace(parser->input[parser->position])) {
        parser->position++;
    }
    
    if (parser->input[parser->position] == '\0') {
        parser->current_token.type = TOKEN_EOF;
        parser->current_token.value[0] = '\0';
        return;
    }
    
    char current_char = parser->input[parser->position];
    
    if (isdigit(current_char)) {
        parser->current_token.type = TOKEN_NUMBER;
        size_t i = 0;
        while (isdigit(parser->input[parser->position]) && i < 31) {
            parser->current_token.value[i++] = parser->input[parser->position++];
        }
        parser->current_token.value[i] = '\0';
    } else if (isalpha(current_char)) {
        parser->current_token.type = TOKEN_IDENTIFIER;
        size_t i = 0;
        while (isalnum(parser->input[parser->position]) && i < 31) {
            parser->current_token.value[i++] = parser->input[parser->position++];
        }
        parser->current_token.value[i] = '\0';
    } else if (is_operator_char(current_char)) {
        parser->current_token.type = TOKEN_OPERATOR;
        parser->current_token.value[0] = current_char;
        parser->current_token.value[1] = '\0';
        parser->position++;
    } else if (current_char == '(' || current_char == ')') {
        parser->current_token.type = TOKEN_PAREN;
        parser->current_token.value[0] = current_char;
        parser->current_token.value[1] = '\0';
        parser->position++;
    } else {
        parser->current_token.type = TOKEN_INVALID;
        parser->current_token.value[0] = current_char;
        parser->current_token.value[1] = '\0';
        parser->position++;
    }
}

void parse_expression(struct Parser *parser) {
    if (parser == NULL) return;
    
    while (parser->current_token.type != TOKEN_EOF && 
           parser->current_token.type != TOKEN_INVALID) {
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
        next_token(parser);
    }
}

int main(void) {
    char input_buffer[1024];
    
    printf("Enter an expression to parse: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (strlen(input_buffer) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    struct Parser parser;
    parser_init(&parser, input_buffer);
    next_token(&parser);
    parse_expression(&parser);
    
    return 0;
}