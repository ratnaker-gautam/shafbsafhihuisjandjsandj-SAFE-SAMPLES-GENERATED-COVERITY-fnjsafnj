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

void print_token(const struct Token *token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "ERROR"
    };
    
    printf("Line %d, Column %d: %s", token->line, token->column, type_names[token->type]);
    
    if (token->type != TOKEN_EOF) {
        printf(" '%s'", token->value);
    }
    
    printf("\n");
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int line = 1;
    int column = 1;
    int token_count = 0;
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        if (isspace((unsigned char)input[pos])) {
            if (input[pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
            continue;
        }
        
        tokens[token_count].line = line;
        tokens[token_count].column = column;
        
        if (isdigit((unsigned char)input[pos])) {
            tokens[token_count].type = TOKEN_NUMBER;
            int i = 0;
            while (isdigit((unsigned char)input[pos]) && i < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[i++] = input[pos++];
                column++;
            }
            tokens[token_count].value[i] = '\0';
            token_count++;
        } else if (isalpha((unsigned char)input[pos]) || input[pos] == '_') {
            tokens[token_count].type = TOKEN_IDENTIFIER;
            int i = 0;
            while ((isalnum((unsigned char)input[pos]) || input[pos] == '_') && i < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[i++] = input[pos++];
                column++;
            }
            tokens[token_count].value[i] = '\0';
            token_count++;
        } else if (is_operator_char(input[pos])) {
            tokens[token_count].type = TOKEN_OPERATOR;
            int op_len = parse_operator(input, pos, tokens[token_count].value);
            pos += op_len;
            column += op_len;
            token_count++;
        } else if (input[pos] == '"') {
            tokens[token_count].type = TOKEN_STRING;
            int i = 0;
            tokens[token_count].value[i++] = input[pos++];
            column++;
            
            while (input[pos] != '"' && input[pos] != '\0' && i < MAX_TOKEN_LEN - 2) {
                if (input[pos] == '\\' && input[pos + 1] != '\0') {
                    tokens[token_count].value[i++] = input[pos++];
                    tokens[token_count].value[i++] = input[pos++];
                    column += 2;
                } else {
                    tokens[token_count].value[i++] = input[pos++];
                    column++;
                }
            }
            
            if (input[pos] == '"') {
                tokens[token_count].value[i++] = input[pos++];
                column++;
                tokens[token_count].value[i] = '\0';
                token_count++;
            } else {
                tokens[token_count].type = TOKEN_ERROR;
                strcpy(tokens[token_count].value, "Unterminated string");
                token_count++;
                break;
            }
        } else {
            tokens[token_count].type = TOKEN_ERROR;
            tokens[token_count].value[0