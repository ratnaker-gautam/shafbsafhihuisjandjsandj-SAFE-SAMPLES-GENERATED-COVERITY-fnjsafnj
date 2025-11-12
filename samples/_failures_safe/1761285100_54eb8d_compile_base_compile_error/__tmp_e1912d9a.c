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

const char *token_type_name(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    int line = 1;
    int column = 1;
    
    if (input == NULL || tokens == NULL || max_tokens <= 0) {
        return 0;
    }
    
    while (input[pos] != '\0' && token_count < max_tokens) {
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
        
        struct Token *token = &tokens[token_count];
        token->line = line;
        token->column = column;
        
        if (isdigit((unsigned char)input[pos])) {
            token->type = TOKEN_NUMBER;
            int i = 0;
            while (isdigit((unsigned char)input[pos]) && i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = input[pos++];
                column++;
            }
            token->value[i] = '\0';
            token_count++;
        } else if (isalpha((unsigned char)input[pos]) || input[pos] == '_') {
            token->type = TOKEN_IDENTIFIER;
            int i = 0;
            while ((isalnum((unsigned char)input[pos]) || input[pos] == '_') && i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = input[pos++];
                column++;
            }
            token->value[i] = '\0';
            token_count++;
        } else if (input[pos] == '"') {
            token->type = TOKEN_STRING;
            int i = 0;
            pos++;
            column++;
            while (input[pos] != '"' && input[pos] != '\0' && i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = input[pos++];
                column++;
            }
            token->value[i] = '\0';
            if (input[pos] == '"') {
                pos++;
                column++;
            }
            token_count++;
        } else if (is_operator_char(input[pos])) {
            token->type = TOKEN_OPERATOR;
            int op_len = parse_operator(input, pos, token->value);
            if (op_len > 0) {
                pos += op_len;
                column += op_len;
                token_count++;
            } else {
                token->type = TOKEN_UNKNOWN;
                token->value[0] = input[pos++];
                token->value[1] = '\0';
                column++;
                token_count++;
            }
        } else {
            token->type = TOKEN_UNKNOWN;
            token->value[0] = input[pos++];
            token->value[1] = '\0';
            column++;
            token_count++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        tokens[token