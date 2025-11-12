//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
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
    TOKEN_EOF
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
    int line;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_number(const char *input, int *pos, char *buffer) {
    int i = 0;
    while (isdigit(input[*pos])) {
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        buffer[i++] = input[*pos];
        (*pos)++;
    }
    buffer[i] = '\0';
    return 1;
}

int parse_identifier(const char *input, int *pos, char *buffer) {
    int i = 0;
    if (!isalpha(input[*pos]) && input[*pos] != '_') return 0;
    
    while (isalnum(input[*pos]) || input[*pos] == '_') {
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        buffer[i++] = input[*pos];
        (*pos)++;
    }
    buffer[i] = '\0';
    return 1;
}

int parse_operator(const char *input, int *pos, char *buffer) {
    int i = 0;
    if (!is_operator_char(input[*pos])) return 0;
    
    buffer[i++] = input[*pos];
    (*pos)++;
    
    if (input[*pos] == '=' && is_operator_char(buffer[0])) {
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        buffer[i++] = input[*pos];
        (*pos)++;
    }
    
    buffer[i] = '\0';
    return 1;
}

int parse_string(const char *input, int *pos, char *buffer) {
    int i = 0;
    if (input[*pos] != '"') return 0;
    
    buffer[i++] = input[*pos];
    (*pos)++;
    
    while (input[*pos] != '"' && input[*pos] != '\0') {
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        buffer[i++] = input[*pos];
        (*pos)++;
    }
    
    if (input[*pos] == '"') {
        buffer[i++] = input[*pos];
        (*pos)++;
        buffer[i] = '\0';
        return 1;
    }
    
    return 0;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    int line = 1;
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        if (isspace(input[pos])) {
            if (input[pos] == '\n') line++;
            pos++;
            continue;
        }
        
        char buffer[MAX_TOKEN_LEN];
        int success = 0;
        
        if (parse_number(input, &pos, buffer)) {
            tokens[token_count].type = TOKEN_NUMBER;
            strcpy(tokens[token_count].value, buffer);
            tokens[token_count].line = line;
            token_count++;
            success = 1;
        } else if (parse_identifier(input, &pos, buffer)) {
            tokens[token_count].type = TOKEN_IDENTIFIER;
            strcpy(tokens[token_count].value, buffer);
            tokens[token_count].line = line;
            token_count++;
            success = 1;
        } else if (parse_operator(input, &pos, buffer)) {
            tokens[token_count].type = TOKEN_OPERATOR;
            strcpy(tokens[token_count].value, buffer);
            tokens[token_count].line = line;
            token_count++;
            success = 1;
        } else if (parse_string(input, &pos, buffer)) {
            tokens[token_count].type = TOKEN_STRING;
            strcpy(tokens[token_count].value, buffer);
            tokens[token_count].line = line;
            token_count++;
            success = 1;
        }
        
        if (!success) {
            fprintf(stderr, "Error: Invalid character '%c' at line %d\n", input[pos], line);
            return -1;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "");
        tokens[token_count].line = line;
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
    }
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin