//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
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
    TOKEN_UNKNOWN
};

struct Token {
    enum TokenType type;
    char value[64];
    int line;
    int column;
};

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int parse_operator(const char *input, int pos, char *output) {
    int len = 0;
    char c = input[pos];
    
    if (is_operator_char(c)) {
        output[len++] = c;
        char next = input[pos + 1];
        
        if ((c == '=' && next == '=') || (c == '!' && next == '=') ||
            (c == '<' && next == '=') || (c == '>' && next == '=') ||
            (c == '&' && next == '&') || (c == '|' && next == '|')) {
            output[len++] = next;
        }
    }
    
    output[len] = '\0';
    return len > 0 ? len : 0;
}

int parse_number(const char *input, int pos, char *output) {
    int len = 0;
    int has_dot = 0;
    
    while (input[pos] != '\0') {
        char c = input[pos];
        if (isdigit(c)) {
            if (len < 63) output[len++] = c;
        } else if (c == '.' && !has_dot) {
            if (len < 63) output[len++] = c;
            has_dot = 1;
        } else {
            break;
        }
        pos++;
    }
    
    output[len] = '\0';
    return len > 0 ? len : 0;
}

int parse_identifier(const char *input, int pos, char *output) {
    int len = 0;
    
    if (isalpha(input[pos]) || input[pos] == '_') {
        while (input[pos] != '\0' && (isalnum(input[pos]) || input[pos] == '_')) {
            if (len < 63) output[len++] = input[pos];
            pos++;
        }
    }
    
    output[len] = '\0';
    return len > 0 ? len : 0;
}

int parse_string(const char *input, int pos, char *output) {
    int len = 0;
    
    if (input[pos] == '"') {
        if (len < 63) output[len++] = input[pos];
        pos++;
        
        while (input[pos] != '\0' && input[pos] != '"') {
            if (len < 63) output[len++] = input[pos];
            pos++;
        }
        
        if (input[pos] == '"') {
            if (len < 63) output[len++] = input[pos];
        }
    }
    
    output[len] = '\0';
    return len > 0 ? len : 0;
}

void tokenize(const char *input, struct Token *tokens, int max_tokens, int *token_count) {
    int pos = 0;
    int line = 1;
    int column = 1;
    *token_count = 0;
    
    while (input[pos] != '\0' && *token_count < max_tokens - 1) {
        while (isspace(input[pos])) {
            if (input[pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
        }
        
        if (input[pos] == '\0') break;
        
        struct Token *token = &tokens[*token_count];
        token->line = line;
        token->column = column;
        int advance = 0;
        
        if ((advance = parse_number(input, pos, token->value)) > 0) {
            token->type = TOKEN_NUMBER;
        } else if ((advance = parse_identifier(input, pos, token->value)) > 0) {
            token->type = TOKEN_IDENTIFIER;
        } else if ((advance = parse_operator(input, pos, token->value)) > 0) {
            token->type = TOKEN_OPERATOR;
        } else if ((advance = parse_string(input, pos, token->value)) > 0) {
            token->type = TOKEN_STRING;
        } else {
            token->type = TOKEN_UNKNOWN;
            if (token->value[0] != '\0' && token->value[0] != '\0') {
                token->value[0] = input[pos];
                token->value[1] = '\0';
            }
            advance = 1;
        }
        
        if (advance > 0) {
            pos += advance;
            column += advance;
            (*token_count)++;
        } else {
            break;
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

const char* token_type_name(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";