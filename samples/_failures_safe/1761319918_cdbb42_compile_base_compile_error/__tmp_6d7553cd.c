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
    } else if (is_operator_char(c)) {
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
    
    while (input[pos] != '\0' && token_count < max_tokens - 1) {
        while (isspace((unsigned char)input[pos])) {
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
        
        if (isdigit((unsigned char)input[pos])) {
            int start = pos;
            while (isdigit((unsigned char)input[pos])) {
                pos++;
                column++;
            }
            int len = pos - start;
            if (len < 64) {
                strncpy(tokens[token_count].value, &input[start], len);
                tokens[token_count].value[len] = '\0';
                tokens[token_count].type = TOKEN_NUMBER;
                token_count++;
            }
        } else if (isalpha((unsigned char)input[pos]) || input[pos] == '_') {
            int start = pos;
            while (isalnum((unsigned char)input[pos]) || input[pos] == '_') {
                pos++;
                column++;
            }
            int len = pos - start;
            if (len < 64) {
                strncpy(tokens[token_count].value, &input[start], len);
                tokens[token_count].value[len] = '\0';
                tokens[token_count].type = TOKEN_IDENTIFIER;
                token_count++;
            }
        } else if (input[pos] == '"') {
            int start = pos;
            pos++;
            column++;
            while (input[pos] != '"' && input[pos] != '\0') {
                if (input[pos] == '\\' && input[pos + 1] != '\0') {
                    pos += 2;
                    column += 2;
                } else {
                    pos++;
                    column++;
                }
            }
            if (input[pos] == '"') {
                pos++;
                column++;
                int len = pos - start;
                if (len < 64) {
                    strncpy(tokens[token_count].value, &input[start], len);
                    tokens[token_count].value[len] = '\0';
                    tokens[token_count].type = TOKEN_STRING;
                    token_count++;
                }
            }
        } else if (is_operator_char(input[pos])) {
            char op[3] = {0};
            int len = parse_operator(input, pos, op);
            if (len > 0) {
                strcpy(tokens[token_count].value, op);
                tokens[token_count].type = TOKEN_OPERATOR;
                token_count++;
                pos += len;
                column += len;
            } else {
                tokens[token_count].value[0] = input[pos];
                tokens[token_count].value[1] = '\0';
                tokens[token_count].type = TOKEN_UNKNOWN;
                token_count++;
                pos++;
                column++;
            }
        } else {
            tokens[token_count].value[0] = input[pos];
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = TOKEN_UNKNOWN;
            token_count++;
            pos++;
            column++;
        }
    }
    
    tokens[token_count].type = TOKEN_EOF;
    tokens[token_count].value[0] = '\0';
    tokens[token_count