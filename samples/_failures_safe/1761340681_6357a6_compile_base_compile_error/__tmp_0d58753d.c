//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
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

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int parse_operator(const char *input, int pos, char *output) {
    int len = 0;
    char c = input[pos];
    
    if (is_operator_char(c)) {
        output[len++] = c;
        if (is_operator_char(input[pos + 1])) {
            output[len++] = input[pos + 1];
        }
        output[len] = '\0';
    }
    
    return len;
}

int parse_string(const char *input, int pos, char *output) {
    int len = 0;
    if (input[pos] == '"') {
        output[len++] = '"';
        pos++;
        while (input[pos] != '\0' && input[pos] != '"') {
            if (len >= 31) break;
            output[len++] = input[pos++];
        }
        if (input[pos] == '"') {
            output[len++] = '"';
        }
        output[len] = '\0';
    }
    return len;
}

int parse_number(const char *input, int pos, char *output) {
    int len = 0;
    while (isdigit(input[pos]) && len < 31) {
        output[len++] = input[pos++];
    }
    output[len] = '\0';
    return len;
}

int parse_identifier(const char *input, int pos, char *output) {
    int len = 0;
    if (isalpha(input[pos]) || input[pos] == '_') {
        while ((isalnum(input[pos]) || input[pos] == '_') && len < 31) {
            output[len++] = input[pos++];
        }
        output[len] = '\0';
    }
    return len;
}

void tokenize(const char *input, struct Token *tokens, int max_tokens, int *token_count) {
    int pos = 0;
    int line = 1;
    int column = 1;
    *token_count = 0;
    
    while (input[pos] != '\0' && *token_count < max_tokens - 1) {
        if (isspace(input[pos])) {
            if (input[pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
            continue;
        }
        
        struct Token *token = &tokens[*token_count];
        token->line = line;
        token->column = column;
        int len = 0;
        
        if (is_operator_char(input[pos])) {
            len = parse_operator(input, pos, token->value);
            if (len > 0) {
                token->type = TOKEN_OPERATOR;
            }
        } else if (input[pos] == '"') {
            len = parse_string(input, pos, token->value);
            if (len > 0) {
                token->type = TOKEN_STRING;
            }
        } else if (isdigit(input[pos])) {
            len = parse_number(input, pos, token->value);
            if (len > 0) {
                token->type = TOKEN_NUMBER;
            }
        } else if (isalpha(input[pos]) || input[pos] == '_') {
            len = parse_identifier(input, pos, token->value);
            if (len > 0) {
                token->type = TOKEN_IDENTIFIER;
            }
        }
        
        if (len > 0) {
            column += len;
            pos += len;
            (*token_count)++;
        } else {
            token->type = TOKEN_INVALID;
            token->value[0] = input[pos];
            token->value[1] = '\0';
            column++;
            pos++;
            (*token_count)++;
        }
    }
    
    if (*token_count < max_tokens) {
        tokens[*token_count].type = TOKEN_EOF;
        tokens[*token_count].value[0] = '\0';
        tokens[*token_count].line = line;
        tokens[*token_count].column = column;
        (*token_count)++;
    }
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "IDENTIFIER", "NUMBER", "OPERATOR", "STRING", "EOF", "INVALID"
    };
    
    switch (token.type) {
        case TOKEN_IDENTIFIER:
            printf("IDENTIFIER: %s at line %d, column %d\n", 
                   token.value, token.line, token.column);
            break;
        case TOKEN_NUMBER:
            printf("NUMBER: %s at line %d, column %d\n", 
                   token.value, token.line, token.column);
            break;
        case TOKEN_OPERATOR:
            printf("OPERATOR: %s at line %d, column %d\n", 
                   token.value, token.line, token.column);
            break;
        case TOKEN_STRING:
            printf("STRING: %s at line