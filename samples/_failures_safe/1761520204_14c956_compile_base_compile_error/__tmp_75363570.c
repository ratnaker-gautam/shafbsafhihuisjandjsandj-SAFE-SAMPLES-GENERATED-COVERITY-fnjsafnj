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
        
        struct Token *token = &tokens[token_count];
        token->line = line;
        token->column = column;
        
        if (isdigit(c)) {
            int len = 0;
            while (pos < input_len && len < MAX_TOKEN_LEN - 1 && isdigit(input[pos])) {
                token->value[len++] = input[pos++];
                column++;
            }
            token->value[len] = '\0';
            token->type = TOKEN_NUMBER;
            token_count++;
        } else if (isalpha(c) || c == '_') {
            int len = 0;
            while (pos < input_len && len < MAX_TOKEN_LEN - 1 && 
                   (isalnum(input[pos]) || input[pos] == '_')) {
                token->value[len++] = input[pos++];
                column++;
            }
            token->value[len] = '\0';
            token->type = TOKEN_IDENTIFIER;
            token_count++;
        } else if (is_operator_char(c)) {
            int len = parse_operator(input, pos, token->value);
            pos += len;
            column += len;
            token->type = TOKEN_OPERATOR;
            token_count++;
        } else if (c == '"') {
            int len = 0;
            pos++;
            column++;
            token->value[len++] = '"';
            
            while (pos < input_len && len < MAX_TOKEN_LEN - 2 && input[pos] != '"') {
                if (input[pos] == '\\' && pos + 1 < input_len) {
                    token->value[len++] = input[pos++];
                    column++;
                }
                token->value[len++] = input[pos++];
                column++;
            }
            
            if (pos < input_len && input[pos] == '"') {
                token->value[len++] = '"';
                pos++;
                column++;
                token->value[len] = '\0';
                token->type = TOKEN_STRING;
                token_count++;
            } else {
                token->value[len] = '\0';
                token->type = TOKEN_ERROR;
                token_count++;
                break;
            }
        } else {
            token->value[0] = c;
            token->value[1] = '\0';
            token->type = TOKEN_ERROR;
            token_count++;
            pos++;
            column++;
            break;
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

void print_token_type(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_EOF: printf