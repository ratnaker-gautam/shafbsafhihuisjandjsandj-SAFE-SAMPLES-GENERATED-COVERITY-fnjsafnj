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
    
    while (isdigit(input[pos]) || (input[pos] == '.' && !has_dot)) {
        if (input[pos] == '.') has_dot = 1;
        if (len < 63) output[len++] = input[pos++];
        else break;
    }
    
    output[len] = '\0';
    return len > 0 ? len : 0;
}

int parse_identifier(const char *input, int pos, char *output) {
    int len = 0;
    
    if (isalpha(input[pos]) || input[pos] == '_') {
        while (isalnum(input[pos]) || input[pos] == '_') {
            if (len < 63) output[len++] = input[pos++];
            else break;
        }
    }
    
    output[len] = '\0';
    return len > 0 ? len : 0;
}

int parse_string(const char *input, int pos, char *output) {
    int len = 0;
    
    if (input[pos] == '"') {
        if (len < 63) output[len++] = input[pos++];
        
        while (input[pos] != '"' && input[pos] != '\0') {
            if (input[pos] == '\\' && input[pos + 1] != '\0') {
                if (len < 62) {
                    output[len++] = input[pos++];
                    output[len++] = input[pos++];
                } else break;
            } else {
                if (len < 63) output[len++] = input[pos++];
                else break;
            }
        }
        
        if (input[pos] == '"' && len < 63) {
            output[len++] = input[pos];
        }
    }
    
    output[len] = '\0';
    return len > 1 ? len : 0;
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
        
        tokens[*token_count].line = line;
        tokens[*token_count].column = column;
        
        char buffer[64];
        int advance = 0;
        
        if ((advance = parse_number(input, pos, buffer)) > 0) {
            tokens[*token_count].type = TOKEN_NUMBER;
            strncpy(tokens[*token_count].value, buffer, 63);
            tokens[*token_count].value[63] = '\0';
        } else if ((advance = parse_string(input, pos, buffer)) > 0) {
            tokens[*token_count].type = TOKEN_STRING;
            strncpy(tokens[*token_count].value, buffer, 63);
            tokens[*token_count].value[63] = '\0';
        } else if ((advance = parse_identifier(input, pos, buffer)) > 0) {
            tokens[*token_count].type = TOKEN_IDENTIFIER;
            strncpy(tokens[*token_count].value, buffer, 63);
            tokens[*token_count].value[63] = '\0';
        } else if ((advance = parse_operator(input, pos, buffer)) > 0) {
            tokens[*token_count].type = TOKEN_OPERATOR;
            strncpy(tokens[*token_count].value, buffer, 63);
            tokens[*token_count].value[63] = '\0';
        } else {
            tokens[*token_count].type = TOKEN_UNKNOWN;
            tokens[*token_count].value[0] = input[pos];
            tokens[*token_count].value[1] = '\0';
            advance = 1;
        }
        
        column += advance;