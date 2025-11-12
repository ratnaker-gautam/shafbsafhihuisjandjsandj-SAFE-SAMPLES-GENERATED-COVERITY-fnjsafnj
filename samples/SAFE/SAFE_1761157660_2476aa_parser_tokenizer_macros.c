//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50
#define DELIMITERS " \t\n"

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

Token* tokenize(const char* input, int* token_count) {
    static Token tokens[MAX_TOKENS];
    *token_count = 0;
    int pos = 0;
    int len = strlen(input);
    
    while (pos < len && *token_count < MAX_TOKENS) {
        while (pos < len && isspace(input[pos])) pos++;
        if (pos >= len) break;
        
        Token* token = &tokens[*token_count];
        
        if (isdigit(input[pos])) {
            token->type = TOKEN_NUMBER;
            int i = 0;
            while (pos < len && i < MAX_TOKEN_LEN - 1 && isdigit(input[pos])) {
                token->value[i++] = input[pos++];
            }
            token->value[i] = '\0';
            (*token_count)++;
        }
        else if (isalpha(input[pos]) || input[pos] == '_') {
            token->type = TOKEN_IDENTIFIER;
            int i = 0;
            while (pos < len && i < MAX_TOKEN_LEN - 1 && 
                   (isalnum(input[pos]) || input[pos] == '_')) {
                token->value[i++] = input[pos++];
            }
            token->value[i] = '\0';
            (*token_count)++;
        }
        else if (is_operator(input[pos])) {
            token->type = TOKEN_OPERATOR;
            token->value[0] = input[pos++];
            token->value[1] = '\0';
            (*token_count)++;
        }
        else if (input[pos] == '"') {
            token->type = TOKEN_STRING;
            int i = 0;
            pos++;
            while (pos < len && i < MAX_TOKEN_LEN - 1 && input[pos] != '"') {
                token->value[i++] = input[pos++];
            }
            if (pos < len && input[pos] == '"') pos++;
            token->value[i] = '\0';
            (*token_count)++;
        }
        else {
            pos++;
        }
    }
    
    if (*token_count < MAX_TOKENS) {
        tokens[*token_count].type = TOKEN_EOF;
        strcpy(tokens[*token_count].value, "");
        (*token_count)++;
    }
    
    return tokens;
}

void print_token_type(TokenType type) {
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
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    int token_count;
    Token* tokens = tokenize(input, &token_count);
    
    printf("Tokens found: %d\n", token_count);
    printf("Type\t\tValue\n");
    printf("----\t\t-----\n");
    
    for (int i = 0; i < token_count; i++) {
        print_token_type(tokens[i].type);
        printf("\t\t%s\n", tokens[i].value);
    }
    
    return 0;
}