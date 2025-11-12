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
    TOKEN_UNKNOWN
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
    
    if (c == '&' && input[pos + 1] == '&') {
        strcpy(output, "&&");
        len = 2;
    } else if (c == '|' && input[pos + 1] == '|') {
        strcpy(output, "||");
        len = 2;
    } else if (c == '=' && input[pos + 1] == '=') {
        strcpy(output, "==");
        len = 2;
    } else if (c == '!' && input[pos + 1] == '=') {
        strcpy(output, "!=");
        len = 2;
    } else if (c == '<' && input[pos + 1] == '=') {
        strcpy(output, "<=");
        len = 2;
    } else if (c == '>' && input[pos + 1] == '=') {
        strcpy(output, ">=");
        len = 2;
    } else {
        output[0] = c;
        output[1] = '\0';
        len = 1;
    }
    
    return len;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int line = 1;
    int column = 1;
    int token_count = 0;
    
    if (input == NULL || tokens == NULL || max_tokens <= 0) {
        return 0;
    }
    
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
        
        tokens[token_count].line = line;
        tokens[token_count].column = column;
        
        if (isdigit(input[pos])) {
            int len = 0;
            while (isdigit(input[pos + len])) {
                if (len < MAX_TOKEN_LEN - 1) {
                    tokens[token_count].value[len] = input[pos + len];
                }
                len++;
            }
            if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            pos += len;
            column += len;
            token_count++;
        } else if (isalpha(input[pos]) || input[pos] == '_') {
            int len = 0;
            while (isalnum(input[pos + len]) || input[pos + len] == '_') {
                if (len < MAX_TOKEN_LEN - 1) {
                    tokens[token_count].value[len] = input[pos + len];
                }
                len++;
            }
            if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            pos += len;
            column += len;
            token_count++;
        } else if (input[pos] == '"') {
            int len = 1;
            tokens[token_count].value[0] = '"';
            while (input[pos + len] != '"' && input[pos + len] != '\0') {
                if (len < MAX_TOKEN_LEN - 2) {
                    tokens[token_count].value[len] = input[pos + len];
                }
                len++;
            }
            if (input[pos + len] == '"') {
                if (len < MAX_TOKEN_LEN - 1) {
                    tokens[token_count].value[len] = '"';
                    tokens[token_count].value[len + 1] = '\0';
                } else {
                    tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
                }
                tokens[token_count].type = TOKEN_STRING;
                pos += len + 1;
                column += len + 1;
                token_count++;
            } else {
                tokens[token_count].value[1] = '\0';
                tokens[token_count].type = TOKEN_UNKNOWN;
                pos++;
                column++;
                token_count++;
            }
        } else if (is_operator_char(input[pos])) {
            int len = parse_operator(input, pos, tokens[token_count].value);
            tokens[token_count].type = TOKEN_OPERATOR;
            pos += len;
            column += len;
            token_count++;
        } else {
            tokens[token_count].value[0] = input[pos];
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = TOKEN