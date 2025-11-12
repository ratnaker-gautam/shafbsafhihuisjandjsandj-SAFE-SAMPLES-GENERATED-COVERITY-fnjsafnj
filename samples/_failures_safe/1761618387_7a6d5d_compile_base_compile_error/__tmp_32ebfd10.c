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
    int line = 1;
    int column = 1;
    int token_count = 0;
    int input_len = strlen(input);
    
    while (pos < input_len && token_count < max_tokens) {
        char current = input[pos];
        
        if (isspace(current)) {
            if (current == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
            continue;
        }
        
        if (isdigit(current)) {
            int start = pos;
            while (pos < input_len && (isdigit(input[pos]) || input[pos] == '.')) {
                pos++;
                column++;
            }
            int len = pos - start;
            if (len >= MAX_TOKEN_LEN) {
                len = MAX_TOKEN_LEN - 1;
            }
            strncpy(tokens[token_count].value, &input[start], len);
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            tokens[token_count].line = line;
            tokens[token_count].column = column - len;
            token_count++;
            continue;
        }
        
        if (isalpha(current) || current == '_') {
            int start = pos;
            while (pos < input_len && (isalnum(input[pos]) || input[pos] == '_')) {
                pos++;
                column++;
            }
            int len = pos - start;
            if (len >= MAX_TOKEN_LEN) {
                len = MAX_TOKEN_LEN - 1;
            }
            strncpy(tokens[token_count].value, &input[start], len);
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            tokens[token_count].line = line;
            tokens[token_count].column = column - len;
            token_count++;
            continue;
        }
        
        if (current == '"') {
            int start = pos;
            pos++;
            column++;
            while (pos < input_len && input[pos] != '"') {
                if (input[pos] == '\\' && pos + 1 < input_len) {
                    pos += 2;
                    column += 2;
                } else {
                    pos++;
                    column++;
                }
            }
            if (pos < input_len && input[pos] == '"') {
                pos++;
                column++;
            }
            int len = pos - start;
            if (len >= MAX_TOKEN_LEN) {
                len = MAX_TOKEN_LEN - 1;
            }
            strncpy(tokens[token_count].value, &input[start], len);
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_STRING;
            tokens[token_count].line = line;
            tokens[token_count].column = column - len;
            token_count++;
            continue;
        }
        
        if (is_operator_char(current)) {
            char op[MAX_TOKEN_LEN];
            int op_len = parse_operator(input, pos, op);
            strcpy(tokens[token_count].value, op);
            tokens[token_count].type = TOKEN_OPERATOR;
            tokens[token_count].line = line;
            tokens[token_count].column = column;
            token_count++;
            pos += op_len;
            column += op_len;
            continue;
        }
        
        tokens[token_count].value[0] = current;
        tokens[token_count].value[1] = '\0';
        tokens[token_count].type = TOKEN