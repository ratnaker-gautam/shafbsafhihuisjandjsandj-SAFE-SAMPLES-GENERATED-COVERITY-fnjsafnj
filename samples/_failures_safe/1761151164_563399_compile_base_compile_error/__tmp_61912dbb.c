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

int get_operator_length(const char *str) {
    if (str[0] == '\0') return 0;
    if (str[0] == '&' && str[1] == '&') return 2;
    if (str[0] == '|' && str[1] == '|') return 2;
    if (str[0] == '=' && str[1] == '=') return 2;
    if (str[0] == '!' && str[1] == '=') return 2;
    if (str[0] == '<' && str[1] == '=') return 2;
    if (str[0] == '>' && str[1] == '=') return 2;
    return 1;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int token_count = 0;
    int line = 1;
    int column = 1;
    int i = 0;
    
    if (input == NULL || tokens == NULL || max_tokens <= 0) {
        return 0;
    }
    
    while (input[i] != '\0' && token_count < max_tokens - 1) {
        while (isspace((unsigned char)input[i])) {
            if (input[i] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            i++;
        }
        
        if (input[i] == '\0') break;
        
        tokens[token_count].line = line;
        tokens[token_count].column = column;
        
        if (isdigit((unsigned char)input[i])) {
            int j = 0;
            while (isdigit((unsigned char)input[i]) && j < 63) {
                tokens[token_count].value[j++] = input[i++];
                column++;
            }
            tokens[token_count].value[j] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
        } else if (isalpha((unsigned char)input[i]) || input[i] == '_') {
            int j = 0;
            while ((isalnum((unsigned char)input[i]) || input[i] == '_') && j < 63) {
                tokens[token_count].value[j++] = input[i++];
                column++;
            }
            tokens[token_count].value[j] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            token_count++;
        } else if (input[i] == '"') {
            int j = 0;
            tokens[token_count].value[j++] = input[i++];
            column++;
            while (input[i] != '"' && input[i] != '\0' && j < 63) {
                if (input[i] == '\\' && input[i+1] != '\0') {
                    tokens[token_count].value[j++] = input[i++];
                    tokens[token_count].value[j++] = input[i++];
                    column += 2;
                } else {
                    tokens[token_count].value[j++] = input[i++];
                    column++;
                }
            }
            if (input[i] == '"') {
                tokens[token_count].value[j++] = input[i++];
                column++;
            }
            tokens[token_count].value[j] = '\0';
            tokens[token_count].type = TOKEN_STRING;
            token_count++;
        } else if (is_operator_char(input[i])) {
            int len = get_operator_length(&input[i]);
            if (len > 0 && len < 64) {
                strncpy(tokens[token_count].value, &input[i], len);
                tokens[token_count].value[len] = '\0';
                i += len;
                column += len;
                tokens[token_count].type = TOKEN_OPERATOR;
                token_count++;
            } else {
                tokens[token_count].value[0] = input[i++];
                tokens[token_count].value[1] = '\0';
                column++;
                tokens[token_count].type = TOKEN_UNKNOWN;
                token_count++;
            }
        } else {
            tokens[token_count].value[0] = input[i++];
            tokens[token_count].value[1] = '\0';
            column++;
            tokens[token_count].type = TOKEN_UNKNOWN;
            token_count++;
        }
    }
    
    tokens[token_count].type = TOKEN_EOF;
    tokens[token_count].value[0] = '\0';
    tokens[token_count].line = line;
    tokens[token_count].column = column;
    
    return token_count + 1;
}

const char* token_type_to_string(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "