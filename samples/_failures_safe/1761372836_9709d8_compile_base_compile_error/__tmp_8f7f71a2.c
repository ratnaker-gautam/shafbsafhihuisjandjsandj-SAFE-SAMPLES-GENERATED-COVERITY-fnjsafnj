//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: parser_tokenizer
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

int is_valid_identifier_start(char c) {
    return isalpha(c) || c == '_';
}

int is_valid_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

int tokenize(const char* input, struct Token* tokens, int max_tokens) {
    int token_count = 0;
    int pos = 0;
    int line = 1;
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        while (isspace(input[pos])) {
            if (input[pos] == '\n') line++;
            pos++;
        }
        
        if (input[pos] == '\0') break;
        
        if (isdigit(input[pos])) {
            int start = pos;
            while (isdigit(input[pos])) pos++;
            if (token_count < max_tokens) {
                tokens[token_count].type = TOKEN_NUMBER;
                strncpy(tokens[token_count].value, &input[start], pos - start);
                tokens[token_count].value[pos - start] = '\0';
                tokens[token_count].line = line;
                token_count++;
            }
            continue;
        }
        
        if (is_valid_identifier_start(input[pos])) {
            int start = pos;
            while (is_valid_identifier_char(input[pos])) pos++;
            if (token_count < max_tokens) {
                tokens[token_count].type = TOKEN_IDENTIFIER;
                strncpy(tokens[token_count].value, &input[start], pos - start);
                tokens[token_count].value[pos - start] = '\0';
                tokens[token_count].line = line;
                token_count++;
            }
            continue;
        }
        
        if (input[pos] == '"') {
            int start = ++pos;
            while (input[pos] != '"' && input[pos] != '\0') {
                if (input[pos] == '\n') line++;
                pos++;
            }
            if (input[pos] == '"') {
                if (token_count < max_tokens) {
                    tokens[token_count].type = TOKEN_STRING;
                    strncpy(tokens[token_count].value, &input[start], pos - start);
                    tokens[token_count].value[pos - start] = '\0';
                    tokens[token_count].line = line;
                    token_count++;
                }
                pos++;
            }
            continue;
        }
        
        if (is_operator_char(input[pos])) {
            int start = pos;
            while (is_operator_char(input[pos])) pos++;
            if (token_count < max_tokens) {
                tokens[token_count].type = TOKEN_OPERATOR;
                strncpy(tokens[token_count].value, &input[start], pos - start);
                tokens[token_count].value[pos - start] = '\0';
                tokens[token_count].line = line;
                token_count++;
            }
            continue;
        }
        
        pos++;
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

void print_tokens(const struct Token* tokens, int count) {
    for (int i = 0; i < count; i++) {
        printf("Line %d: ", tokens[i].line);
        print_token_type(tokens[i].type);
        printf(" '%s'\n", tokens[i].value);
    }
}

int main() {
    char input[1024];
    struct Token tokens[MAX_TOKENS];
    
    printf("Enter input to tokenize (max %d chars):\n", 1023);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
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
    
    if (token_count >= MAX_TOKENS) {
        printf("Warning: Maximum token count reached\n");
    }
    
    printf("\nTokens found:\n");
    print_tokens(tokens, token