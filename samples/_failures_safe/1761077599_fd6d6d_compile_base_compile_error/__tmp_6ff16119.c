//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
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
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char value[32];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int get_operator_length(const char *str) {
    if (!str || strlen(str) < 1) return 0;
    
    if (str[0] == '&' && str[1] == '&') return 2;
    if (str[0] == '|' && str[1] == '|') return 2;
    if (str[0] == '=' && str[1] == '=') return 2;
    if (str[0] == '!' && str[1] == '=') return 2;
    if (str[0] == '<' && str[1] == '=') return 2;
    if (str[0] == '>' && str[1] == '=') return 2;
    
    return 1;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    if (!input || !tokens || max_tokens <= 0) return -1;
    
    int token_count = 0;
    int line = 1;
    int column = 1;
    int i = 0;
    int input_len = (int)strlen(input);
    
    while (i < input_len && token_count < max_tokens) {
        while (i < input_len && isspace(input[i])) {
            if (input[i] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            i++;
        }
        
        if (i >= input_len) break;
        
        struct Token *token = &tokens[token_count];
        token->line = line;
        token->column = column;
        token->value[0] = '\0';
        
        if (isdigit(input[i])) {
            token->type = TOKEN_NUMBER;
            int j = 0;
            while (i < input_len && j < 31 && isdigit(input[i])) {
                token->value[j++] = input[i++];
                column++;
            }
            token->value[j] = '\0';
            token_count++;
        } else if (isalpha(input[i]) || input[i] == '_') {
            token->type = TOKEN_IDENTIFIER;
            int j = 0;
            while (i < input_len && j < 31 && (isalnum(input[i]) || input[i] == '_')) {
                token->value[j++] = input[i++];
                column++;
            }
            token->value[j] = '\0';
            token_count++;
        } else if (input[i] == '"') {
            token->type = TOKEN_STRING;
            int j = 0;
            i++;
            column++;
            while (i < input_len && j < 31 && input[i] != '"') {
                token->value[j++] = input[i++];
                column++;
            }
            if (i < input_len && input[i] == '"') {
                i++;
                column++;
            }
            token->value[j] = '\0';
            token_count++;
        } else if (is_operator_char(input[i])) {
            token->type = TOKEN_OPERATOR;
            int op_len = get_operator_length(&input[i]);
            if (op_len > 0 && op_len <= 2) {
                strncpy(token->value, &input[i], op_len);
                token->value[op_len] = '\0';
                i += op_len;
                column += op_len;
                token_count++;
            } else {
                token->type = TOKEN_INVALID;
                token->value[0] = input[i++];
                token->value[1] = '\0';
                column++;
                token_count++;
            }
        } else {
            token->type = TOKEN_INVALID;
            token->value[0] = input[i++];
            token->value[1] = '\0';
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
    
    return token_count;
}

void print_token(const struct Token *token) {
    if (!token) return;
    
    switch (token->type) {
        case TOKEN_NUMBER:
            printf("NUMBER: %s", token->value);
            break;
        case TOKEN_IDENTIFIER:
            printf("IDENTIFIER: %s", token->value);
            break;
        case TOKEN_OPERATOR:
            printf("OPERATOR: %s", token->value);
            break;
        case TOKEN_STRING:
            printf("STRING: \"%s\"", token->value);
            break;
        case TOKEN_EOF:
            printf("EOF");
            break;