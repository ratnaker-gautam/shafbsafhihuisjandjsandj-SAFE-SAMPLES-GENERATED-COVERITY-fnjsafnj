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
    return strchr("+-*/=<>!&|", c) != NULL;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int line = 1;
    int token_count = 0;
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        while (isspace(input[pos])) {
            if (input[pos] == '\n') line++;
            pos++;
        }
        
        if (input[pos] == '\0') break;
        
        struct Token *token = &tokens[token_count];
        token->line = line;
        
        if (isdigit(input[pos])) {
            token->type = TOKEN_NUMBER;
            int i = 0;
            while (isdigit(input[pos]) && i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = input[pos++];
            }
            token->value[i] = '\0';
            token_count++;
        }
        else if (isalpha(input[pos]) || input[pos] == '_') {
            token->type = TOKEN_IDENTIFIER;
            int i = 0;
            while ((isalnum(input[pos]) || input[pos] == '_') && i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = input[pos++];
            }
            token->value[i] = '\0';
            token_count++;
        }
        else if (is_operator_char(input[pos])) {
            token->type = TOKEN_OPERATOR;
            int i = 0;
            while (is_operator_char(input[pos]) && i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = input[pos++];
            }
            token->value[i] = '\0';
            token_count++;
        }
        else if (input[pos] == '"') {
            token->type = TOKEN_STRING;
            int i = 0;
            pos++;
            while (input[pos] != '"' && input[pos] != '\0' && i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = input[pos++];
            }
            token->value[i] = '\0';
            if (input[pos] == '"') pos++;
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

int main(void) {
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
    printf("Line | Type       | Value\n");
    printf("-----+------------+-----------------\n");
    
    for (int i = 0; i < token_count; i++) {
        printf("%4d | ", tokens[i].line);
        print_token_type(tokens[i].type);
        printf("%*s | %s\n", 11 - (int)strlen("IDENTIFIER") + (int)strlen("IDENTIFIER"), "", tokens[i].value);
    }
    
    return 0;
}