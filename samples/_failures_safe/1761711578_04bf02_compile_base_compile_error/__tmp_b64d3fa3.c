//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: parser_tokenizer
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
    char c = input[pos];
    
    if (is_operator_char(c)) {
        output[len++] = c;
        char next = input[pos + 1];
        
        if ((c == '=' && next == '=') ||
            (c == '!' && next == '=') ||
            (c == '<' && next == '=') ||
            (c == '>' && next == '=') ||
            (c == '&' && next == '&') ||
            (c == '|' && next == '|')) {
            output[len++] = next;
        }
    }
    
    output[len] = '\0';
    return len;
}

int parse_number(const char *input, int pos, char *output) {
    int len = 0;
    int has_dot = 0;
    
    while (input[pos] != '\0') {
        char c = input[pos];
        if (isdigit(c)) {
            output[len++] = c;
        } else if (c == '.' && !has_dot) {
            output[len++] = c;
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
    
    if (isalpha(input[pos]) || input[pos] == '_') {
        output[len++] = input[pos++];
        
        while (input[pos] != '\0' && 
               (isalnum(input[pos]) || input[pos] == '_')) {
            if (len < MAX_TOKEN_LEN - 1) {
                output[len++] = input[pos];
            }
            pos++;
        }
    }
    
    output[len] = '\0';
    return len;
}

int parse_string(const char *input, int pos, char *output) {
    int len = 0;
    
    if (input[pos] == '"') {
        output[len++] = input[pos++];
        
        while (input[pos] != '\0' && input[pos] != '"') {
            if (len < MAX_TOKEN_LEN - 1) {
                output[len++] = input[pos];
            }
            pos++;
        }
        
        if (input[pos] == '"') {
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
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        char c = input[pos];
        
        if (isspace(c)) {
            if (c == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
            continue;
        }
        
        tokens[token_count].line = line;
        tokens[token_count].column = column;
        
        if (isdigit(c)) {
            int len = parse_number(input, pos, tokens[token_count].value);
            if (len > 0) {
                tokens[token_count].type = TOKEN_NUMBER;
                pos += len;
                column += len;
                token_count++;
                continue;
            }
        }
        
        if (isalpha(c) || c == '_') {
            int len = parse_identifier(input, pos, tokens[token_count].value);
            if (len > 0) {
                tokens[token_count].type = TOKEN_IDENTIFIER;
                pos += len;
                column += len;
                token_count++;
                continue;
            }
        }
        
        if (c == '"') {
            int len = parse_string(input, pos, tokens[token_count].value);
            if (len > 0) {
                tokens[token_count].type = TOKEN_STRING;
                pos += len;
                column += len;
                token_count++;
                continue;
            }
        }
        
        if (is_operator_char(c)) {
            int len = parse_operator(input, pos, tokens[token_count].value);
            if (len > 0) {
                tokens[token_count].type = TOKEN_OPERATOR;
                pos += len;
                column += len;
                token_count++;
                continue;
            }
        }
        
        tokens[token_count].type = TOKEN_INVALID;
        tokens[token_count].value[0] = c;
        tokens[token_count].value[1] = '\0';
        pos++;
        column++;
        token_count++;
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "EOF");
        tokens[