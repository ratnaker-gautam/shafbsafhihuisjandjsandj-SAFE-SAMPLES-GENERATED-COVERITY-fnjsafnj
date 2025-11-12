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

int tokenize(const char* input, Token tokens[], int max_tokens);
void print_tokens(const Token tokens[], int count);

int main(void) {
    char input[1024];
    Token tokens[MAX_TOKENS];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    if (token_count < 0) {
        fprintf(stderr, "Tokenization failed\n");
        return 1;
    }
    
    print_tokens(tokens, token_count);
    return 0;
}

int tokenize(const char* input, Token tokens[], int max_tokens) {
    if (input == NULL || tokens == NULL || max_tokens <= 0) {
        return -1;
    }
    
    int token_count = 0;
    const char* pos = input;
    
    while (*pos != '\0' && token_count < max_tokens) {
        while (isspace((unsigned char)*pos)) {
            pos++;
        }
        
        if (*pos == '\0') {
            break;
        }
        
        if (isdigit((unsigned char)*pos)) {
            int i = 0;
            while (isdigit((unsigned char)*pos) && i < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[i++] = *pos++;
            }
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
        }
        else if (isalpha((unsigned char)*pos) || *pos == '_') {
            int i = 0;
            while ((isalnum((unsigned char)*pos) || *pos == '_') && i < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[i++] = *pos++;
            }
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            token_count++;
        }
        else if (*pos == '"') {
            int i = 0;
            pos++;
            while (*pos != '"' && *pos != '\0' && i < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[i++] = *pos++;
            }
            if (*pos == '"') {
                pos++;
            }
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = TOKEN_STRING;
            token_count++;
        }
        else if (strchr("+-*/=<>!&|", *pos) != NULL) {
            int i = 0;
            while (strchr("+-*/=<>!&|", *pos) != NULL && i < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[i++] = *pos++;
            }
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
            token_count++;
        }
        else {
            tokens[token_count].value[0] = *pos++;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
            token_count++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "EOF");
        token_count++;
    }
    
    return token_count;
}

void print_tokens(const Token tokens[], int count) {
    if (tokens == NULL || count <= 0) {
        return;
    }
    
    printf("Tokens found: %d\n", count);
    printf("Type\t\tValue\n");
    printf("----\t\t-----\n");
    
    for (int i = 0; i < count; i++) {
        switch (tokens[i].type) {
            case TOKEN_NUMBER:
                printf("NUMBER\t\t%s\n", tokens[i].value);
                break;
            case TOKEN_IDENTIFIER:
                printf("IDENTIFIER\t%s\n", tokens[i].value);
                break;
            case TOKEN_OPERATOR:
                printf("OPERATOR\t%s\n", tokens[i].value);
                break;
            case TOKEN_STRING:
                printf("STRING\t\t\"%s\"\n", tokens[i].value);
                break;
            case TOKEN_EOF:
                printf("EOF\t\t%s\n", tokens[i].value);
                break;
        }
    }
}