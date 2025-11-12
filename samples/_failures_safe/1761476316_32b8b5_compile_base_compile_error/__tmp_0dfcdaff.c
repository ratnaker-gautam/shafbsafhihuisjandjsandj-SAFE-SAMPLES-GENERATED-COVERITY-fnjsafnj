//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: parser_tokenizer
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
    TOKEN_EOF
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
    int line;
};

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int parse_number(const char *input, int *pos, char *buffer) {
    int start = *pos;
    int len = 0;
    
    while (isdigit(input[*pos])) {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        buffer[len++] = input[*pos];
        (*pos)++;
    }
    
    if (input[*pos] == '.') {
        buffer[len++] = '.';
        (*pos)++;
        while (isdigit(input[*pos])) {
            if (len >= MAX_TOKEN_LEN - 1) return 0;
            buffer[len++] = input[*pos];
            (*pos)++;
        }
    }
    
    buffer[len] = '\0';
    return len > 0;
}

int parse_string(const char *input, int *pos, char *buffer) {
    if (input[*pos] != '"') return 0;
    
    int len = 0;
    (*pos)++;
    
    while (input[*pos] != '"' && input[*pos] != '\0') {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        buffer[len++] = input[*pos];
        (*pos)++;
    }
    
    if (input[*pos] != '"') return 0;
    (*pos)++;
    buffer[len] = '\0';
    return 1;
}

int parse_identifier(const char *input, int *pos, char *buffer) {
    if (!isalpha(input[*pos]) && input[*pos] != '_') return 0;
    
    int len = 0;
    while (isalnum(input[*pos]) || input[*pos] == '_') {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        buffer[len++] = input[*pos];
        (*pos)++;
    }
    
    buffer[len] = '\0';
    return len > 0;
}

int parse_operator(const char *input, int *pos, char *buffer) {
    if (!is_operator_char(input[*pos])) return 0;
    
    int len = 0;
    while (is_operator_char(input[*pos])) {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        buffer[len++] = input[*pos];
        (*pos)++;
    }
    
    buffer[len] = '\0';
    return len > 0;
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
        } else if (parse_string(input, &pos, buffer)) {
            tokens[token_count].type = TOKEN_STRING;
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
        }
        
        if (!success) {
            pos++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
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
    if (fgets(input, sizeof(input), stdin) == NULL) {