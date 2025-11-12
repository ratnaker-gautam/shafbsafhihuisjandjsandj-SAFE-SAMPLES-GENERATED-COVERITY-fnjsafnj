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
    } else if (is_operator_char(c)) {
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
        
        char current = input[pos];
        struct Token *token = &tokens[token_count];
        token->line = line;
        token->column = column;
        
        if (isdigit(current)) {
            int len = 0;
            while (pos < input_len && len < MAX_TOKEN_LEN - 1 && 
                   (isdigit(input[pos]) || input[pos] == '.')) {
                token->value[len++] = input[pos++];
                column++;
            }
            token->value[len] = '\0';
            token->type = TOKEN_NUMBER;
            token_count++;
        } else if (isalpha(current) || current == '_') {
            int len = 0;
            while (pos < input_len && len < MAX_TOKEN_LEN - 1 && 
                   (isalnum(input[pos]) || input[pos] == '_')) {
                token->value[len++] = input[pos++];
                column++;
            }
            token->value[len] = '\0';
            token->type = TOKEN_IDENTIFIER;
            token_count++;
        } else if (current == '"') {
            int len = 0;
            token->value[len++] = input[pos++];
            column++;
            
            while (pos < input_len && len < MAX_TOKEN_LEN - 1 && input[pos] != '"') {
                token->value[len++] = input[pos++];
                column++;
            }
            
            if (pos < input_len && input[pos] == '"') {
                token->value[len++] = input[pos++];
                column++;
            }
            
            token->value[len] = '\0';
            token->type = TOKEN_STRING;
            token_count++;
        } else if (is_operator_char(current)) {
            int op_len = parse_operator(input, pos, token->value);
            if (op_len > 0) {
                pos += op_len;
                column += op_len;
                token->type = TOKEN_OPERATOR;
                token_count++;
            } else {
                token->value[0] = current;
                token->value[1] = '\0';
                token->type = TOKEN_UNKNOWN;
                token_count++;
                pos++;
                column++;
            }
        } else {
            token->value[0] = current;
            token->value[1] = '\0';
            token->type = TOKEN_UNKNOWN;
            token_count++;
            pos++;
            column++;
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

const char *token_type_name(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";