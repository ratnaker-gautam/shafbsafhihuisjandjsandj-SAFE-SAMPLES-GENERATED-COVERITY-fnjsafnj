//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64
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
    const char* p = input;
    token_count = 0;
    
    while (*p && token_count < MAX_TOKENS) {
        while (*p && strchr(DELIMITERS, *p)) p++;
        if (!*p) break;
        
        if (IS_OPERATOR(*p)) {
            tokens[token_count].type = TOKEN_OPERATOR;
            tokens[token_count].value[0] = *p;
            tokens[token_count].value[1] = '\0';
            token_count++;
            p++;
            continue;
        }
        
        if (isdigit(*p)) {
            size_t len = 0;
            while (*p && isdigit(*p) && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *p++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
            continue;
        }
        
        if (isalpha(*p) || *p == '_') {
            size_t len = 0;
            while (*p && (isalnum(*p) || *p == '_') && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *p++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            token_count++;
            continue;
        }
        
        p++;
    }
    
    tokens[token_count].type = TOKEN_END;
}

Token* next_token(void) {
    if (current_token >= token_count) {
        return &tokens[token_count];
    }
    return &tokens[current_token++];
}

Token* peek_token(void) {
    if (current_token >= token_count) {
        return &tokens[token_count];
    }
    return &tokens[current_token];
}

void reset_parser(void) {
    current_token = 0;
}

void print_tokens(void) {
    for (size_t i = 0; i < token_count; i++) {
        const char* type_str = "UNKNOWN";
        switch (tokens[i].type) {
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case TOKEN_IDENTIFIER: type_str = "IDENTIFIER"; break;
            case TOKEN_OPERATOR: type_str = "OPERATOR"; break;
            case TOKEN_END: type_str = "END"; break;
        }
        printf("Token %zu: %s ('%s')\n", i, type_str, tokens[i].value);
    }
}

int parse_expression(void);

int parse_factor(void) {
    Token* token = next_token();
    if (token->type == TOKEN_NUMBER) {
        return atoi(token->value);
    } else if (token->type == TOKEN_IDENTIFIER) {
        return 0;
    } else if (token->type == TOKEN_OPERATOR && token->value[0] == '(') {
        int result = parse_expression();
        token = next_token();
        if (token->type != TOKEN_OPERATOR || token->value[0] != ')') {
            return 0;
        }
        return result;
    }
    return 0;
}

int parse_term(void) {
    int result = parse_factor();
    Token* token = peek_token();
    
    while (token->type == TOKEN_OPERATOR && 
          (token->value[0] == '*' || token->value[0] == '/')) {
        next_token();
        int right = parse_factor();
        
        if (token->value[0] == '*') {
            result *= right;
        } else if (token->value[0] == '/') {
            if (right != 0) {
                result /= right;
            }
        }
        token = peek_token();
    }
    return result;
}

int parse_expression(void) {
    int result = parse_term();
    Token* token = peek_token();
    
    while (token->type == TOKEN_OPERATOR && 
          (token->value[0] == '+' || token->value[0] == '-')) {
        next_token();
        int right = parse_term();
        
        if (token->value[0] == '+') {
            result += right;
        } else if (token->value[0] == '-') {
            result -= right;
        }
        token = peek_token();
    }
    return result;
}

int main(void) {
    char input[1024];
    
    printf("Enter expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(input);
    if (len >