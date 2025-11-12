//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50
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
    const char* ptr = input;
    token_count = 0;
    
    while (*ptr && token_count < MAX_TOKENS) {
        while (*ptr && strchr(DELIMITERS, *ptr)) ptr++;
        if (!*ptr) break;
        
        if (isdigit(*ptr)) {
            size_t len = 0;
            while (isdigit(*ptr) && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *ptr++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
        }
        else if (isalpha(*ptr) || *ptr == '_') {
            size_t len = 0;
            while ((isalnum(*ptr) || *ptr == '_') && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *ptr++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            token_count++;
        }
        else if (is_operator(*ptr)) {
            tokens[token_count].value[0] = *ptr++;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
            token_count++;
        }
        else if (*ptr == '"') {
            ptr++;
            size_t len = 0;
            while (*ptr && *ptr != '"' && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *ptr++;
            }
            if (*ptr == '"') ptr++;
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_STRING;
            token_count++;
        }
        else {
            ptr++;
        }
    }
    
    if (token_count < MAX_TOKENS) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
    }
}

void print_token(Token token) {
    const char* type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF"
    };
    printf("%s: '%s'\n", type_names[token.type], token.value);
}

int parse_expression(void);
int parse_term(void);
int parse_factor(void);

int parse_factor(void) {
    if (current_token >= token_count) return 0;
    
    Token token = tokens[current_token];
    if (token.type == TOKEN_NUMBER) {
        current_token++;
        return atoi(token.value);
    }
    else if (token.type == TOKEN_IDENTIFIER) {
        current_token++;
        return 0;
    }
    else if (token.type == TOKEN_OPERATOR && token.value[0] == '(') {
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

int main(void) {
    char input[1024];
    
    printf("Enter expression: ");
    if (!fgets(input, sizeof(input), stdin)) {
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    tokenize(input);
    
    printf("\nTokens:\n");
    for (size_t i