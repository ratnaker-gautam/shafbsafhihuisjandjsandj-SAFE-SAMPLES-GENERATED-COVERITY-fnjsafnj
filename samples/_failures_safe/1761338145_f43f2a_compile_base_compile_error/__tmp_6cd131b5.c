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
    TOKEN_ERROR
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
    int token_count = 0;
    int line = 1;
    int column = 1;
    
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
                    len++;
                } else {
                    break;
                }
            }
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
                    len++;
                } else {
                    break;
                }
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            pos += len;
            column += len;
            token_count++;
        } else if (input[pos] == '"') {
            int len = 1;
            tokens[token_count].value[0] = '"';
            pos++;
            column++;
            
            while (input[pos] != '"' && input[pos] != '\0') {
                if (len < MAX_TOKEN_LEN - 2) {
                    tokens[token_count].value[len] = input[pos];
                    len++;
                }
                pos++;
                column++;
            }
            
            if (input[pos] == '"') {
                if (len < MAX_TOKEN_LEN - 1) {
                    tokens[token_count].value[len] = '"';
                    len++;
                }
                tokens[token_count].value[len] = '\0';
                tokens[token_count].type = TOKEN_STRING;
                pos++;
                column++;
                token_count++;
            } else {
                tokens[token_count].value[0] = '\0';
                tokens[token_count].type = TOKEN_ERROR;
                token_count++;
                break;
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
            tokens[token_count].type = TOKEN_ERROR;
            pos++;
            column++;
            token_count++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        tokens[token_count].line = line;
        tokens[token_count].column = column;
        token_count++;
    }
    
    return token