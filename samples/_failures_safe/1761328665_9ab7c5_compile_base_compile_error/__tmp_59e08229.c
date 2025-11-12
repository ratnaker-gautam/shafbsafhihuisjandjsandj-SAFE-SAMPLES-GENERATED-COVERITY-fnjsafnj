//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
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
    TOKEN_EOF,
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int is_valid_operator(const char *str) {
    return strcmp(str, "+") == 0 || strcmp(str, "-") == 0 ||
           strcmp(str, "*") == 0 || strcmp(str, "/") == 0 ||
           strcmp(str, "=") == 0 || strcmp(str, "==") == 0 ||
           strcmp(str, "!=") == 0 || strcmp(str, "<") == 0 ||
           strcmp(str, ">") == 0 || strcmp(str, "<=") == 0 ||
           strcmp(str, ">=") == 0 || strcmp(str, "&&") == 0 ||
           strcmp(str, "||") == 0;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int token_count = 0;
    int line = 1;
    int column = 1;
    const char *ptr = input;
    
    while (*ptr != '\0' && token_count < max_tokens) {
        while (isspace(*ptr)) {
            if (*ptr == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            ptr++;
        }
        
        if (*ptr == '\0') break;
        
        struct Token *token = &tokens[token_count];
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
        } else if (isalpha(*ptr) || *ptr == '_') {
            token->type = TOKEN_IDENTIFIER;
            int len = 0;
            while ((isalnum(*ptr) || *ptr == '_') && len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = *ptr++;
                column++;
            }
            token->value[len] = '\0';
            token_count++;
        } else if (*ptr == '"') {
            token->type = TOKEN_STRING;
            int len = 0;
            ptr++;
            column++;
            while (*ptr != '"' && *ptr != '\0' && len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = *ptr++;
                column++;
            }
            if (*ptr == '"') {
                ptr++;
                column++;
            }
            token->value[len] = '\0';
            token_count++;
        } else if (is_operator_char(*ptr)) {
            token->type = TOKEN_OPERATOR;
            int len = 0;
            while (is_operator_char(*ptr) && len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = *ptr++;
                column++;
            }
            token->value[len] = '\0';
            if (!is_valid_operator(token->value)) {
                token->type = TOKEN_INVALID;
            }
            token_count++;
        } else {
            token->type = TOKEN_INVALID;
            token->value[0] = *ptr++;
            token->value[1] = '\0';
            column++;
            token_count++;
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
        case TOKEN_INVALID: printf("INVALID"); break;
    }
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 1) {
        fprintf(stderr, "Empty input\n");
        return 1