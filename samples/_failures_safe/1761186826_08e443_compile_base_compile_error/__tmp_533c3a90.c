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
        if (isspace(input[pos])) {
            if (input[pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
            continue;
        }
        
        if (isdigit(input[pos])) {
            int start = pos;
            while (isdigit(input[pos])) {
                pos++;
                column++;
            }
            int len = pos - start;
            if (len >= MAX_TOKEN_LEN) {
                tokens[token_count].type = TOKEN_ERROR;
                strcpy(tokens[token_count].value, "NUMBER_TOO_LONG");
                tokens[token_count].line = line;
                tokens[token_count].column = column - len;
                token_count++;
                break;
            }
            strncpy(tokens[token_count].value, &input[start], len);
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            tokens[token_count].line = line;
            tokens[token_count].column = column - len;
            token_count++;
            continue;
        }
        
        if (isalpha(input[pos]) || input[pos] == '_') {
            int start = pos;
            while (isalnum(input[pos]) || input[pos] == '_') {
                pos++;
                column++;
            }
            int len = pos - start;
            if (len >= MAX_TOKEN_LEN) {
                tokens[token_count].type = TOKEN_ERROR;
                strcpy(tokens[token_count].value, "IDENTIFIER_TOO_LONG");
                tokens[token_count].line = line;
                tokens[token_count].column = column - len;
                token_count++;
                break;
            }
            strncpy(tokens[token_count].value, &input[start], len);
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            tokens[token_count].line = line;
            tokens[token_count].column = column - len;
            token_count++;
            continue;
        }
        
        if (input[pos] == '"') {
            int start = pos;
            pos++;
            column++;
            while (input[pos] != '"' && input[pos] != '\0') {
                pos++;
                column++;
            }
            if (input[pos] == '\0') {
                tokens[token_count].type = TOKEN_ERROR;
                strcpy(tokens[token_count].value, "UNTERMINATED_STRING");
                tokens[token_count].line = line;
                tokens[token_count].column = column;
                token_count++;
                break;
            }
            pos++;
            column++;
            int len = pos - start - 2;
            if (len >= MAX_TOKEN_LEN) {
                tokens[token_count].type = TOKEN_ERROR;
                strcpy(tokens[token_count].value, "STRING_TOO_LONG");
                tokens[token_count].line = line;
                tokens[token_count].column = column - len - 2;
                token_count++;
                break;
            }
            strncpy(tokens[token_count].value, &input[start + 1], len);
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_STRING;
            tokens[token_count].line = line;
            tokens[token_count].column =