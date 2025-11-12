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
    parser->current_token.type = TOKEN_INVALID;
    memset(parser->current_token.value, 0, sizeof(parser->current_token.value));
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
    if (parser == NULL || parser->input == NULL) {
        struct Token invalid = {TOKEN_INVALID, "", 0, 0};
        return invalid;
    }
    
    skip_whitespace(parser);
    
    if (parser->input[parser->position] == '\0') {
        struct Token eof = {TOKEN_EOF, "", parser->line, parser->column};
        return eof;
    }
    
    char current = parser->input[parser->position];
    struct Token token;
    token.line = parser->line;
    token.column = parser->column;
    memset(token.value, 0, sizeof(token.value));
    
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
    } else if (isalpha(current) || current == '_') {
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
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (parser->input[parser->position] != '\0' && 
               is_operator_char(parser->input[parser->position]) && 
               i < 31) {
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
        while (parser->input[parser->position] != '\0' && 
               parser->input[parser->position] != '"' && 
               i < 30) {
            token.value[i++] = parser->input[parser->position++];
            parser->column++;
        }
        if (parser->input[parser->position] == '"') {
            token.value[i++] = '"';
            parser->position++;
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

void print_token(const struct Token *token) {
    if (token == NULL) return;
    
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "INVALID"
    };
    
    printf("Line %d, Column %d: %s", token->line, token->column, type_names[token->type]);
    if (token->type != TOKEN_EOF) {
        printf(" ('%s')", token->value);
    }
    printf("\n");
}

int main(void) {
    char input_buffer[1024];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr