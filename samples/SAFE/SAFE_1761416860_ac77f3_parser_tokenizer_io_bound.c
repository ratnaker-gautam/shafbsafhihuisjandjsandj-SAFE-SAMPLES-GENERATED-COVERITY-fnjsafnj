//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_IDENTIFIER,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_UNKNOWN
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
};

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '(' || c == ')';
}

int parse_number(const char *input, int *pos, char *buffer) {
    int start = *pos;
    int len = 0;
    
    while (isdigit(input[*pos])) {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        buffer[len++] = input[(*pos)++];
    }
    
    buffer[len] = '\0';
    return len > 0;
}

int parse_identifier(const char *input, int *pos, char *buffer) {
    int start = *pos;
    int len = 0;
    
    if (isalpha(input[*pos]) || input[*pos] == '_') {
        buffer[len++] = input[(*pos)++];
        while (isalnum(input[*pos]) || input[*pos] == '_') {
            if (len >= MAX_TOKEN_LEN - 1) break;
            buffer[len++] = input[(*pos)++];
        }
        buffer[len] = '\0';
        return 1;
    }
    return 0;
}

int parse_string(const char *input, int *pos, char *buffer) {
    int len = 0;
    
    if (input[*pos] == '"') {
        (*pos)++;
        while (input[*pos] != '"' && input[*pos] != '\0') {
            if (len >= MAX_TOKEN_LEN - 1) break;
            buffer[len++] = input[(*pos)++];
        }
        if (input[*pos] == '"') {
            (*pos)++;
            buffer[len] = '\0';
            return 1;
        }
    }
    return 0;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        while (isspace(input[pos])) pos++;
        
        if (input[pos] == '\0') break;
        
        if (parse_number(input, &pos, tokens[token_count].value)) {
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
            continue;
        }
        
        if (parse_identifier(input, &pos, tokens[token_count].value)) {
            tokens[token_count].type = TOKEN_IDENTIFIER;
            token_count++;
            continue;
        }
        
        if (parse_string(input, &pos, tokens[token_count].value)) {
            tokens[token_count].type = TOKEN_STRING;
            token_count++;
            continue;
        }
        
        if (is_operator(input[pos])) {
            tokens[token_count].value[0] = input[pos];
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
            token_count++;
            pos++;
            continue;
        }
        
        tokens[token_count].value[0] = input[pos];
        tokens[token_count].value[1] = '\0';
        tokens[token_count].type = TOKEN_UNKNOWN;
        token_count++;
        pos++;
    }
    
    tokens[token_count].type = TOKEN_EOF;
    strcpy(tokens[token_count].value, "EOF");
    token_count++;
    
    return token_count;
}

void print_token_type(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_EOF: printf("EOF"); break;
        case TOKEN_UNKNOWN: printf("UNKNOWN"); break;
    }
}

int main() {
    char input[1024];
    struct Token tokens[MAX_TOKENS];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    printf("\nTokens found:\n");
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: Type=", i);
        print_token_type(tokens[i].type);
        printf(", Value='%s'\n", tokens[i].value);
    }
    
    return 0;
}