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
            tokens[token_count].type = TOKEN_NUMBER;
            tokens[token_count].line = line;
            tokens[token_count].column = column;
            
            int len = 0;
            while (isdigit(input[pos]) && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = input[pos++];
                column++;
            }
            tokens[token_count].value[len] = '\0';
            token_count++;
            continue;
        }
        
        if (isalpha(input[pos]) || input[pos] == '_') {
            tokens[token_count].type = TOKEN_IDENTIFIER;
            tokens[token_count].line = line;
            tokens[token_count].column = column;
            
            int len = 0;
            while ((isalnum(input[pos]) || input[pos] == '_') && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = input[pos++];
                column++;
            }
            tokens[token_count].value[len] = '\0';
            token_count++;
            continue;
        }
        
        if (input[pos] == '"') {
            tokens[token_count].type = TOKEN_STRING;
            tokens[token_count].line = line;
            tokens[token_count].column = column;
            
            int len = 0;
            tokens[token_count].value[len++] = input[pos++];
            column++;
            
            while (input[pos] != '"' && input[pos] != '\0' && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = input[pos++];
                column++;
            }
            
            if (input[pos] == '"') {
                tokens[token_count].value[len++] = input[pos++];
                column++;
                tokens[token_count].value[len] = '\0';
                token_count++;
            } else {
                tokens[token_count].type = TOKEN_ERROR;
                strcpy(tokens[token_count].value, "Unterminated string");
                token_count++;
                break;
            }
            continue;
        }
        
        if (is_operator_char(input[pos])) {
            tokens[token_count].type = TOKEN_OPERATOR;
            tokens[token_count].line = line;
            tokens[token_count].column = column;
            
            int op_len = parse_operator(input, pos, tokens[token_count].value);
            pos += op_len;
            column += op_len;
            token_count++;
            continue;
        }
        
        tokens[token_count].type = TOKEN_ERROR;
        tokens[token_count].line = line;
        tokens[token_count].column = column;
        snprintf(tokens[token_count].value, MAX_TOKEN_LEN, "Unexpected character: %c", input[pos]);
        token_count++;
        pos++;
        column++;
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].line = line;
        tokens[token_count].column = column;
        strcpy(tokens[token_count].value, "EOF");
        token_count++;