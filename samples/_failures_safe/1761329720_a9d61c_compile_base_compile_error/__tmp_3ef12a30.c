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
    return strchr("+-*/=<>!&|", c) != NULL;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int line = 1;
    int column = 1;
    int token_count = 0;
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        while (isspace(input[pos])) {
            if (input[pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
        }
        
        if (input[pos] == '\0') break;
        
        tokens[token_count].line = line;
        tokens[token_count].column = column;
        
        if (isdigit(input[pos])) {
            int len = 0;
            while (isdigit(input[pos]) && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = input[pos++];
                column++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
        }
        else if (isalpha(input[pos]) || input[pos] == '_') {
            int len = 0;
            while ((isalnum(input[pos]) || input[pos] == '_') && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = input[pos++];
                column++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            token_count++;
        }
        else if (is_operator_char(input[pos])) {
            int len = 0;
            while (is_operator_char(input[pos]) && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = input[pos++];
                column++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
            token_count++;
        }
        else if (input[pos] == '"') {
            int len = 0;
            tokens[token_count].value[len++] = input[pos++];
            column++;
            
            while (input[pos] != '"' && input[pos] != '\0' && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = input[pos++];
                column++;
            }
            
            if (input[pos] == '"') {
                tokens[token_count].value[len++] = input[pos++];
                column++;
                tokens[token_count].value[len] = '\0';
                tokens[token_count].type = TOKEN_STRING;
                token_count++;
            } else {
                return -1;
            }
        }
        else {
            pos++;
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
    
    if (len == 0) {
        printf("Empty input\n");
        return 0;
    }
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count < 0) {
        fprintf(stderr, "Error: Unterminated string literal\n");
        return 1;
    }
    
    if (token_count == MAX_TOKENS) {
        printf("Warning: Maximum token count reached\n");
    }
    
    printf("\nTokens found:\n");
    printf("Type       Value               Line  Column\n");
    printf("---------- ------------------- ----- ------\n");
    
    for (int i = 0; i < token_count; i++) {
        printf("%-10s ", "");
        print_token_type(tokens[i].type);
        printf(" %-18s %5d %6d\n