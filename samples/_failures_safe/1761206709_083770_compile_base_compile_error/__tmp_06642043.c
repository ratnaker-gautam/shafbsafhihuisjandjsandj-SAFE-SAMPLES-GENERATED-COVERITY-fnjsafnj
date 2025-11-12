//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

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
    return c == '+' || c == '-' || c == '*' || c == '/' || 
           c == '=' || c == '<' || c == '>' || c == '!' ||
           c == '&' || c == '|' || c == '^' || c == '%' ||
           c == '(' || c == ')' || c == '{' || c == '}' ||
           c == '[' || c == ']' || c == ',' || c == ';' ||
           c == '.' || c == ':';
}

int is_valid_identifier_start(char c) {
    return isalpha(c) || c == '_';
}

int is_valid_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

const char* token_type_to_string(enum TokenType type) {
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

int tokenize(const char* input, struct Token* tokens, int max_tokens) {
    if (input == NULL || tokens == NULL || max_tokens <= 0) {
        return 0;
    }
    
    int token_count = 0;
    int pos = 0;
    int line = 1;
    int column = 1;
    int input_len = strlen(input);
    
    while (pos < input_len && token_count < max_tokens) {
        while (pos < input_len && isspace(input[pos])) {
            if (input[pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
        }
        
        if (pos >= input_len) break;
        
        int start_pos = pos;
        int start_column = column;
        
        if (isdigit(input[pos])) {
            while (pos < input_len && (isdigit(input[pos]) || input[pos] == '.')) {
                if (pos - start_pos >= MAX_TOKEN_LEN - 1) break;
                pos++;
                column++;
            }
            tokens[token_count].type = TOKEN_NUMBER;
            int copy_len = pos - start_pos;
            if (copy_len >= MAX_TOKEN_LEN) copy_len = MAX_TOKEN_LEN - 1;
            strncpy(tokens[token_count].value, &input[start_pos], copy_len);
            tokens[token_count].value[copy_len] = '\0';
            tokens[token_count].line = line;
            tokens[token_count].column = start_column;
            token_count++;
            continue;
        }
        
        if (is_valid_identifier_start(input[pos])) {
            while (pos < input_len && is_valid_identifier_char(input[pos])) {
                if (pos - start_pos >= MAX_TOKEN_LEN - 1) break;
                pos++;
                column++;
            }
            tokens[token_count].type = TOKEN_IDENTIFIER;
            int copy_len = pos - start_pos;
            if (copy_len >= MAX_TOKEN_LEN) copy_len = MAX_TOKEN_LEN - 1;
            strncpy(tokens[token_count].value, &input[start_pos], copy_len);
            tokens[token_count].value[copy_len] = '\0';
            tokens[token_count].line = line;
            tokens[token_count].column = start_column;
            token_count++;
            continue;
        }
        
        if (input[pos] == '"') {
            pos++;
            column++;
            while (pos < input_len && input[pos] != '"') {
                if (pos - start_pos >= MAX_TOKEN_LEN - 1) break;
                if (input[pos] == '\n') {
                    line++;
                    column = 1;
                } else {
                    column++;
                }
                pos++;
            }
            if (pos < input_len && input[pos] == '"') {
                pos++;
                column++;
            }
            tokens[token_count].type = TOKEN_STRING;
            int copy_len = pos - start_pos;
            if (copy_len >= MAX_TOKEN_LEN) copy_len = MAX_TOKEN_LEN - 1;
            strncpy(tokens[token_count].value, &input[start_pos], copy_len);
            tokens[token_count].value[copy_len] = '\0';
            tokens[token_count].line = line;
            tokens[token_count].column = start_column;
            token_count++;
            continue;
        }
        
        if (is_operator_char(input[pos])) {
            while (pos < input_len && is_operator_char(input[pos])) {
                if (pos - start_pos >= MAX_TOKEN_LEN - 1) break;
                pos++;
                column++;
            }
            tokens[token_count].type = TOKEN_OPERATOR;
            int copy_len = pos - start_pos;
            if (copy_len >= MAX_TOKEN_LEN) copy_len =