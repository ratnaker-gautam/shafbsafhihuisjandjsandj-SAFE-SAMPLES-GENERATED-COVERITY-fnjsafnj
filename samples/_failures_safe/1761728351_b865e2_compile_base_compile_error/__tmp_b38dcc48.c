//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
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
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_number(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    int has_dot = 0;
    
    while (isdigit(input[*pos]) || input[*pos] == '.') {
        if (input[*pos] == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        }
        (*pos)++;
    }
    
    int len = *pos - start;
    if (len <= 0 || len >= MAX_TOKEN_LEN) return 0;
    
    strncpy(token->value, &input[start], len);
    token->value[len] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_identifier(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    
    if (!isalpha(input[*pos]) && input[*pos] != '_') return 0;
    (*pos)++;
    
    while (isalnum(input[*pos]) || input[*pos] == '_') {
        (*pos)++;
    }
    
    int len = *pos - start;
    if (len <= 0 || len >= MAX_TOKEN_LEN) return 0;
    
    strncpy(token->value, &input[start], len);
    token->value[len] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_operator(const char *input, int *pos, struct Token *token) {
    if (!is_operator_char(input[*pos])) return 0;
    
    int start = *pos;
    (*pos)++;
    
    if (is_operator_char(input[*pos])) {
        (*pos)++;
    }
    
    int len = *pos - start;
    if (len <= 0 || len >= MAX_TOKEN_LEN) return 0;
    
    strncpy(token->value, &input[start], len);
    token->value[len] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

int parse_string(const char *input, int *pos, struct Token *token) {
    if (input[*pos] != '"') return 0;
    
    int start = *pos;
    (*pos)++;
    
    while (input[*pos] != '\0' && input[*pos] != '"') {
        if (input[*pos] == '\\') {
            (*pos)++;
        }
        (*pos)++;
    }
    
    if (input[*pos] != '"') return 0;
    (*pos)++;
    
    int len = *pos - start;
    if (len <= 0 || len >= MAX_TOKEN_LEN) return 0;
    
    strncpy(token->value, &input[start], len);
    token->value[len] = '\0';
    token->type = TOKEN_STRING;
    return 1;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        while (isspace(input[pos])) pos++;
        if (input[pos] == '\0') break;
        
        struct Token token;
        int success = 0;
        
        if (parse_number(input, &pos, &token)) success = 1;
        else if (parse_string(input, &pos, &token)) success = 1;
        else if (parse_identifier(input, &pos, &token)) success = 1;
        else if (parse_operator(input, &pos, &token)) success = 1;
        
        if (success) {
            if (token_count < max_tokens) {
                tokens[token_count++] = token;
            }
        } else {
            pos++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "");
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

int main() {
    char input[MAX_INPUT_SIZE];
    struct Token tokens[MAX_TOKENS];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len =