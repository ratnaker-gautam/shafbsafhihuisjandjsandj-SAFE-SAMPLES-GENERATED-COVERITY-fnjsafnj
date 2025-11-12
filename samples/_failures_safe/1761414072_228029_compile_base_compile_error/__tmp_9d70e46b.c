//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

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
    char value[MAX_TOKEN_LEN];
    int line;
    int column;
};

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int parse_operator(const char *input, int pos, char *output) {
    int len = 0;
    if (pos < 0 || !input || !output) return 0;
    
    char c = input[pos];
    
    if (is_operator_char(c)) {
        output[len++] = c;
        if (len >= MAX_TOKEN_LEN - 1) {
            output[len] = '\0';
            return len;
        }
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
    if (pos < 0 || !input || !output) return 0;
    
    while (input[pos] != '\0' && len < MAX_TOKEN_LEN - 1 && 
           (isdigit(input[pos]) || (input[pos] == '.' && !has_dot))) {
        if (input[pos] == '.') has_dot = 1;
        output[len++] = input[pos++];
    }
    
    output[len] = '\0';
    return len;
}

int parse_identifier(const char *input, int pos, char *output) {
    int len = 0;
    if (pos < 0 || !input || !output) return 0;
    
    if (isalpha(input[pos]) || input[pos] == '_') {
        output[len++] = input[pos++];
        while (input[pos] != '\0' && len < MAX_TOKEN_LEN - 1 && 
               (isalnum(input[pos]) || input[pos] == '_')) {
            output[len++] = input[pos++];
        }
    }
    
    output[len] = '\0';
    return len;
}

int parse_string(const char *input, int pos, char *output) {
    int len = 0;
    if (pos < 0 || !input || !output) return 0;
    
    if (input[pos] == '"') {
        output[len++] = input[pos++];
        while (input[pos] != '\0' && input[pos] != '"' && len < MAX_TOKEN_LEN - 1) {
            if (input[pos] == '\\' && input[pos + 1] != '\0') {
                if (len < MAX_TOKEN_LEN - 2) {
                    output[len++] = input[pos++];
                    output[len++] = input[pos++];
                } else {
                    break;
                }
            } else {
                output[len++] = input[pos++];
            }
        }
        if (input[pos] == '"' && len < MAX_TOKEN_LEN - 1) {
            output[len++] = input[pos];
        }
    }
    
    output[len] = '\0';
    return len;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int line = 1;
    int column = 1;
    int token_count = 0;
    
    if (!input || !tokens || max_tokens <= 0) return 0;
    
    while (input[pos] != '\0' && token_count < max_tokens) {
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
        
        int start_column = column;
        char buffer[MAX_TOKEN_LEN];
        int advance = 0;
        enum TokenType type = TOKEN_INVALID;
        
        if (isdigit(input[pos]) || (input[pos] == '.' && isdigit(input[pos + 1]))) {
            advance = parse_number(input, pos, buffer);
            if (advance > 0) type = TOKEN_NUMBER;
        } else if (isalpha(input[pos]) || input[pos] == '_') {
            advance = parse_identifier(input, pos, buffer);
            if (advance > 0) type = TOKEN_IDENTIFIER;
        } else if (is_operator_char(input[pos])) {
            advance = parse_operator(input, pos, buffer);
            if (advance > 0) type = TOKEN_OPERATOR;
        } else if (input[pos] == '"') {
            advance = parse_string(input, pos, buffer);
            if (advance > 0 && buffer[advance - 1] == '"') {
                type =