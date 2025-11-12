//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50
#define DELIMITERS " \t\n\r"

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
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

void add_token(TokenType type, const char* value) {
    if (token_count >= MAX_TOKENS) {
        fprintf(stderr, "Error: Too many tokens\n");
        exit(1);
    }
    tokens[token_count].type = type;
    strncpy(tokens[token_count].value, value, MAX_TOKEN_LEN - 1);
    tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
    token_count++;
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenType classify_token(const char* str) {
    if (isdigit(str[0])) {
        return TOKEN_NUMBER;
    } else if (isalpha(str[0]) || str[0] == '_') {
        return TOKEN_IDENTIFIER;
    } else if (str[0] == '"') {
        return TOKEN_STRING;
    } else if (is_operator(str[0])) {
        return TOKEN_OPERATOR;
    }
    return TOKEN_UNKNOWN;
}

void tokenize(const char* input) {
    char buffer[MAX_TOKEN_LEN];
    size_t buf_index = 0;
    int in_string = 0;
    
    for (size_t i = 0; input[i] != '\0'; i++) {
        char c = input[i];
        
        if (in_string) {
            if (c == '"') {
                buffer[buf_index] = '\0';
                add_token(TOKEN_STRING, buffer);
                buf_index = 0;
                in_string = 0;
            } else if (buf_index < MAX_TOKEN_LEN - 1) {
                buffer[buf_index++] = c;
            }
            continue;
        }
        
        if (isspace(c)) {
            if (buf_index > 0) {
                buffer[buf_index] = '\0';
                add_token(classify_token(buffer), buffer);
                buf_index = 0;
            }
            continue;
        }
        
        if (is_operator(c)) {
            if (buf_index > 0) {
                buffer[buf_index] = '\0';
                add_token(classify_token(buffer), buffer);
                buf_index = 0;
            }
            buffer[0] = c;
            buffer[1] = '\0';
            add_token(TOKEN_OPERATOR, buffer);
            continue;
        }
        
        if (c == '"') {
            if (buf_index > 0) {
                buffer[buf_index] = '\0';
                add_token(classify_token(buffer), buffer);
                buf_index = 0;
            }
            in_string = 1;
            continue;
        }
        
        if (buf_index < MAX_TOKEN_LEN - 1) {
            buffer[buf_index++] = c;
        }
    }
    
    if (buf_index > 0) {
        buffer[buf_index] = '\0';
        add_token(classify_token(buffer), buffer);
    }
    
    add_token(TOKEN_EOF, "");
}

const char* token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

void print_tokens() {
    printf("Tokens:\n");
    for (size_t i = 0; i < token_count; i++) {
        printf("  %s: '%s'\n", token_type_name(tokens[i].type), tokens[i].value);
    }
}

int main() {
    char input[1024];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Error: Empty input\n");
        return 1;
    }
    
    tokenize(input);
    print_tokens();
    
    return 0;
}