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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int tokenize(const char* input, struct Token* tokens, int max_tokens) {
    int token_count = 0;
    int line = 1;
    const char* pos = input;
    
    while (*pos && token_count < max_tokens) {
        while (isspace(*pos)) {
            if (*pos == '\n') line++;
            pos++;
        }
        
        if (!*pos) break;
        
        if (isdigit(*pos)) {
            int len = 0;
            while (isdigit(*pos) && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *pos++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            tokens[token_count].line = line;
            token_count++;
        }
        else if (isalpha(*pos) || *pos == '_') {
            int len = 0;
            while ((isalnum(*pos) || *pos == '_') && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *pos++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            tokens[token_count].line = line;
            token_count++;
        }
        else if (is_operator_char(*pos)) {
            int len = 0;
            while (is_operator_char(*pos) && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *pos++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
            tokens[token_count].line = line;
            token_count++;
        }
        else if (*pos == '"') {
            int len = 0;
            pos++;
            while (*pos && *pos != '"' && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *pos++;
            }
            if (*pos == '"') {
                pos++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_STRING;
            tokens[token_count].line = line;
            token_count++;
        }
        else {
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

int main() {
    char input[1024];
    struct Token tokens[MAX_TOKENS];
    
    printf("Enter text to tokenize (max %d chars):\n", 1023);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    printf("\nTokens found:\n");
    printf("Line | Type       | Value\n");
    printf("-----|------------|-----------------\n");
    
    for (int i = 0; i < token_count; i++) {
        printf("%4d | ", tokens[i].line);
        print_token_type(tokens[i].type);
        printf("%*s | %s\n", 11 - (int)strlen("OPERATOR"), "", tokens[i].value);
    }
    
    return 0;
}