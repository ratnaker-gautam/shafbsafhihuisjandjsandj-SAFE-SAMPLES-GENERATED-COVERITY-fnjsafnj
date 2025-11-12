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
    if (input == NULL || output == NULL) return 0;
    
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
    return len;
}

int parse_number(const char *input, int pos, char *output) {
    int len = 0;
    int has_dot = 0;
    if (input == NULL || output == NULL) return 0;
    
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
    return len;
}

int parse_identifier(const char *input, int pos, char *output) {
    int len = 0;
    if (input == NULL || output == NULL) return 0;
    
    if (isalpha(input[pos]) || input[pos] == '_') {
        if (len < 63) output[len++] = input[pos++];
        while ((isalnum(input[pos]) || input[pos] == '_') && len < 63) {
            output[len++] = input[pos++];
        }
    }
    
    output[len] = '\0';
    return len;
}

int parse_string(const char *input, int pos, char *output) {
    int len = 0;
    if (input == NULL || output == NULL) return 0;
    
    if (input[pos] == '"') {
        if (len < 63) output[len++] = input[pos++];
        while (input[pos] != '\0' && input[pos] != '"' && len < 63) {
            if (input[pos] == '\\' && input[pos + 1] != '\0') {
                if (len < 62) {
                    output[len++] = input[pos++];
                    output[len++] = input[pos++];
                } else {
                    break;
                }
            } else {
                output[len++] = input[pos++];
            }
        }
        if (input[pos] == '"' && len < 63) {
            output[len++] = input[pos];
        }
    }
    
    output[len] = '\0';
    return len;
}

int get_next_token(const char *input, int *pos, int line, int *column, struct Token *token) {
    if (input == NULL || pos == NULL || token == NULL) return 0;
    
    int start_col = *column;
    
    while (input[*pos] != '\0' && isspace(input[*pos])) {
        if (input[*pos] == '\n') {
            line++;
            *column = 1;
        } else {
            (*column)++;
        }
        (*pos)++;
    }
    
    if (input[*pos] == '\0') {
        token->type = TOKEN_EOF;
        strcpy(token->value, "");
        token->line = line;
        token->column = *column;
        return 1;
    }
    
    token->line = line;
    token->column = *column;
    
    int len = parse_number(input, *pos, token->value);
    if (len > 0) {
        token->type = TOKEN_NUMBER;
        *pos += len;
        *column += len;
        return 1;
    }
    
    len = parse_string(input, *pos, token->value);
    if (len > 0) {
        token->type = TOKEN_STRING;
        *pos += len;
        *column += len;
        return 1;
    }
    
    len = parse_operator(input, *pos, token->value);
    if (len > 0) {
        token->type = TOKEN_OPERATOR;
        *pos += len;
        *column += len;
        return 1;
    }
    
    len = parse_identifier(input, *pos, token->value);
    if (len > 0) {
        token->type = TOKEN_IDENTIFIER;
        *pos += len;
        *column += len;
        return 1;
    }
    
    token->type = TOKEN_UNKNOWN;