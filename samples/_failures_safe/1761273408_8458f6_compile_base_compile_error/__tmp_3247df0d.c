//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
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
    char value[64];
    int line;
    int column;
};

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int parse_operator(const char *input, int pos, char *output) {
    int len = 0;
    if (input == NULL || output == NULL || pos < 0) return 0;
    
    char c = input[pos];
    if (c == '\0') return 0;
    
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
    if (input == NULL || output == NULL || pos < 0) return 0;
    
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

int parse_string(const char *input, int pos, char *output) {
    int len = 0;
    if (input == NULL || output == NULL || pos < 0) return 0;
    
    if (input[pos] == '"') {
        pos++;
        while (input[pos] != '\0' && input[pos] != '"') {
            if (len < 63) output[len++] = input[pos];
            pos++;
        }
        if (input[pos] == '"') {
            output[len] = '\0';
            return len + 2;
        }
    }
    
    return 0;
}

int parse_identifier(const char *input, int pos, char *output) {
    int len = 0;
    if (input == NULL || output == NULL || pos < 0) return 0;
    
    if (isalpha(input[pos]) || input[pos] == '_') {
        while (input[pos] != '\0' && (isalnum(input[pos]) || input[pos] == '_')) {
            if (len < 63) output[len++] = input[pos];
            pos++;
        }
        output[len] = '\0';
        return len;
    }
    
    return 0;
}

int get_next_token(const char *input, int *pos, int *line, int *column, struct Token *token) {
    if (input == NULL || pos == NULL || line == NULL || column == NULL || token == NULL) {
        return -1;
    }
    
    while (input[*pos] != '\0') {
        char c = input[*pos];
        
        if (isspace(c)) {
            if (c == '\n') {
                (*line)++;
                *column = 1;
            } else {
                (*column)++;
            }
            (*pos)++;
            continue;
        }
        
        int token_len = 0;
        token->line = *line;
        token->column = *column;
        
        if ((token_len = parse_number(input, *pos, token->value)) > 0) {
            token->type = TOKEN_NUMBER;
            *pos += token_len;
            *column += token_len;
            return 0;
        }
        
        if ((token_len = parse_string(input, *pos, token->value)) > 0) {
            token->type = TOKEN_STRING;
            *pos += token_len;
            *column += token_len;
            return 0;
        }
        
        if ((token_len = parse_operator(input, *pos, token->value)) > 0) {
            token->type = TOKEN_OPERATOR;
            *pos += token_len;
            *column += token_len;
            return 0;
        }
        
        if ((token_len = parse_identifier(input, *pos, token->value)) > 0) {
            token->type = TOKEN_IDENTIFIER;
            *pos += token_len;
            *column += token_len;
            return 0;
        }
        
        token->type = TOKEN_ERROR;
        token->value[0] = c;
        token->value[1] = '\0';
        (*pos)++;
        (*column)++;
        return 0;
    }
    
    token->type = TOKEN_EOF;
    token->value[0] = '\0';
    token->line = *line;
    token