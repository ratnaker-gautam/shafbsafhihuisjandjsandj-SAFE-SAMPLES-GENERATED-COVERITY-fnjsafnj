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
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

Token tokens[MAX_TOKENS];
size_t token_count = 0;
size_t current_token = 0;

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

void tokenize(const char* input) {
    const char* p = input;
    token_count = 0;
    
    while (*p && token_count < MAX_TOKENS) {
        while (*p && strchr(DELIMITERS, *p)) p++;
        if (!*p) break;
        
        if (isdigit(*p)) {
            size_t len = 0;
            while (*p && isdigit(*p) && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *p++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
        } else if (isalpha(*p) || *p == '_') {
            size_t len = 0;
            while (*p && (isalnum(*p) || *p == '_') && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *p++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            token_count++;
        } else if (is_operator(*p)) {
            size_t len = 0;
            while (*p && is_operator(*p) && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *p++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
            token_count++;
        } else if (*p == '"') {
            size_t len = 0;
            p++;
            while (*p && *p != '"' && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *p++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_STRING;
            token_count++;
            if (*p == '"') p++;
        } else {
            tokens[token_count].value[0] = *p++;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = TOKEN_UNKNOWN;
            token_count++;
        }
    }
    
    if (token_count < MAX_TOKENS) {
        tokens[token_count].type = TOKEN_EOF;
        strncpy(tokens[token_count].value, "", MAX_TOKEN_LEN);
    }
}

void print_token_type(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_EOF: printf("EOF"); break;
        default: printf("UNKNOWN"); break;
    }
}

void parse_expression() {
    if (current_token >= token_count) return;
    
    if (tokens[current_token].type == TOKEN_NUMBER || tokens[current_token].type == TOKEN_IDENTIFIER) {
        printf("Parsed value: %s\n", tokens[current_token].value);
        current_token++;
        
        if (current_token < token_count && tokens[current_token].type == TOKEN_OPERATOR) {
            printf("Parsed operator: %s\n", tokens[current_token].value);
            current_token++;
            parse_expression();
        }
    } else if (tokens[current_token].type == TOKEN_STRING) {
        printf("Parsed string: \"%s\"\n", tokens[current_token].value);
        current_token++;
    }
}

int main() {
    char input[1024];
    
    printf("Enter expression to parse: ");
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strcspn(input, "\n");
    if (len < sizeof(input)) {
        input[len] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    tokenize(input);
    
    printf("\nTokens found:\n");
    for (size_t i = 0; i < token_count; i++) {
        printf("Token %zu: ", i + 1);
        print_token_type(tokens[i].type);
        printf(" ('%s')\n", tokens[i].value);
    }
    
    printf("\nParsing expression:\n");
    current_token = 0;
    parse_expression();
    return