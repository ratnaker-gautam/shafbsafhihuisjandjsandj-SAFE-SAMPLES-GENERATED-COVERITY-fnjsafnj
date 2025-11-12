//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_LEN 256
#define MAX_TOKENS 1000

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_PAREN,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

typedef struct {
    Token tokens[MAX_TOKENS];
    int count;
    int pos;
} Tokenizer;

void init_tokenizer(Tokenizer *t) {
    t->count = 0;
    t->pos = 0;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

void add_token(Tokenizer *t, TokenType type, const char *value) {
    if (t->count >= MAX_TOKENS) return;
    Token *token = &t->tokens[t->count++];
    token->type = type;
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
}

int tokenize(Tokenizer *t, const char *input) {
    if (!input) return 0;
    
    int len = strlen(input);
    int i = 0;
    
    while (i < len) {
        if (isspace(input[i])) {
            i++;
            continue;
        }
        
        if (isdigit(input[i])) {
            char num[MAX_TOKEN_LEN] = {0};
            int j = 0;
            while (i < len && isdigit(input[i]) && j < MAX_TOKEN_LEN - 1) {
                num[j++] = input[i++];
            }
            add_token(t, TOKEN_NUMBER, num);
            continue;
        }
        
        if (isalpha(input[i])) {
            char ident[MAX_TOKEN_LEN] = {0};
            int j = 0;
            while (i < len && (isalnum(input[i]) || input[i] == '_') && j < MAX_TOKEN_LEN - 1) {
                ident[j++] = input[i++];
            }
            add_token(t, TOKEN_IDENTIFIER, ident);
            continue;
        }
        
        if (is_operator_char(input[i])) {
            char op[2] = {input[i++], '\0'};
            add_token(t, TOKEN_OPERATOR, op);
            continue;
        }
        
        if (input[i] == '(' || input[i] == ')') {
            char paren[2] = {input[i++], '\0'};
            add_token(t, TOKEN_PAREN, paren);
            continue;
        }
        
        i++;
    }
    
    add_token(t, TOKEN_EOF, "");
    return 1;
}

void print_token_type(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_PAREN: printf("PAREN"); break;
        case TOKEN_EOF: printf("EOF"); break;
    }
}

void print_tokens_recursive(Tokenizer *t, int index) {
    if (index >= t->count) return;
    
    printf("Token %d: Type=", index);
    print_token_type(t->tokens[index].type);
    printf(", Value='%s'\n", t->tokens[index].value);
    
    print_tokens_recursive(t, index + 1);
}

int parse_expression_recursive(Tokenizer *t, int *index);

int parse_primary_recursive(Tokenizer *t, int *index) {
    if (*index >= t->count) return 0;
    
    Token *token = &t->tokens[*index];
    
    if (token->type == TOKEN_NUMBER || token->type == TOKEN_IDENTIFIER) {
        (*index)++;
        return 1;
    }
    
    if (token->type == TOKEN_PAREN && token->value[0] == '(') {
        (*index)++;
        if (!parse_expression_recursive(t, index)) return 0;
        
        if (*index >= t->count) return 0;
        token = &t->tokens[*index];
        if (token->type == TOKEN_PAREN && token->value[0] == ')') {
            (*index)++;
            return 1;
        }
        return 0;
    }
    
    return 0;
}

int parse_expression_recursive(Tokenizer *t, int *index) {
    if (!parse_primary_recursive(t, index)) return 0;
    
    if (*index >= t->count) return 1;
    
    Token *token = &t->tokens[*index];
    if (token->type == TOKEN_OPERATOR) {
        (*index)++;
        if (!parse_primary_recursive(t, index)) return 0;
    }
    
    return 1;
}

int main(void) {
    char input[4096];
    printf("Enter expression to tokenize and parse: ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        printf("Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen