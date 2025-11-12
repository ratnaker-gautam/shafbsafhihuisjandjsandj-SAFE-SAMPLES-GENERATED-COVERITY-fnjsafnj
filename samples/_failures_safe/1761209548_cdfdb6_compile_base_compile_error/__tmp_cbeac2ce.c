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
        while (pos < len && isspace(input[pos])) {
            if (input[pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
        }
        
        if (pos >= len) break;
        
        char current = input[pos];
        struct Token *token = &tokens[token_count];
        token->line = line;
        token->column = column;
        
        if (isdigit(current)) {
            int num_len = 0;
            while (pos < len && num_len < MAX_TOKEN_LEN - 1 && isdigit(input[pos])) {
                token->value[num_len++] = input[pos++];
                column++;
            }
            token->value[num_len] = '\0';
            token->type = TOKEN_NUMBER;
            token_count++;
        } else if (isalpha(current) || current == '_') {
            int id_len = 0;
            while (pos < len && id_len < MAX_TOKEN_LEN - 1 && 
                   (isalnum(input[pos]) || input[pos] == '_')) {
                token->value[id_len++] = input[pos++];
                column++;
            }
            token->value[id_len] = '\0';
            token->type = TOKEN_IDENTIFIER;
            token_count++;
        } else if (current == '"') {
            int str_len = 0;
            pos++;
            column++;
            while (pos < len && str_len < MAX_TOKEN_LEN - 1 && input[pos] != '"') {
                if (input[pos] == '\\' && pos + 1 < len) {
                    token->value[str_len++] = input[pos++];
                    column++;
                }
                token->value[str_len++] = input[pos++];
                column++;
            }
            if (pos < len && input[pos] == '"') {
                pos++;
                column++;
            }
            token->value[str_len] = '\0';
            token->type = TOKEN_STRING;
            token_count++;
        } else {
            char op_buf[3];
            int op_len = parse_operator(input, pos, op_buf);
            if (op_len > 0) {
                strcpy(token->value, op_buf);
                token->type = TOKEN_OPERATOR;
                pos += op_len;
                column += op_len;
                token_count++;
            } else {
                token->value[0] = current;
                token->value[1] = '\0';
                token->type = TOKEN_UNKNOWN;
                pos++;
                column++;
                token_count++;
            }
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
        case TOKEN_EOF: printf("EOF"); break;