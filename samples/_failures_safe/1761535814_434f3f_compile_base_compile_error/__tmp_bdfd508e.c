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

int parse_number(const char *input, int pos, char *output) {
    int len = 0;
    int has_dot = 0;
    
    while (input[pos + len] && (isdigit(input[pos + len]) || input[pos + len] == '.')) {
        if (input[pos + len] == '.') {
            if (has_dot) break;
            has_dot = 1;
        }
        if (len < 63) {
            output[len] = input[pos + len];
            len++;
        } else {
            break;
        }
    }
    
    if (len > 0) {
        output[len] = '\0';
    }
    
    return len;
}

int parse_identifier(const char *input, int pos, char *output) {
    int len = 0;
    
    if (input[pos] && (isalpha(input[pos]) || input[pos] == '_')) {
        while (input[pos + len] && (isalnum(input[pos + len]) || input[pos + len] == '_')) {
            if (len < 63) {
                output[len] = input[pos + len];
                len++;
            } else {
                break;
            }
        }
    }
    
    if (len > 0) {
        output[len] = '\0';
    }
    
    return len;
}

int parse_string(const char *input, int pos, char *output) {
    int len = 0;
    
    if (input[pos] == '"') {
        output[len++] = '"';
        pos++;
        
        while (input[pos + len - 1] && input[pos + len - 1] != '"') {
            if (input[pos + len - 1] == '\\') {
                if (len < 62) {
                    output[len++] = input[pos + len - 1];
                    if (input[pos + len]) {
                        output[len++] = input[pos + len];
                    }
                } else {
                    break;
                }
            } else {
                if (len < 63) {
                    output[len++] = input[pos + len - 1];
                } else {
                    break;
                }
            }
        }
        
        if (input[pos + len - 1] == '"') {
            output[len] = '\0';
        } else {
            len = 0;
        }
    }
    
    return len;
}

const char *token_type_name(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_UNKNOWN: return "UNKNOWN";
        default: return "INVALID";
    }
}

void tokenize(const char *input, struct Token *tokens, int max_tokens, int *token_count) {
    int pos = 0;
    int line = 1;
    int column = 1;
    *token_count = 0;
    
    while (input[pos] && *token_count < max_tokens - 1) {
        while (isspace(input[pos])) {
            if (input[pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
        }
        
        if (!input[pos]) break;
        
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
            if (input[pos]) {
                token->value[0] = input[pos];
                token->value[1] = '\0';
                advance = 1;
            } else {
                break;
            }
        }
        
        if (advance > 0) {
            column += advance;
            pos += advance;
            (*token_count