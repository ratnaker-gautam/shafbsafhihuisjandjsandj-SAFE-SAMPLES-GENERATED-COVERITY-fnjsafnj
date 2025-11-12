//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50
#define DELIMITERS " \t\n\r"
#define IS_OPERATOR(c) (c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '(' || c == ')')

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_END
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

Token tokens[MAX_TOKENS];
size_t token_count = 0;
size_t current_token = 0;

void tokenize(const char* input) {
    const char* ptr = input;
    token_count = 0;
    
    while (*ptr != '\0' && token_count < MAX_TOKENS) {
        while (isspace((unsigned char)*ptr)) ptr++;
        
        if (*ptr == '\0') break;
        
        if (IS_OPERATOR(*ptr)) {
            tokens[token_count].type = TOKEN_OPERATOR;
            tokens[token_count].value[0] = *ptr;
            tokens[token_count].value[1] = '\0';
            token_count++;
            ptr++;
            continue;
        }
        
        if (isdigit((unsigned char)*ptr)) {
            size_t len = 0;
            while (isdigit((unsigned char)*ptr) && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *ptr++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
            continue;
        }
        
        if (isalpha((unsigned char)*ptr) || *ptr == '_') {
            size_t len = 0;
            while ((isalnum((unsigned char)*ptr) || *ptr == '_') && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *ptr++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            token_count++;
            continue;
        }
        
        ptr++;
    }
    
    tokens[token_count].type = TOKEN_END;
    tokens[token_count].value[0] = '\0';
}

Token get_next_token(void) {
    if (current_token >= token_count) {
        Token end_token = {TOKEN_END, ""};
        return end_token;
    }
    return tokens[current_token++];
}

void unget_token(void) {
    if (current_token > 0) current_token--;
}

int parse_expression(void);

int parse_factor(void) {
    Token token = get_next_token();
    
    if (token.type == TOKEN_NUMBER) {
        return atoi(token.value);
    }
    
    if (token.type == TOKEN_IDENTIFIER) {
        return 0;
    }
    
    if (token.type == TOKEN_OPERATOR && token.value[0] == '(') {
        int result = parse_expression();
        token = get_next_token();
        if (token.type != TOKEN_OPERATOR || token.value[0] != ')') {
            return 0;
        }
        return result;
    }
    
    unget_token();
    return 0;
}

int parse_term(void) {
    int result = parse_factor();
    Token token = get_next_token();
    
    while (token.type == TOKEN_OPERATOR && 
           (token.value[0] == '*' || token.value[0] == '/')) {
        int right = parse_factor();
        if (token.value[0] == '*') {
            result *= right;
        } else if (right != 0) {
            result /= right;
        }
        token = get_next_token();
    }
    
    unget_token();
    return result;
}

int parse_expression(void) {
    int result = parse_term();
    Token token = get_next_token();
    
    while (token.type == TOKEN_OPERATOR && 
           (token.value[0] == '+' || token.value[0] == '-')) {
        int right = parse_term();
        if (token.value[0] == '+') {
            result += right;
        } else {
            result -= right;
        }
        token = get_next_token();
    }
    
    unget_token();
    return result;
}

int main(void) {
    char input[1024];
    
    printf("Enter expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    tokenize(input);
    
    printf("Tokens:\n");
    for (size_t i = 0; i < token_count; i++) {
        const char* type_str = "";
        switch (tokens[i].type) {
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case TOKEN_IDENTIFIER: type_str = "IDENTIFIER"; break;
            case TOKEN_OPERATOR: type_str = "OPERATOR"; break;
            default: type_str = "UNKNOWN"; break;
        }
        printf("  %s: '%s'\n", type_str, tokens[i].value);
    }
    
    current_token =