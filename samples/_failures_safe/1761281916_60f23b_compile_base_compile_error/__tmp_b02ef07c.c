//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
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

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || 
           c == '=' || c == '<' || c == '>' || c == '!';
}

void init_parser(struct Parser *parser, const char *input) {
    parser->input = input;
    parser->position = 0;
    parser->line = 1;
    parser->column = 1;
    parser->current_token.type = TOKEN_INVALID;
    memset(parser->current_token.value, 0, sizeof(parser->current_token.value));
}

void skip_whitespace(struct Parser *parser) {
    while (parser->input[parser->position] != '\0') {
        char c = parser->input[parser->position];
        if (c == ' ' || c == '\t') {
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
    struct Token token;
    memset(&token, 0, sizeof(token));
    token.line = parser->line;
    token.column = parser->column;
    
    skip_whitespace(parser);
    
    if (parser->input[parser->position] == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char current = parser->input[parser->position];
    
    if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (isdigit(parser->input[parser->position]) && i < 31) {
            token.value[i++] = parser->input[parser->position++];
            parser->column++;
        }
        token.value[i] = '\0';
    } else if (isalpha(current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while ((isalnum(parser->input[parser->position]) || 
                parser->input[parser->position] == '_') && i < 31) {
            token.value[i++] = parser->input[parser->position++];
            parser->column++;
        }
        token.value[i] = '\0';
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        token.value[0] = '"';
        parser->position++;
        parser->column++;
        int i = 1;
        while (parser->input[parser->position] != '"' && 
               parser->input[parser->position] != '\0' && i < 30) {
            token.value[i++] = parser->input[parser->position++];
            parser->column++;
        }
        if (parser->input[parser->position] == '"') {
            token.value[i++] = '"';
            parser->position++;
            parser->column++;
        }
        token.value[i] = '\0';
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (is_operator_char(parser->input[parser->position]) && i < 31) {
            token.value[i++] = parser->input[parser->position++];
            parser->column++;
        }
        token.value[i] = '\0';
    } else {
        token.type = TOKEN_INVALID;
        token.value[0] = current;
        token.value[1] = '\0';
        parser->position++;
        parser->column++;
    }
    
    return token;
}

void parse_expression(struct Parser *parser) {
    struct Token token = parser->current_token;
    
    switch (token.type) {
        case TOKEN_NUMBER:
            printf("Number: %s\n", token.value);
            break;
        case TOKEN_IDENTIFIER:
            printf("Identifier: %s\n", token.value);
            break;
        case TOKEN_STRING:
            printf("String: %s\n", token.value);
            break;
        case TOKEN_OPERATOR:
            printf("Operator: %s\n", token.value);
            break;
        case TOKEN_INVALID:
            printf("Invalid token: '%s' at line %d, column %d\n", 
                   token.value, token.line, token.column);
            break;
        case TOKEN_EOF:
            printf("End of input\n");
            break;
    }
}

int main(void) {
    char input[1024];
    
    printf("Enter expression to parse: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0