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

void parser_init(struct Parser *parser, const char *input) {
    if (parser == NULL || input == NULL) return;
    parser->input = input;
    parser->position = 0;
    parser->line = 1;
    parser->column = 1;
    memset(&parser->current_token, 0, sizeof(parser->current_token));
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(struct Parser *parser) {
    if (parser == NULL || parser->input == NULL) return;
    
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
    struct Token token = {TOKEN_INVALID, "", 0, 0};
    if (parser == NULL || parser->input == NULL) return token;
    
    skip_whitespace(parser);
    
    if (parser->input[parser->position] == '\0') {
        token.type = TOKEN_EOF;
        token.line = parser->line;
        token.column = parser->column;
        return token;
    }
    
    char current = parser->input[parser->position];
    token.line = parser->line;
    token.column = parser->column;
    
    if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (i < 31 && isdigit(parser->input[parser->position])) {
            token.value[i++] = parser->input[parser->position++];
            parser->column++;
        }
        token.value[i] = '\0';
    } else if (isalpha(current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (i < 31 && (isalnum(parser->input[parser->position]) || parser->input[parser->position] == '_')) {
            token.value[i++] = parser->input[parser->position++];
            parser->column++;
        }
        token.value[i] = '\0';
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        token.value[0] = parser->input[parser->position++];
        token.value[1] = '\0';
        parser->column++;
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        int i = 0;
        parser->position++;
        parser->column++;
        
        while (i < 31 && parser->input[parser->position] != '\0' && parser->input[parser->position] != '"') {
            token.value[i++] = parser->input[parser->position++];
            parser->column++;
        }
        
        if (parser->input[parser->position] == '"') {
            parser->position++;
            parser->column++;
        }
        token.value[i] = '\0';
    } else {
        token.type = TOKEN_INVALID;
        token.value[0] = parser->input[parser->position++];
        token.value[1] = '\0';
        parser->column++;
    }
    
    return token;
}

void print_token(const struct Token *token) {
    if (token == NULL) return;
    
    switch (token->type) {
        case TOKEN_NUMBER:
            printf("NUMBER: %s", token->value);
            break;
        case TOKEN_IDENTIFIER:
            printf("IDENTIFIER: %s", token->value);
            break;
        case TOKEN_OPERATOR:
            printf("OPERATOR: %s", token->value);
            break;
        case TOKEN_STRING:
            printf("STRING: \"%s\"", token->value);
            break;
        case TOKEN_EOF:
            printf("EOF");
            break;
        case TOKEN_INVALID:
            printf("INVALID: %s", token->value);
            break;
    }
    printf(" (line %d, column %d)\n", token->line, token->column);
}

int main(void) {
    char input[1024];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 1) {
        printf