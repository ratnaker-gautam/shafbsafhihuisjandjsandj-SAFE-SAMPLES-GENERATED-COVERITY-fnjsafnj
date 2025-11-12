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
    TOKEN_ERROR
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
    parser->current_token.type = TOKEN_ERROR;
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
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (is_operator_char(parser->input[parser->position]) && i < 31) {
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
            token.value[i] = '\0';
        } else {
            token.type = TOKEN_ERROR;
        }
    } else {
        token.type = TOKEN_ERROR;
        token.value[0] = current;
        token.value[1] = '\0';
        parser->position++;
        parser->column++;
    }
    
    return token;
}

const char* token_type_to_string(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

void process_tokens(const char *input) {
    struct Parser parser;
    init_parser(&parser, input);
    
    printf("Input: \"%s\"\n", input);
    printf("Tokens:\n");
    
    while (1) {
        struct Token token = get_next_token(&parser);
        
        printf("  %s", token_type_to_string(token.type));
        if (token.type != TOKEN_EOF && token.type != TOKEN_ERROR) {
            printf(": '%s'", token.value);
        }
        printf(" (line %d, col %d)\n", token.line, token.column);
        
        if (token.type == TOKEN_EOF || token.type == TOKEN_ERROR) {
            break;
        }
    }
    printf("\n");
}

int main(void) {
    char input[256];