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
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || 
           c == '=' || c == '<' || c == '>' || c == '!' ||
           c == '&' || c == '|';
}

int tokenize(const char* input, struct Token* tokens, int max_tokens) {
    int token_count = 0;
    int line = 1;
    int column = 1;
    const char* ptr = input;
    
    while (*ptr && token_count < max_tokens) {
        while (isspace(*ptr)) {
            if (*ptr == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            ptr++;
        }
        
        if (!*ptr) break;
        
        struct Token* token = &tokens[token_count];
        token->line = line;
        token->column = column;
        
        if (isdigit(*ptr)) {
            token->type = TOKEN_NUMBER;
            int len = 0;
            while (isdigit(*ptr) && len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = *ptr++;
                column++;
            }
            token->value[len] = '\0';
            token_count++;
        }
        else if (isalpha(*ptr) || *ptr == '_') {
            token->type = TOKEN_IDENTIFIER;
            int len = 0;
            while ((isalnum(*ptr) || *ptr == '_') && len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = *ptr++;
                column++;
            }
            token->value[len] = '\0';
            token_count++;
        }
        else if (*ptr == '"') {
            token->type = TOKEN_STRING;
            int len = 0;
            ptr++;
            column++;
            while (*ptr != '"' && *ptr && len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = *ptr++;
                column++;
            }
            if (*ptr == '"') {
                ptr++;
                column++;
            }
            token->value[len] = '\0';
            token_count++;
        }
        else if (is_operator_char(*ptr)) {
            token->type = TOKEN_OPERATOR;
            int len = 0;
            while (is_operator_char(*ptr) && len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = *ptr++;
                column++;
            }
            token->value[len] = '\0';
            token_count++;
        }
        else {
            ptr++;
            column++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        tokens[token_count].line = line;
        tokens[token_count].column = column;
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
    
    printf("Enter text to tokenize (max %d chars):\n", (int)sizeof(input) - 1);
    
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
    printf("Line Col Type        Value\n");
    printf("---- --- ----------  -----\n");
    
    for (int i = 0; i < token_count; i++) {
        printf("%3d %3d ", tokens[i].line, tokens[i].column);
        print_token_type(tokens[i].type);
        printf("   %s\n", tokens[i].value);
    }
    
    return 0;
}