//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64
#define DELIMITERS " \t\n\r"

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

Token tokens[MAX_TOKENS];
size_t token_count = 0;
size_t current_token = 0;

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '(' || c == ')';
}

void tokenize(const char* input) {
    const char* p = input;
    token_count = 0;
    
    while (*p && token_count < MAX_TOKENS) {
        while (*p && strchr(DELIMITERS, *p)) p++;
        if (!*p) break;
        
        if (isdigit(*p)) {
            size_t len = 0;
            while (isdigit(*p) && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *p++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
        }
        else if (isalpha(*p) || *p == '_') {
            size_t len = 0;
            while ((isalnum(*p) || *p == '_') && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *p++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            token_count++;
        }
        else if (is_operator(*p)) {
            tokens[token_count].value[0] = *p++;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
            token_count++;
        }
        else if (*p == '"') {
            size_t len = 0;
            p++;
            while (*p && *p != '"' && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *p++;
            }
            if (*p == '"') p++;
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_STRING;
            token_count++;
        }
        else {
            p++;
        }
    }
    
    if (token_count < MAX_TOKENS) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
    }
}

void print_token_type(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_EOF: printf("EOF"); break;
    }
}

int parse_expression(void);
int parse_term(void);
int parse_factor(void);

int parse_factor(void) {
    if (current_token >= token_count) return 0;
    
    if (tokens[current_token].type == TOKEN_NUMBER) {
        int value = atoi(tokens[current_token].value);
        current_token++;
        return value;
    }
    else if (tokens[current_token].type == TOKEN_IDENTIFIER) {
        current_token++;
        return 1;
    }
    else if (tokens[current_token].type == TOKEN_OPERATOR && 
             tokens[current_token].value[0] == '(') {
        current_token++;
        int result = parse_expression();
        if (current_token < token_count && tokens[current_token].type == TOKEN_OPERATOR && 
            tokens[current_token].value[0] == ')') {
            current_token++;
            return result;
        }
        return 0;
    }
    return 0;
}

int parse_term(void) {
    int result = parse_factor();
    
    while (current_token < token_count && tokens[current_token].type == TOKEN_OPERATOR) {
        char op = tokens[current_token].value[0];
        if (op == '*' || op == '/') {
            current_token++;
            int right = parse_factor();
            if (op == '*') result *= right;
            else if (right != 0) result /= right;
        }
        else break;
    }
    return result;
}

int parse_expression(void) {
    int result = parse_term();
    
    while (current_token < token_count && tokens[current_token].type == TOKEN_OPERATOR) {
        char op = tokens[current_token].value[0];
        if (op == '+' || op == '-') {
            current_token++;
            int right = parse_term();
            if (op == '+') result += right;
            else result -= right;
        }
        else break;
    }
    return result;
}

int parse(void) {
    current_token = 0;
    return parse_expression();
}

int main(void) {
    char input[1024];
    
    printf("Enter expression to parse: ");
    if (fgets(input, sizeof(input), stdin) ==