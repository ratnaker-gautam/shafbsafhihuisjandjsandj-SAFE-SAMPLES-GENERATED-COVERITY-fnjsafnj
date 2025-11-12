//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

enum TokenType {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
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
        return 2;
    } else if (c == '|' && input[pos + 1] == '|') {
        strcpy(output, "||");
        return 2;
    } else if (c == '=' && input[pos + 1] == '=') {
        strcpy(output, "==");
        return 2;
    } else if (c == '!' && input[pos + 1] == '=') {
        strcpy(output, "!=");
        return 2;
    } else if (c == '<' && input[pos + 1] == '=') {
        strcpy(output, "<=");
        return 2;
    } else if (c == '>' && input[pos + 1] == '=') {
        strcpy(output, ">=");
        return 2;
    } else if (is_operator_char(c)) {
        output[0] = c;
        output[1] = '\0';
        return 1;
    }
    return 0;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int line = 1;
    int column = 1;
    int token_count = 0;
    int len = strlen(input);
    
    while (pos < len && token_count < max_tokens) {
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
        
        if (isalpha(c) || c == '_') {
            int start = pos;
            while (pos < len && (isalnum(input[pos]) || input[pos] == '_')) {
                pos++;
            }
            int token_len = pos - start;
            if (token_len >= MAX_TOKEN_LEN) {
                token_len = MAX_TOKEN_LEN - 1;
            }
            strncpy(tokens[token_count].value, &input[start], token_len);
            tokens[token_count].value[token_len] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            tokens[token_count].line = line;
            tokens[token_count].column = column;
            column += token_len;
            token_count++;
            continue;
        }
        
        if (isdigit(c)) {
            int start = pos;
            while (pos < len && isdigit(input[pos])) {
                pos++;
            }
            int token_len = pos - start;
            if (token_len >= MAX_TOKEN_LEN) {
                token_len = MAX_TOKEN_LEN - 1;
            }
            strncpy(tokens[token_count].value, &input[start], token_len);
            tokens[token_count].value[token_len] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            tokens[token_count].line = line;
            tokens[token_count].column = column;
            column += token_len;
            token_count++;
            continue;
        }
        
        if (c == '"') {
            int start = pos;
            pos++;
            column++;
            while (pos < len && input[pos] != '"') {
                if (input[pos] == '\\' && pos + 1 < len) {
                    pos += 2;
                    column += 2;
                } else {
                    pos++;
                    column++;
                }
            }
            if (pos < len && input[pos] == '"') {
                pos++;
                column++;
            }
            int token_len = pos - start;
            if (token_len >= MAX_TOKEN_LEN) {
                token_len = MAX_TOKEN_LEN - 1;
            }
            strncpy(tokens[token_count].value, &input[start], token_len);
            tokens[token_count].value[token_len] = '\0';
            tokens[token_count].type = TOKEN_STRING;
            tokens[token_count].line = line;
            tokens[token_count].column = column - token_len;
            token_count++;
            continue;
        }
        
        char operator_buf[3] = {0};
        int op_len = parse_operator(input, pos, operator_buf);
        if (op_len > 0) {
            strcpy(tokens[token_count].value, operator_buf);
            tokens[token_count].type = TOKEN_OPERATOR;
            tokens[token_count].line = line;
            tokens[token_count].column = column;
            column += op_len;
            pos += op_len;
            token_count++;
            continue;
        }
        
        tokens[token_count].value[0] = c;
        tokens[token_count].value[